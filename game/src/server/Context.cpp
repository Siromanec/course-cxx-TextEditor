//
// Created by ADMIN on 02-May-24.
//

#include "Context.hpp"

void ContextManager::parse(byte_istream &command, byte_ostream &response, Player player) {
  MANAGER_TYPE type;
  command >> type;
  switch (type) {
    case LOBBY_MANAGER: {
      return lobbyManager->parse(command, response, player);
    }
    case ARMY_CREATION_MANAGER: {
      break;
    }
    case DEPLOYMENT_MANAGER: {
      break;
    }
    case BATTLE_MANAGER: {
      break;
    }
    default: {
      throw std::runtime_error("unknown context!");
    }
  }
}
