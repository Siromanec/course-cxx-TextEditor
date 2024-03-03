//// This is a personal academic project. Dear PVS-Studio, please check it.
//// PVS-Studio Static Code Analyzer for C, C++, C#, and Java: http://www.viva64.com
//
//#include <iostream>
//
//int main(int argc, char* argv[]) {
//    std::cout << "hello text" << std::endl;
//    return 0;
//}
#if 0
#include "opencv2/opencv.hpp"
#include "opencv2/highgui/highgui.hpp"


int main(int argc, char** argv) {

  //create a gui window:
  cv::namedWindow("mr-fresh", 1);

  auto output = cv::imread("data/mr-fresh.png");

  //write text on the matrix:
  cv::putText(output,
              "Hello World :)",
              cv::Point(15,70),
              cv::FONT_HERSHEY_PLAIN,
              3,
              cv::Scalar(0,255,0),
              4);

  //display the image:
  cv::imshow("mr-fresh", output);

  //wait for the user to press any key:
  cv::waitKey(0);


  return 0;

}
#endif

#if 0
#include <QApplication>
#include <QPushButton>

int main(int argc, char **argv)
{
  QApplication app (argc, argv);

  QPushButton button;
  button.setText("My text");
  button.setToolTip("A tooltip");
  button.show();

  return app.exec();
}
#endif

#include <thread>
#include "game/src/TacticalGame.hpp"
#include "game/src/GameFacade.hpp"

int main() {
  // TODO change to some sort of facade to game
  // running update from this thread is a certified bruh moment
  auto game = GameFacade::getInstance();
  /*
   * multiplayer -> create server
   * singleplayer -> load locally
   * */
  int val = 0;
  while (true) {
    std::cin >> val;
    if (val == 0) {
      break;
    }
    game->command(val);

    auto future = game->getState();
//    future.wait();
    auto state = future.get();
    std::cout << "the tick of the game is " << state.ticks << std::endl;
  }
  return 0;
}