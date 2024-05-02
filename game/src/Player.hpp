//
// Created by ADMIN on 28-Feb-24.
//

#ifndef MOTION_PLAYER_HPP
#define MOTION_PLAYER_HPP


#include <cstddef>
#include <cstdint>
#include <unordered_set>
#include <bitset>

//class Player {
//  /* sometype faction */
//
//public:
//  size_t id;
//
//  bool operator<(const Player & other) const {
//    return id < other.id;
//  }
//  bool operator==(const Player & other) const {
//    return id == other.id;
//  }
//
//  [[nodiscard]] size_t getId() const{
//    return id;
//  }
//};
//

/**
 * class for player id
 * can be used as regular int
 * autoincrements on creation*/
struct Player {
  typedef std::uint8_t player_id_t;
  static constexpr std::size_t max_players = 255;
private:

  player_id_t id;
  static inline player_id_t nextID{1};
public:
  Player(): id{nextID++} {}

  explicit
  operator player_id_t() const {return id;}

//  Player& operator=(const Player&) = default;
  Player(const Player& p) = default;


  std::size_t hash() const {return id;}
  bool operator==(const Player& other) const {return id == other.id;}
};

namespace std {
  template<>
  struct hash<Player> {
    std::size_t operator()(const Player& p) const {
      return p.hash();
    }
  };
}

//using Players = std::unordered_set<Player>;
// player at pos 0 should be invalid
using Players = std::bitset<Player::max_players + 1>;

#endif //MOTION_PLAYER_HPP
