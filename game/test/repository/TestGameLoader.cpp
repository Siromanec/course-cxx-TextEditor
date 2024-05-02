//
// Created by ADMIN on 03-Mar-24.
//

#include "TestGameLoader.hpp"
#include "repository/GameLoader.hpp"

TEST(TestGameLoader, UnitLoader) {
  /*todo create directory by hand (with this code)*/
  std::filesystem::path path = "./resource/factions/france/units/infantry.json";
  GameLoader loader{{}};
  auto unit = loader.loadUnitData(path);
  ASSERT_STREQ(unit.name.c_str(), "infantry");
  ASSERT_EQ(unit.mass, 70);
  ASSERT_FLOAT_EQ(unit.speed, 1.33);
  ASSERT_FLOAT_EQ(unit.unitRadius, 0.3);
  ASSERT_FLOAT_EQ(unit.attackRadius, 0.5);
  ASSERT_EQ(unit.cost, 100);
  ASSERT_EQ(unit.soldierCount, 100);
  ASSERT_EQ(unit.hitPoints, 100);
  ASSERT_EQ(unit.attack, 20);
  ASSERT_EQ(unit.defence, 5);





}

TEST(TestGameLoader, UnitRepoLoader) {
  /*todo create directory by hand (with this code)*/
  std::filesystem::path path = "./resource/factions/france/units/";
  GameLoader loader{{}};
  auto unitRepo = loader.loadUnitRepository(path);
  ASSERT_EQ(unitRepo.size(), 1);
}

TEST(TestGameLoader, FactionLoader) {
  std::filesystem::path path = "./resource/factions/france/";
  GameLoader loader{{}};
  auto faction = loader.loadFaction(path);
  ASSERT_EQ(faction.getName(), "france");
}

TEST(TestGameLoader, FactionRepoLoader) {
  std::filesystem::path path = "./resource/factions/";
  GameLoader loader{{}};
  auto factionRepo = loader.loadFactionRepository(path);
  ASSERT_EQ(factionRepo.size(), 2);

}

TEST(TestGameLoader, GameLoader) {
  /*todo create directory by hand (with this code)*/
  std::filesystem::path path = "./resource";
  GameLoader loader{path};
  auto game = loader.loadGame();
  ASSERT_EQ(game.getFactions().size(), 2);

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

