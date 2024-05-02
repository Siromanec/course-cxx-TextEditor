//
// Created by ADMIN on 12-Mar-24.
//

#ifndef MOTION_SETUPCOMPONENTS_HPP
#define MOTION_SETUPCOMPONENTS_HPP


#include <cstdint>
#include <vector>
#include <unordered_set>
#include "server/GameServer.hpp"
/*
 * Actor wil have an interface
 * */
using Actor = std::uint16_t;
using Team = std::unordered_set<Actor>;

class SetupComponents {

  /* 1. map
   * how to verify that two players have the same map? checksum must match or globally distributed id
   * -- whatever, just an id , mods will need their own folders
   * the point is that map ids are already somewhere
   * 2. team -- a set of players
   * 3.1 players -- available slots -- an array
   * 3.2 AIs -- an array -- possibly the same as players called actors
   * 4. budget -- how much each player has -- an array*/

  static auto createActors(std::uint8_t players, std::uint8_t AIs) {
    std::vector<Actor> actors {};
    actors.resize(players + AIs);
    return actors;
  }
  static auto createBudget(std::uint8_t players, std::uint8_t AIs, std::uint32_t budget) {
    std::vector<std::uint32_t> budgets {};
    budgets.resize(players + AIs, budget);
    return budgets;
  }
  static auto createTeams(std::uint8_t nteams) {
    std::vector<Team> teams {};
    teams.resize(nteams);
    return teams;
  }

  static auto createMap(std::uint8_t mapID) {
      /* todo:
       * 1. load map names into memory
       * 2. assign ids
       * 3. load map with mapID into memory and return it
       * */
  }
//  static auto parseSetup(const Setup& setup) {
//    return std::tuple(createActors(setup.players, setup.AIs), createBudget(setup.players, setup.AIs, setup.budget), createTeams(setup.teams));
//
//  }
};



//std::

#endif //MOTION_SETUPCOMPONENTS_HPP
