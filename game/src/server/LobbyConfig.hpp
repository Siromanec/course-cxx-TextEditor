//
// Created by ADMIN on 17-May-24.
//

#ifndef MOTION_LOBBYCONFIG_HPP
#define MOTION_LOBBYCONFIG_HPP

#include <cstdint>
#include <unordered_map>
#include "serialization.hpp"



enum WEATHER: uint8_t {
  CLEAR = 0X0,
  RAIN = 0x1,
  WIND = 0x2,
};
enum TIME: uint8_t {
  DAY = 0X0,
  NIGHT = 0x1,
  DUSK = 0x2,
};
enum MAP: uint8_t {
  PLAIN = 0x0,
};

class LobbyConfig: public serializable {
public:
  typedef uint8_t team_id_t;
  TIME time{};
  MAP map{};
  WEATHER weather{};
  uint8_t max_players{};
  uint8_t max_units{};
  uint32_t budget{};
//  std::unordered_map<team_id_t, uint8_t> ai_count;
  byte_istream & deserialize(byte_istream &istream) final {
    istream >> time;
    istream >> map;
    istream >> weather;
    istream >> max_players;
    istream >> max_units;
    istream >> budget;

//    uint8_t ai_count_size;
//    istream >> ai_count_size;
//    for (uint8_t i = 0; i < ai_count_size; ++i) {
//      team_id_t team_id;
//      uint8_t count;
//      istream >> team_id;
//      istream >> count;
//      ai_count[team_id] = count;
//    }
    return istream;

  }
  byte_ostream &serialize (byte_ostream &ostream) const final{
    ostream << time;
    ostream << map;
    ostream << weather;
    ostream << max_players;
    ostream << max_units;
    ostream << budget;
//    ostream << ai_count.size();
//    for (const auto &[team_id, count]: ai_count) {
//      ostream << team_id;
//      ostream << count;
//    }
    return ostream;
  }
};


#endif //MOTION_LOBBYCONFIG_HPP
