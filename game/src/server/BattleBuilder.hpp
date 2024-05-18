//
// Created by ADMIN on 18-May-24.
//

#ifndef MOTION_BATTLEBUILDER_HPP
#define MOTION_BATTLEBUILDER_HPP


#include "LobbyConfig.hpp"

struct UnitStats {
  typedef int health_t;
  typedef int attack_t;
  typedef int defense_t;
  typedef float speed_t;
  typedef float range_t;
  health_t health;
  attack_t attack;
  defense_t defense;
  speed_t speed;
  range_t range;
};

class UnitComponentSystem {

  std::vector<UnitStats::health_t> health_component;
//  std::vector<UnitStats::attack_t> health_component;
};
class Battle {
  TIME time;
  MAP map;
  WEATHER weather;
  UnitComponentSystem unitComponentSystem;
};
class BattleBuilder {

  TIME time{};
  MAP map{};
  WEATHER weather{};

  void parseConfig(const LobbyConfig &config) {
    time = config.time;
    map = config.map;
    weather = config.weather;
  }
  void build() {}
};


#endif //MOTION_BATTLEBUILDER_HPP
