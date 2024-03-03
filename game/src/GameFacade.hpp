//
// Created by ADMIN on 28-Feb-24.
//

#ifndef MOTION_GAMEFACADE_HPP
#define MOTION_GAMEFACADE_HPP


#include <thread>
#include <future>
//#include <execution>
#include "TacticalGame.hpp"
#include "TacticalGameState.hpp"

class GameFacade {
  TacticalGame game;
  std::thread game_thread;
  std::thread ai_thread;
  Player player;
  static GameFacade *instance;

  GameFacade() : game_thread{std::thread{&TacticalGame::run, &game}}, ai_thread{[=, this]() {
    Player ai{};
    ai.id = game.getAvailablePlayerId();
    game.join(ai);
    while (true) {
      std::this_thread::sleep_for(std::chrono::milliseconds(5000));
      game.command(ai, 42);
      auto future = game.getState(ai);
      auto state = future.get();
      std::cout << "the tick of the game for ai is " << state.ticks << std::endl;
    }
  }}, player{} { // or do connection to a server with the game

    player.id = game.getAvailablePlayerId();


    game.join(player);
  }

  ~GameFacade() {
    ai_thread.join();
    game_thread.join();
  }

public:
  static GameFacade *getInstance() {
    if (instance == nullptr) { instance = new GameFacade{}; }
    return instance;
  }

  GameFacade(const GameFacade &) = delete;

  GameFacade &operator=(const GameFacade &) = delete;


  void command(int c) {
    game.command(player, c);
  };

  std::future<TacticalGameState> getState() {
    return game.getState(player);
  }
};

GameFacade *GameFacade::instance = nullptr;
#endif //MOTION_GAMEFACADE_HPP
