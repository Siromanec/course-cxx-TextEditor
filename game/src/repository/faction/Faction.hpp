//
// Created by ADMIN on 03-Mar-24.
//

#ifndef MOTION_FACTION_HPP
#define MOTION_FACTION_HPP


#include <string>
#include "repository/faction/unit/UnitData.hpp"
#include "repository/Repository.hpp"


class Faction {
  /* is derived from Loader*/
  std::string name;
  std::vector<UnitData> unitRepository;

public:
  Faction(std::string &&name, std::vector<UnitData> &&unitRepository) : name{std::move(name)}, unitRepository{std::move(unitRepository)} {}

  [[nodiscard]] const std::vector<UnitData> &getUnitData() const {
    return unitRepository;
  }

  [[nodiscard]] const std::string &getName() const {
    return name;
  }
};


#endif //MOTION_FACTION_HPP
