//
// Created by ADMIN on 28-Feb-24.
//

#ifndef MOTION_TACTICALGAMEREQUEST_HPP
#define MOTION_TACTICALGAMEREQUEST_HPP


#include <cstddef>

enum RequestType {
  MOVE
};
struct RequestHeader {
  RequestType type;
  size_t size;
};
struct UnitAttackRequest {
  int yourUnitID;
  int enemyUnitID;
};

struct UnitMoveRequest {
  int yourUnitID;
  double position[2];
};
struct UnitStopRequest {
  int yourUnitID;
};

struct GetStateRequest {
};

#endif //MOTION_TACTICALGAMEREQUEST_HPP
