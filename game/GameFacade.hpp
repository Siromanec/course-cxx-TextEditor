//
// Created by ADMIN on 28-Feb-24.
//

#ifndef MOTION_GAMEFACADE_HPP
#define MOTION_GAMEFACADE_HPP


#include <thread>
#include <future>
#include "TacticalGame.hpp"
#include "TacticalGameState.hpp"

class GameFacade {
  TacticalGame game;
  std::thread game_thread;
  Player player;
  static GameFacade *instance;

  GameFacade() : game_thread{std::thread{&TacticalGame::run, &game}}, player{} { // or do connection to a server with the game

    player.id = game.getAvailablePlayerId();

    game.join(player);
  }

  ~GameFacade() {
    game_thread.join();
  }

public:
  static GameFacade *getInstance() {
    if (instance == nullptr) { instance = new GameFacade{};}
    return instance;
  }

  GameFacade(const GameFacade&) = delete;
  GameFacade& operator=(const GameFacade&) = delete;



  void command(int c) {
    game.command(player, c);
  };

  std::future<TacticalGameState> getState() {
    return game.getState(player);
  }
};

GameFacade * GameFacade::instance = nullptr;
#endif //MOTION_GAMEFACADE_HPP
