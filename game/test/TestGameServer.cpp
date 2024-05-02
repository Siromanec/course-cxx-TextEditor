//
// Created by ADMIN on 09-Mar-24.
//
#include "TestGameServer.hpp"
#include "server/GameServer.hpp"

TEST(TestGameServer, UpdateDeliveredAcksIdempotency) {

  ConnectionInfo info{};


  auto ack = 8;
  auto acks = Acks{0b111};

  auto expected_ack = 9;
  auto expected_acks = Acks{0b1111};

  info.updateDeliveredAcks(ack, acks);

  ASSERT_EQ(info.lastDeliveredPacket, ack);
  ASSERT_EQ(info.deliveredAcks, acks);

  info.updateDeliveredAcks(ack, acks);

  ASSERT_EQ(info.lastDeliveredPacket, 8);
  ASSERT_EQ(info.deliveredAcks, Acks{0b111});

  info.updateDeliveredAcks(9, Acks{0b111});

  ASSERT_EQ(info.lastDeliveredPacket, 9);
  ASSERT_EQ(info.deliveredAcks, Acks {0b1111});

}

TEST(TestGameServer, UpdateDeliveredAcksFromFutureNone) {

  ConnectionInfo info{};


  auto ack = 8;
  auto acks = Acks{0b0};

  auto new_ack = 12;
  auto new_acks = Acks{0b111};

  auto expected_ack = 12;
  auto expected_acks = Acks{0b1111};

  info.updateDeliveredAcks(ack, acks);

  ASSERT_EQ(info.lastDeliveredPacket, ack);
  ASSERT_EQ(info.deliveredAcks, acks);

  info.updateDeliveredAcks(new_ack, new_acks);

  ASSERT_EQ(info.lastDeliveredPacket, expected_ack);
  ASSERT_EQ(info.deliveredAcks, expected_acks);

}

TEST(TestGameServer, UpdateDeliveredAcksFromFutureSome) {

  ConnectionInfo info{};


  auto ack = 8;
  auto acks = Acks{0b1};

  auto new_ack = 13;
  auto new_acks = Acks{0b111};

  auto expected_ack = 13;
  auto expected_acks = Acks{0b110111};

  info.updateDeliveredAcks(ack, acks);

  ASSERT_EQ(info.lastDeliveredPacket, ack);
  ASSERT_EQ(info.deliveredAcks, acks);

  info.updateDeliveredAcks(new_ack, new_acks);

  ASSERT_EQ(info.lastDeliveredPacket, expected_ack);
  ASSERT_EQ(info.deliveredAcks, expected_acks);

}

TEST(TestGameServer, UpdateDeliveredAcksFromPastNone) {

  ConnectionInfo info{};


  auto ack = 8;
  auto acks = Acks{0b0};

  auto new_ack = 5;
  auto new_acks = Acks{0b111};

  auto expected_ack = 8;
  auto expected_acks = Acks{0b111100};

  info.updateDeliveredAcks(ack, acks);

  ASSERT_EQ(info.lastDeliveredPacket, ack);
  ASSERT_EQ(info.deliveredAcks, acks);

  info.updateDeliveredAcks(new_ack, new_acks);

  ASSERT_EQ(info.lastDeliveredPacket, expected_ack);
  ASSERT_EQ(info.deliveredAcks, expected_acks);

}

TEST(TestGameServer, UpdateDeliveredAcksFromPastSome) {

  ConnectionInfo info{};


  auto ack = 8;
  auto acks = Acks{0b1};

  auto new_ack = 5;
  auto new_acks = Acks{0b111};

  auto expected_ack = 8;
  auto expected_acks = Acks{0b111101};

  info.updateDeliveredAcks(ack, acks);

  ASSERT_EQ(info.lastDeliveredPacket, ack);
  ASSERT_EQ(info.deliveredAcks, acks);

  info.updateDeliveredAcks(new_ack, new_acks);

  ASSERT_EQ(info.lastDeliveredPacket, expected_ack);
  ASSERT_EQ(info.deliveredAcks, expected_acks);

}

TEST(TestGameServer, UpdateReceivedPacketsFutureFromInit) {

  Sequence remoteSequence = 8;
  ConnectionInfo info{};
  info.remoteSequence = remoteSequence;

  Sequence newRemoteSeq = 10;

  auto expectedRemoteSeq = 10;
  auto expectedReceivedPackets = Acks{0b10};
  info.updateReceivedPackets(newRemoteSeq);

  ASSERT_EQ(info.remoteSequence, expectedRemoteSeq);
  ASSERT_EQ(info.receivedPackets, expectedReceivedPackets);

}

TEST(TestGameServer, UpdateReceivedPacketsPastFromInit) {

  Sequence remoteSequence = 8;
  ConnectionInfo info{};
  info.remoteSequence = remoteSequence;

  Sequence newRemoteSeq = 6;

  auto expectedRemoteSeq = 8;
  auto expectedReceivedPackets = Acks{0b10};
  info.updateReceivedPackets(newRemoteSeq);

  ASSERT_EQ(info.remoteSequence, expectedRemoteSeq);
  ASSERT_EQ(info.receivedPackets, expectedReceivedPackets);

}

TEST(TestGameServer, UpdateReceivedPacketsFutureWithHistory) {

  Sequence remoteSequence = 8;
  ConnectionInfo info{};
  info.remoteSequence = remoteSequence;

  Sequence newRemoteSeq = 10;

  auto expectedRemoteSeq = 10;
  auto expectedReceivedPackets = Acks{0b10};
  info.updateReceivedPackets(newRemoteSeq);

  ASSERT_EQ(info.remoteSequence, expectedRemoteSeq);
  ASSERT_EQ(info.receivedPackets, expectedReceivedPackets);

}

TEST(TestGameServer, UpdateReceivedPacketsPastRepeatWithHistory) {

  Sequence remoteSequence = 8;
  Acks receivedPackets = {0b0010011};
  ConnectionInfo info{};
  info.remoteSequence = remoteSequence;
  info.receivedPackets = receivedPackets;

  Sequence newRemoteSeq = 6;

  auto expectedRemoteSeq = 8;
  auto expectedReceivedPackets = Acks{0b0010011};
  info.updateReceivedPackets(newRemoteSeq);

  ASSERT_EQ(info.remoteSequence, expectedRemoteSeq);
  ASSERT_EQ(info.receivedPackets, expectedReceivedPackets);

}

TEST(TestGameServer, UpdateReceivedPacketsPastNewWithHistory) {

  Sequence remoteSequence = 8;
  Acks receivedPackets = {0b0010011};
  ConnectionInfo info{};
  info.remoteSequence = remoteSequence;
  info.receivedPackets = receivedPackets;

  Sequence newRemoteSeq = 5;

  auto expectedRemoteSeq = 8;
  auto expectedReceivedPackets = Acks{0b0010111};
  info.updateReceivedPackets(newRemoteSeq);

  ASSERT_EQ(info.remoteSequence, expectedRemoteSeq);
  ASSERT_EQ(info.receivedPackets, expectedReceivedPackets);

}

TEST(TestGameServer, UpdateReceivedPacketsFutureNewWithHistory) {

  Sequence remoteSequence = 8;
  Acks receivedPackets = {0b0010011};
  ConnectionInfo info{};
  info.remoteSequence = remoteSequence;
  info.receivedPackets = receivedPackets;

  Sequence newRemoteSeq = 10;

  auto expectedRemoteSeq = 10;
  auto expectedReceivedPackets = Acks{0b001001110};
  info.updateReceivedPackets(newRemoteSeq);

  ASSERT_EQ(info.remoteSequence, expectedRemoteSeq);
  ASSERT_EQ(info.receivedPackets, expectedReceivedPackets);

}

TEST(TestGameServer, UpdateReceivedPacketsSameWithHistory) {

  Sequence remoteSequence = 8;
  Acks receivedPackets = {0b0010011};
  ConnectionInfo info{};
  info.remoteSequence = remoteSequence;
  info.receivedPackets = receivedPackets;

  Sequence newRemoteSeq = 8;

  auto expectedRemoteSeq = 8;
  auto expectedReceivedPackets = Acks{0b0010011};
  info.updateReceivedPackets(newRemoteSeq);

  ASSERT_EQ(info.remoteSequence, expectedRemoteSeq);
  ASSERT_EQ(info.receivedPackets, expectedReceivedPackets);

}

TEST(TestGameServer, UpdateReceivedPacketsFutureLoop) {

  Sequence remoteSequence = 0;
  Acks receivedPackets = {0b0};
  size_t npackets = 33;
  ConnectionInfo info{};
  info.remoteSequence = remoteSequence;
  info.receivedPackets = receivedPackets;


  auto expectedRemoteSeq = npackets;
  auto expectedReceivedPackets = Acks{std::numeric_limits<Ack>::max()};
  for (size_t i = 0; i < npackets + 1; ++i) {
    info.updateReceivedPackets(i);
  }

  ASSERT_EQ(info.remoteSequence, expectedRemoteSeq);
  ASSERT_EQ(info.receivedPackets, expectedReceivedPackets);

}