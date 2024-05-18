//
// Created by ADMIN on 17-May-24.
//

#ifndef MOTION_RESPONSE_HPP
#define MOTION_RESPONSE_HPP


#include "serialization.hpp"

enum SERVER_MESSAGE : uint8_t {
  HEALTHCHECK = 0x0,
  RESPONSE = 0x1,
  MESSAGE = 0x2,
  BROADCAST = 0x3,
};

// i feel like i am reinventing http responses
namespace Response {
  enum RESPONSE_TYPE: uint8_t {
    NONE = 0x0, // or success
    DATA = 0x1,
    ERROR = 0x2,
  };

  enum ERROR_TYPE: uint8_t {
    INVALID_CONTEXT = 0x1,
    INVALID_COMMAND = 0x2,
    INVALID_ARGUMENTS = 0x3,
    INVALID_STATE = 0x4,
    INVALID_REQUEST = 0x5,
    CONFLICT = 0x6,
    NOT_FOUND = 0x7,


  };
}

#endif //MOTION_RESPONSE_HPP
