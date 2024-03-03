cmake_minimum_required(VERSION 3.22)

#! Check every comment after the "#!"

#! CHANGE YOUR PROJECT NAME
#  It is used as your project's main executable name. 
set(PROJECT_NAME my-total-war) # my notion
project(${PROJECT_NAME} C CXX) # project(${PROJECT_NAME} C CXX ASM)

set(CMAKE_CXX_STANDARD 20)

##########################################################
# User configurable options of the template
##########################################################
# Note: symbols like WARNINGS_AS_ERRORS in configuration are intentionally variables
# and not CMake options --using options creates too many problems for students.

#! It is a good practice to set "WARNINGS_AS_ERRORS" ON,
#  but sometimes it creates too much trouble, so default is OFF.
set(WARNINGS_AS_ERRORS OFF)

#! Always use PVS Studio while developing.
set(ENABLE_PVS_STUDIO OFF)

#! Select appropriate sanitizers.
#  Definitely enable sanitizers while developing.
#  Disable it for the production builds and before submitting for grading.
#  Only one of the Memory (MSAN), Address (ASAN), or Thread (TSan)
#  sanitizers is applicable at the time -- the first defined.

#! UndefinedBehaviorSanitizer (UBSan).
#  Info: https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
set(ENABLE_UBSan OFF)
#! AddressSanitizer -- detects use after free or after scope exit,
#  memory overflows and leaks.
#  Info: https://github.com/google/sanitizers/wiki/AddressSanitizer
set(ENABLE_ASAN OFF)
#! ThreadSanitizer -- detects data races.
#  Info: https://clang.llvm.org/docs/ThreadSanitizer.html
set(ENABLE_TSan OFF)
#! MemorySanitizer -- detects uninitialized memory reads
#  Info: https://github.com/google/sanitizers/wiki/MemorySanitizer
set(ENABLE_MSAN OFF)

#! Be default -- build release version if not specified otherwise.
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif ()

# Warnings as errors should be imported here -- do not move this line
include(cmake/CompilerWarnings.cmake)

#include(FetchContent)
#FetchContent_Declare(
#        googletest
#        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
#)
## For Windows: Prevent overriding the parent project's compiler/linker settings
#set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
#FetchContent_MakeAvailable(googletest)
###########################################################
## Project files, packages, libraries and so on
###########################################################
#
##! Project main executable source compilation
#set(GAME_INCLUDE
#        game/include/simdjson.h
#        game/include/simdjson.cpp)
#
#
#add_library(game
#        ${GAME_INCLUDE}
#        game/src/TacticalGame.hpp
#        game/src/clock/GameClock.hpp
#        game/src/clock/GameClock.cpp
#        game/include/ThreadQueue.h
#        game/src/TacticalGameState.cpp
#        game/src/TacticalGameState.hpp
#        game/src/request/TacticalGameRequest.cpp
#        game/src/request/TacticalGameRequest.hpp
#        game/src/GameFacade.cpp
#        game/src/GameFacade.hpp
#        game/src/Player.cpp
#        game/src/Player.hpp
#        game/src/map/GameMap.cpp
#        game/src/map/GameMap.hpp
#        game/src/map/PhysicalMap.cpp
#        game/src/map/PhysicalMap.hpp
#        game/src/map/Path.cpp
#        game/src/map/Path.hpp
#        game/src/map/Graph.cpp
#        game/src/map/Graph.hpp
#        game/src/repository/faction/unit/TacticalUnit.cpp
#        game/src/repository/faction/unit/TacticalUnit.hpp
#        game/src/repository/faction/unit/UnitData.hpp
#        game/src/repository/faction/unit/UnitStatsFactory.cpp
#        game/src/repository/faction/unit/UnitStatsFactory.hpp
#        game/src/GameServer.cpp
#        game/src/GameServer.hpp
#        game/src/GameClient.cpp
#        game/src/GameClient.hpp
#        game/src/Game.cpp
#        game/src/Game.hpp
#        game/src/repository/faction/Faction.cpp
#        game/src/repository/faction/Faction.hpp
#        game/src/repository/Repository.hpp
#        game/src/repository/GameLoader.cpp
#        game/src/repository/GameLoader.hpp)
##set_target_properties(game PROPERTIES PUBLIC_HEADER game/src/TacticalGame.hpp)
#target_include_directories(game PRIVATE game/src)
#target_include_directories(game PRIVATE game/include)
#
#find_package(Boost REQUIRED COMPONENTS graph)
#target_include_directories(game PRIVATE ${Boost_INCLUDE_DIR})
#target_link_libraries(game Boost::graph)
#
#find_package(glm REQUIRED)
#target_link_libraries(game glm::glm)
#
#
#
#
#
#set(TEST_FILES test/repository/TestGameLoader.cpp)
#add_executable(Tests ${TEST_FILES})
##target_include_directories(testAll PRIVATE game/src)
#target_include_directories(Tests PRIVATE test/)
#target_link_libraries(
#        Tests
#        GTest::gtest_main
#        game
#)

#include(game/CMakeLists.txt)
add_subdirectory(game)

add_executable(${PROJECT_NAME} main.cpp)

target_link_libraries(${PROJECT_NAME} game)

#! Put path to your project headers
#target_include_directories(${PROJECT_NAME} PRIVATE options_parser)
target_link_libraries(${PROJECT_NAME} pthread)
#! Add external packages
# options_parser requires boost::program_options library

#target_link_libraries(${PROJECT_NAME} Boost::program_options Boost::system)

#find_package(Qt5 REQUIRED COMPONENTS Widgets)
#target_link_libraries(${PROJECT_NAME} PRIVATE )

#find_package(OpenCV REQUIRED)
#target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} Qt5::Widgets)
##########################################################
# Fixed CMakeLists.txt part
##########################################################

INSTALL(PROGRAMS
        $<TARGET_FILE:${PROJECT_NAME}> # ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}
        DESTINATION bin)

# Define ALL_TARGETS variable to use in PVS and Sanitizers
set(ALL_TARGETS ${PROJECT_NAME})

# Include CMake setup
include(cmake/main-config.cmake)
