//
// Created by ADMIN on 25-Feb-24.
//

#ifndef COROUTINES_GAME_HPP
#define COROUTINES_GAME_HPP


#include <iostream>
#include <unordered_map>
#include <future>
#include "clock/GameClock.hpp"
#include "ThreadQueue.h"
#include "Player.hpp"
#include "TacticalGameState.hpp"

class TacticalGame { // todo: refactor to be BattleContext
  std::unordered_map<Player, ThreadQueue<int>> inQueues;
  std::unordered_map<Player, ThreadQueue<std::promise<TacticalGameState>>> outQueues;
  size_t availablePlayerId;
  GameClock clock;


private:
  void update(tick_t tick) {
  }

public:
  void accept(const Player &player) {
    inQueues[player];
    outQueues[player];
  }

  void command(const Player &player, int c) {
    inQueues[player].push(c);
  }

  std::future<TacticalGameState> getState(const Player & player) {
//    inQueue.push(c);
    auto promise = std::promise<TacticalGameState>();
    auto future = promise.get_future();
    outQueues[player].push(std::move(promise));
    return future;
  }

  size_t getAvailablePlayerId() {
    return availablePlayerId++;
  }

  void update(std::chrono::nanoseconds delta_time) {
    clock += delta_time;


    while (clock.hasNext()) {
      tick_t tick = clock.getNext();
      update(tick);
    }
  }

  [[noreturn]] void run() {
//    auto elapsed = std::chrono::milliseconds{};
    auto lastUpdated = std::chrono::steady_clock::now();

    while (true) {
      auto elapsed = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now() - lastUpdated);
      lastUpdated = std::chrono::steady_clock::now();
      int val{};
      // check for command requests
      for (auto & [player, playerInQueue]: inQueues) {
        if (playerInQueue.try_pop(val)) {
          std::cout << clock.getTicks() << ' ' << val << std::endl;
        }
      }
      // check for state requests
      for (auto & [player, playerOutQueue]: outQueues) {
        std::promise<TacticalGameState> promise;
        if (playerOutQueue.try_pop(promise)) {
          promise.set_value(TacticalGameState{.ticks = clock.getTicks()});
          std::cout << clock.getTicks() << ' ' << static_cast<typename Player::player_id_t> (player) << std::endl;
        }
      }

      update(elapsed);
    }
  }

  void operator()() {
    run();
  }

  TacticalGame() : clock{1000 / 100} {}
//  TacticalGame(TacticalGame &&) = default;
//
  TacticalGame(const TacticalGame &) = delete;
//
//
  TacticalGame operator=(const TacticalGame &) = delete;
//
};


#endif //COROUTINES_GAME_HPP
