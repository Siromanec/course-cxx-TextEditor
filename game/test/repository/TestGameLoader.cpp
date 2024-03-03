//
// Created by ADMIN on 03-Mar-24.
//

#include <gtest/gtest.h>
#include "repository/GameLoader.hpp"

TEST(TestGameLoader, UnitLoader) {
  /*todo create directory by hand (with this code)*/
  std::filesystem::path path = "./resource/factions/france/units/infantry.json";
  GameLoader loader{{}};
  auto unit = loader.loadUnitData(path);
//  auto &factionsRepo = game.getFactions();
//  auto &factions = factionsRepo.getAll();
//  auto &faction = factions.at(0);
//  auto &factionName = faction.getName();
//  auto &unitRepo = faction.getUnitData();
//  auto &units = unitRepo.getAll();
//  std::cout << units.size() << std::endl;
//  auto &unit = units.at(0);
//
//
//  std::cout << "faction name: " << factionName << std::endl;
  std::cout << "Parsed UnitData:" << std::endl;
  std::cout << "Name: " << unit.name << std::endl;
  std::cout << "Mass: " << unit.mass << " kg" << std::endl;
  std::cout << "Speed: " << unit.speed << " m/s" << std::endl;
  std::cout << "Unit Radius: " << unit.unitRadius << " m" << std::endl;
  std::cout << "Attack Radius: " << unit.attackRadius << " m" << std::endl;
  std::cout << "Cost: " << unit.cost << std::endl;
  std::cout << "Soldier Count: " << unit.soldierCount << std::endl;
  std::cout << "Hit Points: " << unit.hitPoints << std::endl;
  std::cout << "Attack: " << unit.attack << std::endl;
  std::cout << "Defence: " << unit.defence << std::endl;



}

TEST(TestGameLoader, UnitRepoLoader) {
  /*todo create directory by hand (with this code)*/
  std::filesystem::path path = "./resource/factions/france/units/";
  GameLoader loader{{}};
  auto unitRepo = loader.loadUnitRepository(path);
  std::cout << "unit count: " << unitRepo.size() << std::endl;
}

TEST(TestGameLoader, FactionLoader) {
  /*todo create directory by hand (with this code)*/
  std::filesystem::path path = "./resource/factions/france/";
  GameLoader loader{{}};
  auto faction = loader.loadFaction(path);
//  auto & units = unitRepo.getAll();
  std::cout << "faction name" << faction.getName() << std::endl;
}

TEST(TestGameLoader, FactionRepoLoader) {
  /*todo create directory by hand (with this code)*/
  std::filesystem::path path = "./resource/factions/";
  GameLoader loader{{}};
  auto factionRepo = loader.loadFactionRepository(path);
  std::cout << "faction count" << factionRepo.size() << std::endl;
}

TEST(TestGameLoader, GameLoader) {
  /*todo create directory by hand (with this code)*/
  std::filesystem::path path = "./resource";
  GameLoader loader{path};
  auto game = loader.loadGame();
  std::cout << "faction count" << game.getFactions().size() << std::endl;
}


TEST(TestGameLoader, UnitLoaderNoSuchDir) {
  std::filesystem::path path{};
  GameLoader loader{{}};
  EXPECT_THROW(loader.loadUnitData(path), simdjson::simdjson_error);

}

TEST(TestGameLoader, UnitRepoLoaderNoSuchDir) {
  std::filesystem::path path{};
  GameLoader loader{{}};
  EXPECT_THROW(loader.loadUnitRepository(path), std::filesystem::filesystem_error);
}

TEST(TestGameLoader, FactionLoaderNoSuchDir) {
  std::filesystem::path path{};
  GameLoader loader{{}};
  EXPECT_THROW(loader.loadFaction(path), std::filesystem::filesystem_error);

}

TEST(TestGameLoader, FactionRepoLoaderNoSuchDir) {
  std::filesystem::path path{};
  GameLoader loader{{}};
  EXPECT_THROW(loader.loadFactionRepository(path), std::filesystem::filesystem_error);
}

TEST(TestGameLoader, GameLoaderNoSuchDir) {
  std::filesystem::path path{};
  GameLoader loader{path};
  EXPECT_THROW(loader.loadGame(), std::filesystem::filesystem_error);
}

