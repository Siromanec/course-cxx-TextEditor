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
#include "Player.hpp"


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
struct OwnerComponent {
  Player owner;
};
class UnitComponentSystem: public serializable {
  ComponentGroup<LocationComponent, DirectionComponent, DestinationComponent, SpeedComponent> movementComponentGroup;
  ComponentGroup<HealthComponent, UnitStatsComponent, OwnerComponent> existanceComponentGroup;
  sparse_set<AttackComponent> attackComponent;


  void set_destination(UnitId id, glm::vec2 destination) {
    auto direction = glm::normalize(destination - movementComponentGroup.get<LocationComponent>()[id].location);

    const auto& stats = existanceComponentGroup.get<UnitStatsComponent>()[id].get_stats();

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
    for(std::size_t i = 0; speed_begin != speed_end; ++i, ++speed_begin, ++direction_begin, ++location_begin, ++destination_begin)
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
public:
  UnitComponentSystem(const std::unordered_map<Player, UnitConfig> & unitConfig, std::size_t universe_size) :
      movementComponentGroup(universe_size),
      existanceComponentGroup(universe_size),
      attackComponent(universe_size) {
//    std::size_t universe_size = std::accumulate(std::begin(unitConfig), std::end(unitConfig), 0, [](auto acc, const auto &pair) {
//      return acc + pair.second;
//    });

    UnitId id = 0;
    for(const auto &[player, config] : unitConfig){
      for(const auto &[unit_template, count] : config){
        for(std::size_t i = 0; i < count; ++i){
          movementComponentGroup.get<LocationComponent>().insert(id, LocationComponent{{static_cast<double>(i) / 4, 0}});
          movementComponentGroup.get<DirectionComponent>().insert(id, DirectionComponent{{1,0}});
          existanceComponentGroup.get<HealthComponent>().insert(id, HealthComponent{get_unit_stats(unit_template).health});
          existanceComponentGroup.get<UnitStatsComponent>().insert(id, UnitStatsComponent{unit_template});
          existanceComponentGroup.get<OwnerComponent>().insert(id, OwnerComponent{player}); //could be opitimised to use a range of ids
          ++id;
        }
      }
    }
  }
  void update(float delta_time) {
    update_movement(delta_time);
  }
  byte_istream & deserialize(byte_istream &istream) final {
    // parses the whole component
    movementComponentGroup.deserialize(istream);
    existanceComponentGroup.deserialize(istream);
    attackComponent.deserialize(istream);
    return istream;
  }
  byte_ostream &serialize(byte_ostream &ostream) const final {
    // serializes the whole component
    movementComponentGroup.serialize(ostream);
    existanceComponentGroup.serialize(ostream);
    attackComponent.serialize(ostream);
    return ostream;
  }

  void update_with_remote(byte_istream &istream) {
    // updates only dynamic components
    movementComponentGroup.get<LocationComponent>().deserialize(istream);
    movementComponentGroup.get<DirectionComponent>().deserialize(istream);
    existanceComponentGroup.get<HealthComponent>().deserialize(istream);
  }

  void send_to_remote(byte_ostream &ostream) {
    // writes only dynamic components
    movementComponentGroup.get<LocationComponent>().serialize(ostream);
    movementComponentGroup.get<DirectionComponent>().serialize(ostream);
    existanceComponentGroup.get<HealthComponent>().serialize(ostream);
  }


};
class Battle {
  TIME time;
  MAP map;
  WEATHER weather;
  UnitComponentSystem unitComponentSystem;
public:
  Battle(TIME time, MAP map, WEATHER weather, const std::unordered_map<Player, UnitConfig> &unitConfig, std::size_t universe_size) :
      time(time),
      map(map),
      weather(weather),
      unitComponentSystem(unitConfig, universe_size) {}
  void update(float delta_time) {
    unitComponentSystem.update(delta_time);
  }
  void update_with_remote(byte_istream &istream) {
    unitComponentSystem.update_with_remote(istream);
  }
  void send_to_remote(byte_ostream &ostream) {
    unitComponentSystem.send_to_remote(ostream);
  }
  byte_istream & deserialize(byte_istream &istream) {
    istream >> time;
    istream >> map;
    istream >> weather;
    unitComponentSystem.deserialize(istream);
    return istream;
  }
  byte_ostream &serialize(byte_ostream &ostream) const {
    ostream << time;
    ostream << map;
    ostream << weather;
    unitComponentSystem.serialize(ostream);
    return ostream;
  }
};
class BattleBuilder {

  TIME time{};
  MAP map{};
  WEATHER weather{};
  std::unordered_map<Player, UnitConfig> unitConfig;
  std::size_t universe_size;

  void parseLobbyConfig(const LobbyConfig &config) {
    time = config.time;
    map = config.map;
    weather = config.weather;
  }
  void parseUnitConfig(const std::unordered_map<Player, UnitConfig> &unitConfig_) {
    unitConfig = unitConfig_;
    universe_size = std::accumulate(std::begin(unitConfig), std::end(unitConfig), 0, [](auto acc, const auto &pair) {
      return std::accumulate(std::begin(pair.second), std::end(pair.second), 0, [](auto acc, const auto &pair) {
        return acc + pair.second;
      });
    });

  }
  Battle build() {
    return {time, map, weather, unitConfig, universe_size};
  }
};


#endif //MOTION_BATTLEBUILDER_HPP
