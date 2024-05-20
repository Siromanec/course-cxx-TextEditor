////
//// Created by ADMIN on 02-Mar-24.
////
//
//#define RUN_GAME_SERVER
#ifdef RUN_GAME_SERVER

#include "GameServer.hpp"
#include "BattleBuilder.hpp"

//#define BATTLE
//#ifdef SERVER
//int main() {
//  boost::asio::io_context ioContext{};
//  auto server = GameServer{ioContext};
//  auto thread = std::thread{[&ioContext] () {ioContext.run();}};
//
//
//
////  while (true) {
////    std::this_thread::sleep_for(std::chrono::milliseconds(20));
//////    sleep(1);
////    server.send_all("|" + std::to_string(i++) + "|");
////    if (i > 1000)
////      break;
////  }
//
//  thread.join();
//  return 0;
//}
//#endif
//#ifdef BATTLE
#include <SFML/Graphics.hpp>
#include <iostream>
#include <glm/vec4.hpp>

void drawGrid(sf::RenderWindow &win, int rows, int cols, float startX = 0, float startY = 0) {
  // initialize values
  int numLines = rows + cols - 2;
  sf::VertexArray grid(sf::Lines, 2 * (numLines));
//  win.setView(win.getDefaultView());
  auto size = win.getView().getSize();
  size.x *= 8;
  size.y *= 8;
  float rowH = size.y / rows;
  float colW = size.x / cols;
  // row separators
  for (int i = 0; i < rows - 1; i++) {
    int r = i + 1;
    float rowY = startY + rowH * r;
    grid[i * 2].position = {startX, rowY};
    grid[i * 2 + 1].position = {size.x, rowY};
  }
  // column separators
  for (int i = rows - 1; i < numLines; i++) {
    int c = i - rows + 2;
    float colX = startX + colW * c;
    grid[i * 2].position = {colX, startY};
    grid[i * 2 + 1].position = {colX, size.y};
  }
  // draw it
  win.draw(grid);
}

int main() {
  auto window = sf::RenderWindow{{1920u / 2, 1920u / 2}, "CMake SFML Project"};
  std::size_t framerate = 30;
  window.setFramerateLimit(framerate);
  window.setVerticalSyncEnabled(true);
  // create a view with the rectangular area of the 2D world to show
//  sf::View view1(sf::FloatRect(200.f, 200.f, 300.f, 200.f));

// create a view with its center and size
  sf::View view2(sf::Vector2f(0, 0), sf::Vector2f(16., 16.));

  sf::CircleShape shape(100);
  shape.setFillColor(sf::Color::Green);
  shape.setPosition(100, 100);
  window.setView(view2);

//  window
  std::size_t i = 0;
  std::vector<Player> players = {
      Player(),
      Player(),
      Player(),
      Player(),
  };
  std::unordered_map<Player, UnitConfig>
  unitConfig_map = {
      {players.at(0), UnitConfig{{UNIT_TEMPLATE::INFANTRY,          255},
                                 {UNIT_TEMPLATE::CAVALRY,           255},
                                 {UNIT_TEMPLATE::SHOCK_INFANTRY,    255},
                                 {UNIT_TEMPLATE::ARMOURED_INFANTRY, 255},
                                 {UNIT_TEMPLATE::PEASANTS,          255}}
                                 },
       {players.at(1), UnitConfig{{UNIT_TEMPLATE::INFANTRY,          255},
                                  {UNIT_TEMPLATE::CAVALRY,           255},
                                  {UNIT_TEMPLATE::SHOCK_INFANTRY,    255},
                                  {UNIT_TEMPLATE::ARMOURED_INFANTRY, 255},
                                  {UNIT_TEMPLATE::PEASANTS,          255}}},
                                         {players.at(2), UnitConfig{{UNIT_TEMPLATE::INFANTRY,          255},
                                  {UNIT_TEMPLATE::CAVALRY,           255},
                                  {UNIT_TEMPLATE::SHOCK_INFANTRY,    255},
                                  {UNIT_TEMPLATE::ARMOURED_INFANTRY, 255},
                                  {UNIT_TEMPLATE::PEASANTS,          255}}},
                                         {players.at(3), UnitConfig{{UNIT_TEMPLATE::INFANTRY,          255},
                                  {UNIT_TEMPLATE::CAVALRY,           255},
                                  {UNIT_TEMPLATE::SHOCK_INFANTRY,    255},
                                  {UNIT_TEMPLATE::ARMOURED_INFANTRY, 255},
                                  {UNIT_TEMPLATE::PEASANTS,          255}}},
//      {players.at(1), UnitConfig{{UNIT_TEMPLATE::INFANTRY, 10}}},
      };
      LobbyConfig lobbyConfig = LobbyConfig(TIME::DAY, MAP::PLAIN, WEATHER::CLEAR, 2, 10, 1000);
      BattleBuilder builder;
      builder.parseLobbyConfig(lobbyConfig);
      builder.parsePlayers(players);
      builder.parseUnitConfig(unitConfig_map);
      auto battle = builder.build();
      float delta_time = 1. / framerate;
      UnitId selectedUnitId = std::numeric_limits<UnitId>::max();


      sf::Vector2f mouse_pos_clicked;
      sf::Vector2f mouse_pos_released;
      bool was_released = false;
      glm::vec4 boundingBox;
      std::vector<UnitId> selectedUnits;

      while (window.isOpen()) {
        for (auto event = sf::Event{}; window.pollEvent(event);) {
          switch (event.type) {
            case sf::Event::Closed:
              window.close();
              break;
            case sf::Event::KeyPressed: {
              switch (event.key.code) {
                case sf::Keyboard::Up:
                  view2.move(0, -2.5);
                  break;
                case sf::Keyboard::Down:
                  view2.move(0, 2.5);
                  break;
                case sf::Keyboard::Left:
                  view2.move(-2.5, 0);
                  break;
                case sf::Keyboard::Right:
                  view2.move(2.5, 0);
                  break;
                default:
                  break;
              }
              default:
                break;
            }
            case sf::Event::MouseButtonPressed: {

              auto mouse_pos = sf::Mouse::getPosition(window);
              mouse_pos_clicked = window.mapPixelToCoords(mouse_pos);
              if (was_released) {

                battle.move_selected_units(selectedUnits, boundingBox, {mouse_pos_clicked.x, mouse_pos_clicked.y});
                was_released = false;
              } else {
                // draw selection box
                boundingBox = {mouse_pos_clicked.x, mouse_pos_clicked.y, mouse_pos_clicked.x, mouse_pos_clicked.y};
              }
              break;

//          bool selected = selectedUnitId != std::numeric_limits<UnitId>::max();
//          if (selected) {
//            battle.move_unit(selectedUnitId, {world_pos.x, world_pos.y});
//            std::cout << "Unit moved to: " << world_pos.x << ", " << world_pos.y << std::endl;
//            selectedUnitId = std::numeric_limits<UnitId>::max();
//            break;
//          }
//          else {
//            selectedUnitId = battle.get_unit_id_at({world_pos.x, world_pos.y});
//
//            if (selectedUnitId == std::numeric_limits<UnitId>::max())
//              break;
//            std::cout << "Unit selected: " << selectedUnitId << std::endl;
//          }
//
//          std::cout << "Mouse clicked at: " << world_pos.x << ", " << world_pos.y << std::endl;
//          break;
            }
            case sf::Event::MouseButtonReleased: {
              if (was_released)
                break;
              mouse_pos_released = window.mapPixelToCoords(sf::Mouse::getPosition(window));
              auto min_x = std::min(mouse_pos_clicked.x, mouse_pos_released.x);
              auto max_x = std::max(mouse_pos_clicked.x, mouse_pos_released.x);
              auto min_y = std::min(mouse_pos_clicked.y, mouse_pos_released.y);
              auto max_y = std::max(mouse_pos_clicked.y, mouse_pos_released.y);

              boundingBox = {min_x, min_y, max_x, max_y};
              selectedUnits = battle.select_units_in(boundingBox);

              was_released = true;
              break;
            }
            case sf::Event::MouseWheelScrolled: {
              //zoom
              if (event.mouseWheelScroll.delta > 0) {
                view2.zoom(0.9);
              } else {
                view2.zoom(1.1);
              }
              break;
            }
          }
        }


        window.clear();
        battle.update(delta_time);

        drawGrid(window, 16 * 8, 16 * 8, -16 * 8 / 2, -16 * 8 / 2);
        if (!was_released) {
          sf::RectangleShape rect;
          mouse_pos_released = window.mapPixelToCoords(sf::Mouse::getPosition(window));
          boundingBox = {mouse_pos_clicked.x, mouse_pos_clicked.y, mouse_pos_released.x, mouse_pos_released.y};
          rect.setPosition(boundingBox.x, boundingBox.y);
          rect.setSize({boundingBox.z - boundingBox.x, boundingBox.w - boundingBox.y});
          rect.setFillColor(sf::Color::Transparent);
          rect.setOutlineColor(sf::Color::Red);
          rect.setOutlineThickness(0.1);
          window.draw(rect);
        }
        battle.draw(window);
        window.setView(view2);


//    view2.move(100.f * (i % 2), 0);
        window.display();
        ++i;
      }
      return 0;
  }
//#endif
#endif // RUN_GAME_SERVER