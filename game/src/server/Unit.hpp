//
// Created by ADMIN on 19-May-24.
//

#ifndef MOTION_UNIT_HPP
#define MOTION_UNIT_HPP


#include <cstdint>

struct UnitStats {
  typedef int health_t;
  typedef int attack_t;
  typedef float speed_t;
  typedef float range_t;
  health_t health;
  attack_t attack;
  speed_t speed;
  range_t range;
};

enum UNIT_TEMPLATE : uint8_t {
  PEASANTS = 0x0, // tha meat
  INFANTRY = 0x1, // average stats
  ARMOURED_INFANTRY = 0x2, // slow, ok damage, heavily armoured, small range
  SHOCK_INFANTRY = 0x3, // slow and high damage, low armor, big range
  CAVALRY = 0x4, // like infantry but faster and higher damage due to the speed
  // this should be migrated to some config file but i cannot be bothered rn
  // balance is a future issue
};

UnitStats PeasantStats{80, 10, 2.4, 0.8};
UnitStats InfantryStats{100, 20, 2.0, 1.0};
UnitStats ArmouredInfantryStats{200, 20, 1.4, 0.5};
UnitStats ShockInfantryStats{100, 50, 1.8, 2.0};
UnitStats CavalryStats{100, 20, 5.0, 1.8};


const auto & get_unit_stats(UNIT_TEMPLATE unit_template) {
  switch (unit_template) {
    case PEASANTS:
      return PeasantStats;
    case INFANTRY:
      return InfantryStats;
    case ARMOURED_INFANTRY:
      return ArmouredInfantryStats;
    case SHOCK_INFANTRY:
      return ShockInfantryStats;
    case CAVALRY:
      return CavalryStats;
  }


}


#endif //MOTION_UNIT_HPP
