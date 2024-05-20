//
// Created by ADMIN on 28-Feb-24.
//

#ifndef MOTION_PLAYER_HPP
#define MOTION_PLAYER_HPP


#include <cstddef>
#include <cstdint>
#include <unordered_set>
#include <bitset>
#include "serialization.hpp"

/**
 * class for player id
 * can be used as regular int
 * autoincrements on creation*/
struct Player : public serializable {
  typedef std::uint32_t player_id_t;
private:

  player_id_t id;
  static inline player_id_t nextID{0};
public:
  Player() : id{++nextID} {}

  [[nodiscard]] std::size_t hash() const { return id; }

  bool operator==(const Player &other) const { return id == other.id; }

  byte_istream &deserialize(byte_istream &istream) final {
    istream >> id;
    return istream;
  }

  byte_ostream &serialize(byte_ostream &ostream) const final {
    ostream << id;
    return ostream;
  }
  [[nodiscard]] player_id_t getID() const { return id; }
};

// it appears that virtual base classes are at least 8 bytes without any data members
// the required class size is 4 bytes
// with alignment it becomes 16 bytes
// a whole 4x increase in size -- the net communiction is not affected, but the memory usage is
// i will not change the player class now, but i will keep this in mind for the future
// aka not every class should be serializable by itself
namespace std {
  template<>
  struct hash<Player> {
    std::size_t operator()(const Player &p) const {
      return p.hash();
    }
  };
}


#endif //MOTION_PLAYER_HPP
