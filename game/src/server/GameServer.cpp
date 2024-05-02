////
//// Created by ADMIN on 02-Mar-24.
////
//
#ifdef RUN_GAME_SERVER
#include "GameServer.hpp"
//
//
int main() {
  boost::asio::io_context ioContext{};
  auto server = GameServer{ioContext};
//  ioContext.run();
  auto thread = std::thread{[&ioContext] () {ioContext.run();}};

//  std::size_t i = 0;
//  sleep(10);


//  while (true) {
//    std::this_thread::sleep_for(std::chrono::milliseconds(20));
////    sleep(1);
//    server.send_all("|" + std::to_string(i++) + "|");
//    if (i > 1000)
//      break;
//  }

  thread.join();
  return 0;
}
#endif // RUN_GAME_SERVER