//
// Created by ADMIN on 02-May-24.
//

#include "Context.hpp"

std::unique_ptr<Command> ContextManager::get_command(byte_istream &istream, Player player) {
  MANAGER_TYPE type;
  istream >> type;
  switch (type) {
    case LOBBY_MANAGER: {
      return lobbyManager->get_command(istream, player);
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
