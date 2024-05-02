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
#include "Player.hpp"

class Command;

class Context : public std::enable_shared_from_this<Context> {
public:
  virtual std::unique_ptr<Command> get_command(byte_istream &, Player player) = 0;
};

class Command {
protected:
  std::weak_ptr<Context> context;
  Player player;
public:
  Command(std::weak_ptr<Context> context, Player player) : context{std::move(context)}, player{player} {}

  virtual void operator()() = 0;
};

class ContextManager;

class PlayerManager;

class LobbyManager;

class ArmyCreationManager;

class DeploymentManager;

class BattleManager;


class ContextManager : public Context {
  std::unique_ptr<LobbyManager> lobbyManager;
  enum MANAGER_TYPE : uint8_t {
    NONE,
    LOBBY_MANAGER = 0x1,
    ARMY_CREATION_MANAGER,
    DEPLOYMENT_MANAGER,
    BATTLE_MANAGER,
  };
  /*PLAYERS*/

public:
  ContextManager() : lobbyManager{std::make_unique<LobbyManager>(*this)} {}

  virtual std::unique_ptr<Command> get_command(byte_istream &istream, Player player) final;

};


/*
 * contains data required for one game. During battle may start a game in another thread */
// hierarchy of states make server not stateless. but it is a game...
// i can force the client to know which state it is in
class Lobby {
  /* Players, State or Current context */
  std::unordered_set<Player> players;
  Player admin;
public:
  Lobby(Player admin) : admin{admin} { add_player(admin); }

  [[nodiscard]] const std::unordered_set<Player> &get_players() const { return players; }

  [[nodiscard]] const Player &get_admin() const { return admin; }

  [[nodiscard]] bool is_admin(Player player) const { return admin == player; }

  [[nodiscard]] bool has_player(Player player) const { return players.contains(player); }

  [[nodiscard]] bool is_full() const { return players.size() == Player::max_players; }

  [[nodiscard]] bool is_empty() const { return players.empty(); }

  void erase_player(Player player) { players.erase(player); }

  void add_player(Player player) {
    players.insert(player);
  }
};

class JoinCommand : public Command {
  Lobby &lobby;
public:
//  using Command::Command;
  JoinCommand(std::weak_ptr<Context> context, Player player, Lobby &lobby) : Command{std::move(context), player}, lobby{lobby} {}

  virtual void operator()() final {
    lobby.add_player(player);
  }
};

//class CreateLobbyCommand: public Command {
//  Lobby & lobby;
//public:
////  using Command::Command;
//  CreateLobbyCommand(std::weak_ptr<Context> context, Player player, Lobby & lobby): Command{std::move(context), player}, lobby{lobby} {}
//  void operator()() final {
//    lobby.add_player(player);
//  }
//};

class DudCommand : public Command {
public:
  using Command::Command;

  virtual void operator()() final {
    std::cerr << "dud command from player: " << (short) static_cast<typename Player::player_id_t>(player) << std::endl;
  };

};

class NopCommand : public Command {
public:
  using Command::Command;

  virtual void operator()() final {
//    std::cerr << "dud command from player: " << static_cast<typename Player::player_id_t>(player) << std::endl;
  };

};

/**
 * manages and stores multiple lobbies
 * */
class LobbyManager : public Context {
  typedef std::size_t LobbyId;
  std::unordered_map<LobbyId, Lobby> lobbies{};
  LobbyId last_lobby_id{0};
  enum LOBBY_CONTEXT : uint8_t {
    NONE,
    CREATE_LOBBY = 0x1,
    JOIN = 0x2,
    GET_LOBBIES = 0x3,
    LEAVE = 0x4,
    GET_PLAYERS = 0x5,
  };

  std::unique_ptr<Command> get_join_command(byte_istream &istream, Player player) {
    auto id = lobby_containing_player(player);
    if (id.has_value()) {
      return std::make_unique<DudCommand>(weak_from_this(), player);
    }

    std::uint64_t lobby_id;
    istream >> lobby_id;
    auto lobby = lobbies.find(lobby_id);
    if (lobby == lobbies.end()) {
      return std::make_unique<DudCommand>(weak_from_this(), player);
    }

    return std::make_unique<JoinCommand>(weak_from_this(), player, lobby->second);
  }

  std::unique_ptr<Command> get_create_lobby(byte_istream &istream, Player player) {
    auto id = lobby_containing_player(player);
    if (id.has_value()) { //todo change to <Player, Lobby> in JOINED_LOBBIES
      return std::make_unique<DudCommand>(weak_from_this(), player);
    }
    lobbies.emplace(++last_lobby_id, Lobby{player});
    return std::make_unique<NopCommand>(weak_from_this(), player);
  }

  std::unique_ptr<Command> get_lobbies(byte_istream &istream, Player player) {
    for (auto &[id, lobby]: lobbies) {

      std::cout << "lobby: " << id << std::endl;
    }
    return std::make_unique<NopCommand>(weak_from_this(), player);


//    return std::make_unique<DudCommand>(weak_from_this(), player);;
  }

  std::optional<uint64_t> lobby_containing_player(Player player) {
    for (auto &[id, lobby]: lobbies) {

      if (lobby.has_player(player)) {
        return id;
      }
    }
    return std::nullopt;
//    throw std::runtime_error("player not in any lobby");
  }

  std::unique_ptr<Command> get_players(byte_istream &istream, Player player) {
    std::uint64_t lobby_id;
    istream >> lobby_id;

    auto lobby = lobbies.find(lobby_id);
    if (lobby == lobbies.end()) {
      return std::make_unique<DudCommand>(weak_from_this(), player);
    }
    std::cout << "lobby: " << lobby->first << std::endl;
    for (const auto &p: lobby->second.get_players()) {
      std::cout << "\tplayer: " << (short) static_cast<typename Player::player_id_t>(p) << std::endl;
    };
    return std::make_unique<NopCommand>(weak_from_this(), player);
//    return std::make_unique<JoinCommand>(weak_from_this(), player, lobby->second);
  }

  std::unique_ptr<Command> leave(byte_istream &istream, Player player) {
    auto id = lobby_containing_player(player);
    if (id.has_value()) {
      if (lobbies.at(id.value()).is_admin(player)) {
        lobbies.erase(id.value());
      }
      else {
        lobbies.at(id.value()).erase_player(player);
      }


      return std::make_unique<NopCommand>(weak_from_this(), player);
    }
    return std::make_unique<DudCommand>(weak_from_this(), player);
  }

public:
  explicit LobbyManager(ContextManager &contextManager) : contextManager(contextManager) {}

  virtual std::unique_ptr<Command> get_command(byte_istream &istream, Player player) final {
    LOBBY_CONTEXT context;
    istream >> context;
    switch (context) {
      case JOIN: {
        return get_join_command(istream, player);
      }
      case CREATE_LOBBY: {
        return get_create_lobby(istream, player);
      }
      case GET_LOBBIES: {
        return get_lobbies(istream, player);
      }
      case GET_PLAYERS: {
        return get_players(istream, player);
      }
      case LEAVE: {
        return leave(istream, player);
      }
      default: {
        throw std::runtime_error("unknown lobby command!");
      }
    }
  };

private:
  ContextManager &contextManager;

};


#endif //MOTION_CONTEXT_HPP
