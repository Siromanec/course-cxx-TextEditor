//
// Created by ADMIN on 02-May-24.
//

#ifndef MOTION_CONTEXT_HPP
#define MOTION_CONTEXT_HPP


#include <memory>
#include <utility>
#include <unordered_map>
#include <iostream>
#include <optional>
#include "serialization.hpp"
#include "../Player.hpp"
#include "Response.hpp"
#include "LobbyConfig.hpp"


class Context : public std::enable_shared_from_this<Context> {
public:
  virtual void parse(byte_istream &command,
                     byte_ostream &response,
                     Player player) = 0;
};

//class Response {
//protected:
//  std::weak_ptr<Context> context;
//  Player player;
//public:
//  Response(std::weak_ptr<Context> context, Player player) : context{std::move(context)}, player{player} {}
//
//  virtual void operator()() = 0;
//};

class ContextManager;

class PlayerManager;

class LobbyManager;

class ArmyCreationManager;

class DeploymentManager;

class BattleManager;

class Lobby {
  /* Players, State or Current context */
  std::unordered_set<Player> players;
  Player admin;
  LobbyConfig config;
public:
  Lobby(LobbyConfig, const Player& admin) : admin{admin} { add_player(admin); }

  [[nodiscard]] const std::unordered_set<Player> &get_players() const { return players; }

  [[nodiscard]] const Player &get_admin() const { return admin; }

  [[nodiscard]] bool is_admin(const Player& player) const { return admin == player; }

  [[nodiscard]] bool has_player(const Player& player) const { return players.contains(player); }

  [[nodiscard]] bool is_full() const { return players.size() == config.max_players; }

  [[nodiscard]] bool is_empty() const { return players.empty(); }

  void erase_player(const Player& player) { players.erase(player); }

  void add_player(const Player& player) {
    players.insert(player);
  }
  const LobbyConfig &get_config() const { return config; }
};


class ContextManager : public Context {
public:
  typedef std::size_t LobbyId;
  typedef std::unordered_map<LobbyId, Lobby> open_lobbies_t;
private:
  std::unique_ptr<LobbyManager> lobbyManager;
  std::unique_ptr<ArmyCreationManager> armyCreationManager;
  open_lobbies_t open_lobbies{};
  enum MANAGER_TYPE : uint8_t {
    LOBBY_MANAGER = 0x1,
    ARMY_CREATION_MANAGER = 0x2,
    DEPLOYMENT_MANAGER = 0x3,
    BATTLE_MANAGER = 0x4,
  };
  /*PLAYERS*/
  std::optional<uint64_t> lobby_containing_player(const Player& player) {
    for (auto &[id, lobby]: open_lobbies) {
      if (lobby.has_player(player)) {
        return id;
      }
    }
    return std::nullopt;
  }

  friend class LobbyManager;
  friend class ArmyCreationManager;
public:

  ContextManager() : lobbyManager{std::make_unique<LobbyManager>(*this, open_lobbies)},
                     armyCreationManager{std::make_unique<ArmyCreationManager>(*this, open_lobbies)} {}

  void parse(byte_istream &istream, byte_ostream &response, Player player) final;

};


/*
 * contains data required for one game. During battle may start a game in another thread */
// hierarchy of states make server not stateless. but it is a game...
// i can force the client to know which state it is in

/**
 * manages and stores multiple lobbies
 * */



class LobbyManager : public Context {

  enum LOBBY_CONTEXT : uint8_t {
    CREATE_LOBBY = 0x1,
    JOIN = 0x2,
    GET_LOBBIES = 0x3,
    LEAVE = 0x4,
    GET_PLAYERS = 0x5,
    START = 0x6,
  };

  void parse_join_command(byte_istream &args, byte_ostream &response, Player player) {
    auto id = contextManager.lobby_containing_player(player);
    if (id.has_value()) {
      response << Response::ERROR << Response::ERROR_TYPE::CONFLICT; // already in a lobby
      return;
    }

    std::uint64_t lobby_id;
    args >> lobby_id;
    auto lobby = open_lobbies.find(lobby_id);
    if (lobby == open_lobbies.end()) {
      response << Response::ERROR << Response::ERROR_TYPE::NOT_FOUND;
      return;
    }

    lobby->second.add_player(player);

  }


  void parse_create_lobby(byte_istream &args, byte_ostream &response, const Player &player) {
    auto id = contextManager.lobby_containing_player(player);
    if (id.has_value()) { //todo change to <Player, Lobby> in JOINED_LOBBIES
      response << Response::ERROR << Response::ERROR_TYPE::CONFLICT; // already in a lobby
      return;
    }
    LobbyConfig config{};
    args >> config;
    open_lobbies.emplace(++last_lobby_id, Lobby{config, player});

    // todo: transition to new context

  }

  void parse_get_lobbies(byte_istream &args, byte_ostream &response, const Player &player) {
    response << Response::DATA;
    response << open_lobbies.size();
    for (const auto &[id, lobby]: open_lobbies) {
      response << id;
    }
  }



  void parse_get_players(byte_istream &args, byte_ostream &response, const Player& player) {
    std::uint64_t lobby_id;
    args >> lobby_id;

    auto lobby = open_lobbies.find(lobby_id);
    if (lobby == open_lobbies.end()) {
      response << Response::ERROR << Response::ERROR_TYPE::NOT_FOUND;
      return;
    }
    response << Response::DATA;
    response << lobby->second.get_players().size();

    for (const auto &p: lobby->second.get_players()) {
      response << p;
    };

  }

  void parse_leave(byte_istream &args, byte_ostream &response, Player player) {
    auto id = contextManager.lobby_containing_player(player);
    if (id.has_value()) {
      if (open_lobbies.at(id.value()).is_admin(player)) {
        open_lobbies.erase(id.value());
        // todo notify other players of the end of the lobby
      }
      else {
        open_lobbies.at(id.value()).erase_player(player);
      }
      return;
    }
    response << Response::ERROR << Response::ERROR_TYPE::INVALID_STATE; // not in any lobby

  }

public:
  LobbyManager(ContextManager &contextManager,
               typename ContextManager::open_lobbies_t &open_lobbies) :
               contextManager(contextManager),
               open_lobbies{open_lobbies}
               {}

  void parse(byte_istream &command, byte_ostream &response, Player player) final {
    LOBBY_CONTEXT context;
    command >> context;
    switch (context) {
      case JOIN: {
        parse_join_command(command, response, player);
        break;
      }
      case CREATE_LOBBY: {
        parse_create_lobby(command, response, player);
        break;
      }
      case GET_LOBBIES: {
        parse_get_lobbies(command, response, player);
        break;
      }
      case GET_PLAYERS: {
        parse_get_players(command, response, player);
        break;
      }
      case LEAVE: {
        parse_leave(command, response, player);
        break;
      }
      default: {
        response << Response::ERROR << Response::ERROR_TYPE::INVALID_COMMAND;
      }
    }
  };
private:
  ContextManager &contextManager;
  typename ContextManager::open_lobbies_t &open_lobbies;
  typename ContextManager::LobbyId last_lobby_id{0};


};

// given time constraints, the game would be a free for all, with no teams and no factions
// (anyway, the things i would come up would not be much different or require much more unique implementations)
// also no AIs
class ArmyCreationManager: public Context {
  enum ARMY_CREATION_CONTEXT : uint8_t {
    TOGGLE_READY = 0x1,
    UNITS_ADD = 0x2,
    START = 0x3,
  };


  enum UNIT_TEMPLATE : uint8_t {
    PEASANTS = 0x0, // tha meat
    INFANTRY = 0x1, // average stats
    ARMOURED_INFANTRY = 0x2, // slow, ok damage, heavily armoured, small range
    SHOCK_INFANTRY = 0x3, // slow and high damage, low armor, big range
    CAVALRY = 0x2, // like infantry but faster and higher damage due to the speed
    // this should be migrated to some config file but i cannot be bothered rn
    // balance is a future issue
  };

  const std::unordered_map<UNIT_TEMPLATE, std::uint32_t> unitCosts = {
    {PEASANTS, 10},
    {INFANTRY, 20},
    {ARMOURED_INFANTRY, 30},
    {SHOCK_INFANTRY, 30},
    {CAVALRY, 50},
  };

  typedef std::unordered_map<UNIT_TEMPLATE, std::uint8_t> UnitConfig;
  std::unordered_map<Player, UnitConfig> player_units;
  std::unordered_set<Player> ready_players;
  void parse_toggle_ready(byte_istream &args, byte_ostream &response, const Player& player) {
    // todo
    ready_players.insert(player);
  }


  void parse_units_add(byte_istream &args, byte_ostream &response, const Player& player) {
    // todo
    // i will force the client to send all data in one batch



    auto id = contextManager.lobby_containing_player(player);
    if (!id.has_value()) { //todo change to <Player, Lobby> in JOINED_LOBBIES
      response << Response::ERROR << Response::ERROR_TYPE::CONFLICT; // already in a lobby
      return;
    }
    auto & lobby = open_lobbies.at(id.value());
    const auto & config = lobby.get_config();
    std::uint64_t total_cost = 0; // to prevent overflow attacks
    std::uint64_t total_units = 0;

    UnitConfig units;

    std::uint8_t num_unit_types;

    args >> num_unit_types;

    for (std::uint8_t i = 0; i < num_unit_types; ++i) {
      UNIT_TEMPLATE unit_type;
      std::uint8_t num_units;
      args >> unit_type;
      args >> num_units;
      if ( ! unitCosts.contains(unit_type)) {
        response << Response::ERROR << Response::ERROR_TYPE::INVALID_ARGUMENTS;
        return;
      }

      units[unit_type] = num_units;
      total_cost += unitCosts.at(unit_type) * num_units;
      total_units += num_units;
    }
    if (total_cost > config.budget) {
      response << Response::ERROR << Response::ERROR_TYPE::CONFLICT;
      return;
    }
    if (total_units > config.max_units) {
      response << Response::ERROR << Response::ERROR_TYPE::CONFLICT;
      return;

    }
    player_units[player] = units;
  }

  void parse_start(byte_istream &args, byte_ostream &response, const Player& player) {
    // todo
    auto id = contextManager.lobby_containing_player(player);
    if (!id.has_value()) { //todo change to <Player, Lobby> in JOINED_LOBBIES
      response << Response::ERROR << Response::ERROR_TYPE::CONFLICT; // already in a lobby
      return;
    }
    auto & lobby = open_lobbies.at(id.value());
    if (player != lobby.get_admin()) {
      response << Response::ERROR << Response::ERROR_TYPE::UNAUTHORIZED;
      return;
    }
    const auto & config = lobby.get_config();
    // create battle object
    // start loop
  }
public:
  ArmyCreationManager(ContextManager &contextManager,
                      typename ContextManager::open_lobbies_t &open_lobbies) :
                      contextManager(contextManager),
                      open_lobbies{open_lobbies}
                      {}

  void parse(byte_istream &command, byte_ostream &response, Player player) override {
    ARMY_CREATION_CONTEXT context;
    command >> context;
    switch (context) {
      case TOGGLE_READY: {
        parse_toggle_ready(command, response, player);
        break;
      }
      case UNITS_ADD: {
        parse_units_add(command, response, player);
        break;
      }
      case START: {
        parse_start(command, response, player);
        break;
      }
      default: {
        response << Response::ERROR << Response::ERROR_TYPE::INVALID_COMMAND;
      }
    }

  }

private:
  ContextManager &contextManager;
  typename ContextManager::open_lobbies_t &open_lobbies;
};

//      case START: {
// check if admin
// deny access
// send available units?? or make it a resource file?
// send budget
// start accepting unit configurations?
// how is the data from lobby manager forwarded to army creation manager?
// context manager magic ???
// context manager is notified that a lobby is ready to start
// lobby manager moves the lobby to context manager, which forwards it to army creation manager
// lobby is deleted from lobby manager or std::variant<Lobby, StartedLobby>
//
// create instance of army creation manager
// delete lobby or mark as started // std::variant<Lobby, StartedLobby>

// once finished, create instance of deployment manager
// interface:
//    // this should actually be included in another manager - creation manager, because players should not wait for admin to setup lobby
//    <admin> create_lobby new_lobby -> // a map with <player, lobby>
//    <admin> create_lobby weather set <weather_enum>
//    <admin> create_lobby time set <time_enum>
//    <admin> create_lobby map set <map_id>
//    <admin> create_lobby map list -> num map_id map_id ... map_id // should come with the client or by more dynmic?        //    <admin> create_lobby budget set <budget>
//    <admin> create_lobby ai add <team_id>
//    <admin> create_lobby ai remove <team_id>
//    <admin> create_lobby cancel
//    <admin> create_lobby open
//    <admin> create_lobby max_players set <num>
//    <admin> max_teams
//    these are commands that are compiled on the client side, and then sent with `create_lobby open` as one package
//    different clients may have different defaults
//
//    <player> army_create toggle_ready // this seperation forces client to keep track of its state
//    <player> army_create unit add <unit_template_id>
//    <player> army_create unit remove <unit_id>
//    <player> army_create units add <num> <unit_template_id> <unit_template_id>... <unit_template_id>
//    <player> army_create team join <team>
//    <player> army_create team parse_leave
//    <player> army_create set_faction <faction_id>
//    <admin>  army_create start

// how respoonses may work?
// every command has some sequence number? there should be diifferent types of responses from the server
// 1. targetted response - response to a specific command: data or error
// 2. broadcast message - response to all players (or message)
// 3. healthcheck - client must provide a response to this message or another - as long as the header with acks is sent back - kind of targeted response
// some errors should be ignored as "incorrect usage of api". only data related errors should be transmitted
// end of stream checks for arrays sent florom a user


//        break;
//      }

#endif //MOTION_CONTEXT_HPP
