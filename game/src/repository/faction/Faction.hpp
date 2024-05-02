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

  template<typename U, typename V>
  Faction(U &&name, V &&unitRepository) : name{std::forward<U>(name)}, unitRepository{std::forward<V>(unitRepository)} {}

  [[nodiscard]] const std::vector<UnitData> &getUnitData() const {
    return unitRepository;
  }

  [[nodiscard]] const std::string &getName() const {
    return name;
  }
};


#endif //MOTION_FACTION_HPP
