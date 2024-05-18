//
// Created by ADMIN on 02-Mar-24.
//

#ifndef MOTION_GAMESERVER_HPP
#define MOTION_GAMESERVER_HPP

#include <boost/asio.hpp>
#include <iostream>
#include <bitset>
#include <glm/vec2.hpp>
#include <boost/serialization/access.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <span>
#include "serialization.hpp"
#include "Context.hpp"
#include "../Player.hpp"
#include "Header.hpp"

//using byte_ostream = std::basic_ostream<std::byte>;
//using byte_istream = std::basic_istream<std::byte>;


constexpr bool enable_logs = true;

typedef std::array<std::byte, 4> communication_protocol_t;
constexpr communication_protocol_t COMMUNICATION_PROTOCOL = {std::byte(0x70),
                                                             std::byte(0x42),
                                                             std::byte(0xE1),
                                                             std::byte(0x1C)};


//using Player = std::uint32_t;



struct Client {
  ConnectionInfo connectionInfo;
  Player player{};
};


class GameServer {
  ContextManager contextManager{};
public:
  using udp = boost::asio::ip::udp;

  explicit GameServer(boost::asio::io_context &ioContext, unsigned short port = 8080) :
      socket{ioContext, udp::endpoint(udp::v4(), port)} {
    std::cout << "starting server on udp address " << socket.local_endpoint() << std::endl;
    recv_buf.getContainer().resize(512); // violating open-closed principle with no doubt
    start_receive();
  }

  void send_all(std::string_view message) {
    for (auto &[key, client]: clients) {
      auto header = client.connectionInfo.getNextHeader();
      byte_ostream buf;
      buf << COMMUNICATION_PROTOCOL;
      buf << header;
      buf << std::span(message);

      socket.async_send_to(boost::asio::buffer(buf.getContainer()), key,
                           std::bind(&GameServer::handle_write, this,
                                     std::placeholders::_1,
                                     std::placeholders::_2));
    }
  }

  void send_all(const byte_ostream &message) {
    for (auto &[key, client]: clients) {
      auto header = client.connectionInfo.getNextHeader();
      byte_ostream buf;
      buf << COMMUNICATION_PROTOCOL;
      buf << header;
      buf << message; // extra copy
      // perhaps only header should change and the data, common for the players, should not change

      socket.async_send_to(boost::asio::buffer(buf.getContainer()), key,
                           std::bind(&GameServer::handle_write, this,
                                     std::placeholders::_1,
                                     std::placeholders::_2));
    }
  }

private:

  void start_receive() {
    remote_endpoint = udp::endpoint();
    recv_buf.reset();
    socket.async_receive_from(boost::asio::buffer(recv_buf.getContainer()),
                              remote_endpoint,
                              std::bind(&GameServer::handle_receive, this,
                                        std::placeholders::_1,
                                        std::placeholders::_2));

  }


  void handle_receive(const boost::system::error_code &error,
                      std::size_t bytes_transferred) {

    if (!error) {
      communication_protocol_t comm;
      recv_buf.set_size(bytes_transferred);
      recv_buf >> comm;

      if (std::memcmp(comm.data(), COMMUNICATION_PROTOCOL.data(), COMMUNICATION_PROTOCOL.size()) == 0) {

        auto &client = clients[remote_endpoint];
        byte_ostream resp_buf;
        resp_buf << COMMUNICATION_PROTOCOL;
        if (bytes_transferred == sizeof(COMMUNICATION_PROTOCOL))
          std::cout << "client has connected: " << remote_endpoint << std::endl;
        else {
          Header header;
          recv_buf >> header;
          client.connectionInfo.updateWithRemote(header);
          resp_buf << client.connectionInfo.getNextHeader();
          resp_buf << SERVER_MESSAGE::RESPONSE;
          resp_buf << header.sequence; // response to

          contextManager.parse(recv_buf, resp_buf, client.player); // return a vector
          /**           what is the logic behind delayed execution?
           * that some data is bound to a specific command?
           * why not just function arguments?
           *  should it actually just generate the response (as the responsibility of get_command idea)?
           *  and make the response optional?
           *  how response should be differentiated from the command?
           *  can clients send the response-like data? because all commands are data and not all data is commands
           *  response (to relative sequence) (those, who wait for it lnow how to parse it) |
           *  command (as specified before)
           *  message (that is not bound to any sequence)
           *  can server command the client with the same protocol? no - the commands are not symmetric
           *  but the commands that are called "messages" can use the same concept,
           *  although with
           *  a different protocol and dirrected from a server to the client
           *  are there any compulsory messages that, must respond to?
           *  level of compulsion:
           *  1. (must respond)
           *     - an ack
           *  2. (must react, or the client will be in an invalid state)
           *  3. (informative)
           *  (1 and 2 can be combined)
           *  i propose to sperate those messages by flags
           *  e.g. msg | ack | react
           *  e.g. command | context_a | context_b ...
           *  should command and msg be number or a bit?
           *  restriction: only 8 bits for all flags and types
           *  first 4 bits for type, last 4 for flags
           *  can last 4 bits behave differently for different types?
           *  can the game state be considered a message?
           *  */

        }


        socket.async_send_to(boost::asio::buffer(resp_buf.getContainer()), remote_endpoint,
                             std::bind(&GameServer::handle_send, this, boost::asio::buffer(resp_buf.getContainer()),
                                       std::placeholders::_1,
                                       std::placeholders::_2));

      } else {
        if constexpr (enable_logs) {
          std::cout << "unauthorized byte sequence " << std::hex;
          for (auto c: recv_buf.getContainer()) {
            std::cout << std::hex << (short) c;
          }
          std::cout << std::dec << std::endl;
        }
      }
      start_receive();

    }

  };

  void handle_send(std::any /*message*/,
                   const boost::system::error_code & /*error*/,
                   std::size_t /*bytes_transferred*/) {
  }


  void handle_write(
      const boost::system::error_code & /*error*/,
      std::size_t /*bytes_transferred*/) {
  }

  udp::endpoint remote_endpoint;
  udp::socket socket;
//  std::array<std::byte, 512> recv_buf{};
  byte_istream recv_buf{};

  class UdpHasher {
  public:
    std::size_t operator()(const udp::endpoint &endpoint) const {
      return std::hash<std::uint64_t>{}(endpoint.address().to_v4().to_uint()) + std::hash<unsigned short>{}(endpoint.port());
    }
  };

  std::unordered_map<udp::endpoint, Client, UdpHasher> clients;

};

/* every state requires a unique representation of the state sent over*/
//enum BattleCommand {
//  /*note: unit id is unique for all units*/
//  A_ATTACK_B, /*unit id, other unit id; assert units belong to diff players*/
//  A_GOTO_P, /*unit id, pos (vec2) assert pos in map*/
//  A_STOP, /*unit id; assert the player owns the unit*/
//  A_REFORMTO_F, /*unit id, new formation shape (u8 w, u8 h), new rotation; assert n <= w*h*/
//};
//
//enum DeploymentCommand {
//  A_REFORMTO_F,
//  A_GOTO_P,
//  TOGGLE_CONFIRM, /*toggle readiness*/
//};
//enum ArmyCreationCommand {
//  ADD_UNIT_T, /*unit template id*/
//  REMOVE_UNIT_A, /* remove unit id (id of prototype) */
//  SET_FACTION_F, /*set faction*/
//  SET_TEAM_T, /*select team*/
//  TOGGLE_CONFIRM, /*toggle readiness*/
//};
//struct Setup {
//  std::uint8_t teams;
//  std::uint8_t players;
//  std::uint8_t AIs;
//  std::uint8_t map;
//  std::uint32_t budget;
//};
//enum SetupCommand {
//  /* must be setup as a single struct atomically by host. partial transactions do not make sense*/
//  SET_N_TEAMS,
//  SET_M_PLAYERS,
//  SET_K_AIS,
//  SET_BUDGET_B,
//  SET_MAP_M,
//};
//struct CommandHeader {
//  BattleCommand command;
//  std::uint8_t bytes[16];
//};
//
//struct A_ATTACK_B_Header {
//  Unit a;
//  Unit b;
//};
//
//struct A_GOTO_P_Header {
//  Unit a;
//  glm::vec2 p;
//  /*
//   * adds destination component to a unit. if at some epsilon from dest stop movement
//   * adds movement component to the unit
//   * */
//};
//struct Entity2D {
//  glm::vec2 location;
//};
//struct Movement2DComponent {
//  glm::vec2 direction;
//};
//struct Destination2DComponent {
//  glm::vec2 destination;
//};
//struct Target2DComponent {
//  const std::uint32_t entityID;
//};
//struct HealthComponent {
//  std::uint32_t health;
//  const std::uint32_t defence; /*possible feature "defence cooldown"*/
//};
//struct AttackComponent {
//  /*references some default config for unit*/
//  const std::uint32_t damage;
//  std::uint32_t cooldown;
//};
//
//struct A_STOP_Header {
//  Unit a;
//};
//
//struct A_REFORMTO_F_Header {
//  Unit a;
//  std::uint8_t w;
//  std::uint8_t h;
//  float d;
//};


#endif //MOTION_GAMESERVER_HPP
