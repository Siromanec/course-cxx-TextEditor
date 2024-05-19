//
// Created by ADMIN on 19-May-24.
//

#ifndef MOTION_UNIT_HPP
#define MOTION_UNIT_HPP


#include <cstdint>
#include <unordered_map>

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



const UnitStats & get_unit_stats(UNIT_TEMPLATE unit_template);
typedef uint32_t UnitId;

typedef std::uint8_t unit_count_t;
typedef std::unordered_map<UNIT_TEMPLATE, unit_count_t> UnitConfig;

#endif //MOTION_UNIT_HPP
