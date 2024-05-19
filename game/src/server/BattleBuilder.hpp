//
// Created by ADMIN on 18-May-24.
//

#ifndef MOTION_BATTLEBUILDER_HPP
#define MOTION_BATTLEBUILDER_HPP


#include <optional>
#include <numeric>
#include <glm/vec2.hpp>
#include <glm/geometric.hpp>
#include "LobbyConfig.hpp"
#include "EntityComponentSystem.hpp"
#include "Unit.hpp"


struct LocationComponent {
  typedef glm::vec2 location_t;
  location_t location;
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

struct DirectionComponent {
  typedef glm::vec2 direction_t;
  direction_t direction;
};

struct SpeedComponent {
  typedef float speed_t;
  speed_t speed;
};

//void use_group(){
//  ComponentGroup<HealthComponent, LocationComponent, MovementComponent> group(100);
//  group.get<HealthComponent>().insert(0, HealthComponent{100});
//  group.get<LocationComponent>().insert(0, LocationComponent{0, 0});
//  group.get<MovementComponent>().insert(0, MovementComponent{1, 0});
//  group.get<HealthComponent>().insert(1, HealthComponent{100});
//  group.get<LocationComponent>().insert(1, LocationComponent{0, 0});
//  group.get<MovementComponent>().insert(1, MovementComponent{1, 0});
//  group.get<HealthComponent>().insert(2, HealthComponent{100});
//  group.get<LocationComponent>().insert(2, LocationComponent{0, 0});
//  group.get<MovementComponent>().insert(2, MovementComponent{1, 0});
//  group.erase<HealthComponent>(1);
//}

struct DestinationComponent {
  typedef glm::vec2 destination_t;
  destination_t destination;
};
struct UnitStatsComponent {
  UNIT_TEMPLATE unit_template;
  [[nodiscard]] const UnitStats& get_stats() const {
    return get_unit_stats(unit_template);
  }
};
class UnitComponentSystem {
  ComponentGroup<LocationComponent, DirectionComponent, DestinationComponent, SpeedComponent> movementComponentGroup;
  sparse_set<HealthComponent> healthComponent;
  sparse_set<AttackComponent> attackComponent;
  sparse_set<UnitStatsComponent> unitStatsComponent;
  typedef uint32_t UnitId;


  void set_destination(UnitId id, glm::vec2 destination) {
    auto direction = glm::normalize(destination - movementComponentGroup.get<LocationComponent>()[id].location);

    const auto& stats = unitStatsComponent[id].get_stats();

    movementComponentGroup.insert(id, DestinationComponent{destination});
    movementComponentGroup.insert(id, DirectionComponent{direction});
    movementComponentGroup.insert(id, SpeedComponent{stats.speed}); // no acceleration for now


  }
  void update_movement(float delta_time) {
    auto speed_begin = movementComponentGroup.get<SpeedComponent>().begin();
    auto speed_end = movementComponentGroup.get<SpeedComponent>().end();
    auto destination_begin = movementComponentGroup.get<DestinationComponent>().begin();
    auto direction_begin = movementComponentGroup.get<DirectionComponent>().begin();
    auto location_begin = movementComponentGroup.get<LocationComponent>().begin();
    std::vector<UnitId> to_stop;
    for(std::size_t i = 0; speed_begin != speed_end; ++i, ++speed_begin, ++direction_begin, location_begin, destination_begin)
    {
      // would be nice to stop the motion when close to destination
      auto distance_to_dest = glm::distance(location_begin->location, destination_begin->destination);
      auto distance_to_move = speed_begin->speed * delta_time;
      if(distance_to_move > distance_to_dest){
        location_begin->location = destination_begin->destination;
        to_stop.push_back(movementComponentGroup.get<SpeedComponent>().get_dense()[i]);
        continue;
      }
      location_begin->location += distance_to_move * direction_begin->direction;
    }
    for(auto id : to_stop){
      movementComponentGroup.erase<SpeedComponent>(id);
      movementComponentGroup.erase<DirectionComponent>(id);
      movementComponentGroup.erase<DestinationComponent>(id);
    }
  }
  void update(float delta_time) {
    update_movement(delta_time);
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
