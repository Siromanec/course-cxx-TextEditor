//
// Created by ADMIN on 18-May-24.
//

#include "BattleBuilder.hpp"

byte_istream &operator>>(byte_istream &istream, OwnerComponent &ownerComponent) {
  return istream >> ownerComponent.owner;
}

byte_ostream &operator<<(byte_ostream &ostream, const OwnerComponent &ownerComponent) {
  return ostream << ownerComponent.owner;
}
