//
// Created by ADMIN on 02-Mar-24.
//

#ifndef MOTION_UNITDATA_HPP
#define MOTION_UNITDATA_HPP

#include <string>
#include <glm/vec2.hpp>

struct UnitData {

  const std::string name;

  const float mass; // kg
  const float speed; // m/s
  const float unitRadius; // m
  const float attackRadius; // m

  const unsigned cost;
  const unsigned soldierCount;
  const unsigned hitPoints;
  const unsigned attack;
  const unsigned defence;
  /*unit height?*/
};


#endif //MOTION_UNITDATA_HPP
