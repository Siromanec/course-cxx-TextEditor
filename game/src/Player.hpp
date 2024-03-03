//
// Created by ADMIN on 28-Feb-24.
//

#ifndef MOTION_PLAYER_HPP
#define MOTION_PLAYER_HPP


#include <cstddef>

class Player {
  /* sometype faction */

public:
  size_t id;

  bool operator<(const Player & other) const {
    return id < other.id;
  }
  bool operator==(const Player & other) const {
    return id == other.id;
  }

  [[nodiscard]] size_t getId() const{
    return id;
  }
};

class PlayerHasher {
public:
  size_t operator() (const Player & player) const{
    return player.getId();
  }
};


#endif //MOTION_PLAYER_HPP
