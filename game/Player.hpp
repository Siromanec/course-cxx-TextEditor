//
// Created by ADMIN on 28-Feb-24.
//

#ifndef MOTION_PLAYER_HPP
#define MOTION_PLAYER_HPP


#include <cstddef>

struct Player {
  size_t id;
public:

  bool operator<(const Player & other) const {
    return id < other.id;
  }
  bool operator==(const Player & other) const {
    return id == other.id;
  }
};

class PlayerHasher {
public:
  size_t operator() (const Player & player) const{
    return player.id;
  }
};


#endif //MOTION_PLAYER_HPP
