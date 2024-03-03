//
// Created by ADMIN on 03-Mar-24.
//

#ifndef MOTION_GAME_HPP
#define MOTION_GAME_HPP


#include "repository/faction/Faction.hpp"

class Game {
  std::vector<Faction> factions;
public:
  explicit Game(std::vector<Faction> &&factions):factions{factions} {
  }

   [[nodiscard]] const std::vector<Faction>& getFactions() const {
    return factions;
  }

};


#endif //MOTION_GAME_HPP
