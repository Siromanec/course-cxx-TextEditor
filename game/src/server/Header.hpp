//
// Created by ADMIN on 17-May-24.
//

#ifndef MOTION_HEADER_HPP
#define MOTION_HEADER_HPP


#include <cstdint>
#include <bitset>
#include "serialization.hpp"

constexpr std::uint32_t MAX_ACKS = 64;

using Sequence = std::uint32_t;
using Ack = std::uint32_t;
using Acks = std::bitset<MAX_ACKS>;


struct Header : public serializable {
  // local to the sender
  Sequence sequence{};
  // remote to the sender
  Ack ack{};
  Acks acks{};

  constexpr Header() noexcept = default;

  constexpr Header(const Header &) noexcept = default;

  constexpr Header(Header &&) noexcept = default;

  constexpr Header(Sequence sequence, Ack ack, Acks acks) noexcept:
      sequence{sequence}, ack{ack}, acks{acks} {}



  byte_ostream &serialize(byte_ostream &o) const final {
    o << sequence << ack << acks.to_ullong();
    return o;
  }

  byte_istream &deserialize(byte_istream &o) final {
    std::uint64_t acks_;
    o >> sequence >> ack >> acks_;
    acks = acks_;
    return o;
  }

};


struct ConnectionInfo {
  Sequence localSequence{};
  Sequence remoteSequence{};

  Ack lastDeliveredPacket{};

  Acks deliveredAcks{};
  Acks receivedPackets{};

  void updateWithRemote(const Header &header) {
    updateDeliveredAcks(header.ack, header.acks);
    updateReceivedPackets(header.sequence);
  }

  Header getNextHeader() {
    return {++localSequence, remoteSequence, receivedPackets};
  }

  void updateDeliveredAcks(Ack ack, Acks acks) {
    if (ack > lastDeliveredPacket) {
      std::swap(ack, lastDeliveredPacket);
      std::swap(acks, deliveredAcks);
    } else if (ack == lastDeliveredPacket) {
      deliveredAcks |= acks;
      return;
    }
    auto diff = (lastDeliveredPacket - ack) * (ack != 0);
    /* shift acks to match local and set bit to received*/
    deliveredAcks |= acks << (diff) | Acks{(1ul << (diff - 1)) * (ack != 0)};
  }

  void updateReceivedPackets(Sequence s) {
    auto diff = std::abs((long long) s - remoteSequence);
    if (s > remoteSequence) {
      receivedPackets <<= diff;
    }
    receivedPackets |= (0b1 << (diff - 1)) * (diff != 0);
    remoteSequence = std::max(remoteSequence, s);
  }
};


#endif //MOTION_HEADER_HPP
