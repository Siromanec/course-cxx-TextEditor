//
// Created by ADMIN on 19-May-24.
//

#include "Unit.hpp"
constexpr UnitStats PeasantStats{80 / 2, 10 * 2, 2.4, 0.8, 0.2, 3.0 / 3};
constexpr UnitStats InfantryStats{100 / 2, 20 * 2, 2.0, 1.0, 0.2, 3.0 /3};
constexpr UnitStats ArmouredInfantryStats{200 /2, 20 * 2, 1.4, 0.5, 0.3, 6.0 /3};
constexpr UnitStats ShockInfantryStats{100 / 2, 50 * 2, 1.8, 2.0, 0.2, 10.0 / 3};
constexpr UnitStats CavalryStats{100 / 2, 80, 10.0 * 3, 2.2, 0.5, 1.0};

const UnitStats &get_unit_stats(UNIT_TEMPLATE unit_template) {
  switch (unit_template) {
    case PEASANTS:
      return PeasantStats;
    case INFANTRY:
      return InfantryStats;
    case ARMOURED_INFANTRY:
      return ArmouredInfantryStats;
    case SHOCK_INFANTRY:
      return ShockInfantryStats;
    case CAVALRY:
      return CavalryStats;
  }
}
