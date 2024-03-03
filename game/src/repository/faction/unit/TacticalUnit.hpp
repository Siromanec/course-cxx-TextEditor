//
// Created by ADMIN on 02-Mar-24.
//

#ifndef MOTION_TACTICALUNIT_HPP
#define MOTION_TACTICALUNIT_HPP

#include <glm/vec2.hpp>
#include "clock/GameClock.hpp"
#include "UnitData.hpp"

enum UnitState {
  IDLE,
  MOVING_TO_POINT,
  MOVING_TO_ENEMY,
  FIGHTING
};
class TacticalUnit {
  /* enforce dependency injection at compile time
   * force user to use unit stats factory
   * */
  const UnitData & stats;
  glm::vec2 direction;
public:
  TacticalUnit(const UnitData & stats, const GameClock & clock): stats{stats} {

  }




};


#endif //MOTION_TACTICALUNIT_HPP
