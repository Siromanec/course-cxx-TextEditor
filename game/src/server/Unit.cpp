//
// Created by ADMIN on 19-May-24.
//

#include "Unit.hpp"
UnitStats PeasantStats{80, 10, 2.4, 0.8, 0.2, 3.0};
UnitStats InfantryStats{100, 20, 2.0, 1.0, 0.2, 3.0};
UnitStats ArmouredInfantryStats{200, 20, 1.4, 0.5, 0.3, 6.0};
UnitStats ShockInfantryStats{100, 50, 1.8, 2.0, 0.2, 10.0};
UnitStats CavalryStats{100, 20, 10.0, 1.8, 0.5, 10.0};

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
