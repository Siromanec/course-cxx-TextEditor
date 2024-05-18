//
// Created by ADMIN on 02-May-24.
//

#include "Context.hpp"

void ContextManager::parse(byte_istream &command, byte_ostream &response, Player player) {
  MANAGER_TYPE type;
  command >> type;
  switch (type) {
    case LOBBY_MANAGER: {
      lobbyManager->parse(command, response, player);
      break;
    }
    case ARMY_CREATION_MANAGER: {
      armyCreationManager->parse(command, response, player);
      break;
    }
    case DEPLOYMENT_MANAGER: {
      break;
    }
    case BATTLE_MANAGER: {
      break;
    }
    default: {
      response << Response::ERROR << Response::INVALID_CONTEXT;
    }
  }
}
