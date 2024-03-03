//
// Created by ADMIN on 03-Mar-24.
//

#ifndef MOTION_GAMELOADER_HPP
#define MOTION_GAMELOADER_HPP


#include <filesystem>
#include <memory>
#include <simdjson.h>

#include "Game.hpp"
#include "repository/faction/Faction.hpp"

class GameLoader {
  const std::filesystem::path & rootDir;
  const std::string_view factionRepositoryResourceDirName = "factions";
  const std::string_view unitRepositoryResourceDirName = "units";

public:
  explicit GameLoader(const std::filesystem::path & rootDir): rootDir{rootDir} {}

  Game loadGame() {
    return Game{loadFactionRepository(rootDir / factionRepositoryResourceDirName)};
  };

  std::vector<Faction> loadFactionRepository(const std::filesystem::path &pathToResource) {
    std::vector<Faction> repository;
    std::filesystem::directory_iterator iterator{pathToResource};
    for (auto &i: iterator) {
      repository.emplace_back(loadFaction(i));
    }

    return repository;

  }

  Faction loadFaction(const std::filesystem::path &pathToResource) {
    return {pathToResource.parent_path().filename().string(), loadUnitRepository(pathToResource / unitRepositoryResourceDirName)};


  }

  std::vector<UnitData> loadUnitRepository(const std::filesystem::path &pathToResource) {
    std::vector<UnitData> repository;
    std::filesystem::directory_iterator iterator{pathToResource};
    for (auto &i: iterator) {
      repository.emplace_back(loadUnitData(i));
    }

    return repository;
  }

  UnitData loadUnitData(const std::filesystem::path &pathToResource) {
//    simdjson::dom::parser parser;
    simdjson::ondemand::parser parser;

    simdjson::padded_string json = simdjson::padded_string::load(pathToResource.string());
//    simdjson::dom::parser parser;
//    simdjson::dom::element root;
    simdjson::error_code error;
    simdjson::ondemand::document root = parser.iterate(json);

//    if (!parser.parse(json).get(root)) {
//      std::cerr << "Error parsing JSON: " << error << std::endl;
//      return {};
//    }

    try {
      UnitData unitData {
          pathToResource.filename().stem().string(),
          static_cast<float>(root["mass"].get_double()),
          static_cast<float>(root["speed"].get_double()),
          static_cast<float>(root["unitRadius"].get_double()),
          static_cast<float>(root["attackRadius"].get_double()),
          static_cast<unsigned>(root["cost"].get_uint64()),
          static_cast<unsigned>(root["soldierCount"].get_uint64()),
          static_cast<unsigned>(root["hitPoints"].get_uint64()),
          static_cast<unsigned>(root["attack"].get_uint64()),
          static_cast<unsigned>(root["defence"].get_uint64())
      };

//      std::cout << "Parsed UnitData:" << std::endl;
//      std::cout << "Mass: " << unitData.mass << " kg" << std::endl;
//      std::cout << "Velocity Magnitude: " << unitData.speed << " m/s" << std::endl;
//      std::cout << "Unit Radius: " << unitData.unitRadius << " m" << std::endl;
//      std::cout << "Attack Radius: " << unitData.attackRadius << " m" << std::endl;
//      std::cout << "Cost: " << unitData.cost << std::endl;
//      std::cout << "Soldier Count: " << unitData.soldierCount << std::endl;
//      std::cout << "Hit Points: " << unitData.hitPoints << std::endl;
//      std::cout << "Attack: " << unitData.attack << std::endl;
//      std::cout << "Defence: " << unitData.defence << std::endl;
      return unitData;
    } catch (simdjson::simdjson_error &e) {
      std::cerr << pathToResource << std::endl;
      std::cerr << "Error accessing JSON values: " << e.what() << std::endl;
//      return 1;
      return {};
    }

  }
};


#endif //MOTION_GAMELOADER_HPP
