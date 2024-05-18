//
// Created by ADMIN on 18-May-24.
//

#ifndef MOTION_BATTLEBUILDER_HPP
#define MOTION_BATTLEBUILDER_HPP


#include <optional>
#include <numeric>
#include "LobbyConfig.hpp"
//https://programmingpraxis.com/2012/03/09/sparse-sets/
template <typename T>
class sparse_set {
  typedef std::size_t index_t;
  typedef T value_t;
  typedef std::vector<value_t> data_container_t;
  typedef std::vector<index_t> dense_container_t;
  typedef std::vector<index_t> sparse_container_t;
  typedef data_container_t::iterator iterator;
  typedef data_container_t::const_iterator const_iterator;
  data_container_t data;
  dense_container_t dense;
  sparse_container_t sparse;
  const std::size_t universe_size;
public:
  explicit sparse_set(std::size_t universe_size) :
  universe_size(universe_size),
  sparse(universe_size, std::numeric_limits<std::size_t>::max()) {}

  constexpr
  void insert(index_t i, value_t && value) {
    dense.emplace_back(i);
    data.emplace_back(std::forward(value));
    sparse[i] = dense.size() - 1;
  }

  constexpr
  void erase(index_t i) {
    auto last = dense.back();
    dense[sparse[i]] = last;
    sparse[last] = sparse[i];
    dense.pop_back();
    data.pop_back();
  }
  // not implementing contains method because it would lead to bad design (seductive if statements)
  constexpr
  T &operator[](index_t i) {
    return data[sparse[i]];
  }
  constexpr
  const T &operator[](index_t i) const {
    return data[sparse[i]];
  }

  constexpr
  iterator begin() {
    return data.begin();
  }
  constexpr
  iterator end() {
    return data.end();
  }
  [[nodiscard]] constexpr
  const_iterator begin() const {
    return data.begin();
  }
  [[nodiscard]] constexpr
  const_iterator end() const {
    return data.end();
  }
};

struct LocationComponent {
  typedef float x_t;
  typedef float y_t;
  x_t x;
  y_t y;
};

struct MovementComponent {
  typedef float speed_t;
  typedef float direction_t;
  speed_t speed;
  direction_t direction;
};

struct HealthComponent {
  typedef int health_t;
  health_t health;
};

struct AttackComponent {
  typedef int attack_t;
  attack_t attack;
};

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
  typedef uint32_t UnitId;

//  sparse_set<HealthComponent> health_component;
//  std::vector<std::optional<UnitId>> target_component;
    sparse_set<LocationComponent> location_component;
    sparse_set<MovementComponent> movement_component;
//  std::vector<UnitStats::attack_t> health_component;
  void update_damage() {

  }
  void update() {
//    for (int i = 0; i < health_component.size(); i++) {
//      health_component[i] -= 1;
//    }

  }


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
