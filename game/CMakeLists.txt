
#! Check every comment after the "#!"

#! CHANGE YOUR PROJECT NAME
#  It is used as your project's main executable name.
set(GAME_LIB_NAME game)

##########################################################
# User configurable options of the template
##########################################################
# Note: symbols like WARNINGS_AS_ERRORS in configuration are intentionally variables
# and not CMake options --using options creates too many problems for students.

##! It is a good practice to set "WARNINGS_AS_ERRORS" ON,
##  but sometimes it creates too much trouble, so default is OFF.
#set(WARNINGS_AS_ERRORS OFF)
#
##! Always use PVS Studio while developing.
#set(ENABLE_PVS_STUDIO OFF)
#
##! Select appropriate sanitizers.
##  Definitely enable sanitizers while developing.
##  Disable it for the production builds and before submitting for grading.
##  Only one of the Memory (MSAN), Address (ASAN), or Thread (TSan)
##  sanitizers is applicable at the time -- the first defined.
#
##! UndefinedBehaviorSanitizer (UBSan).
##  Info: https://clang.llvm.org/docs/UndefinedBehaviorSanitizer.html
#set(ENABLE_UBSan OFF)
##! AddressSanitizer -- detects use after free or after scope exit,
##  memory overflows and leaks.
##  Info: https://github.com/google/sanitizers/wiki/AddressSanitizer
#set(ENABLE_ASAN OFF)
##! ThreadSanitizer -- detects data races.
##  Info: https://clang.llvm.org/docs/ThreadSanitizer.html
#set(ENABLE_TSan OFF)
##! MemorySanitizer -- detects uninitialized memory reads
##  Info: https://github.com/google/sanitizers/wiki/MemorySanitizer
#set(ENABLE_MSAN OFF)
#
##! Be default -- build release version if not specified otherwise.
#if (NOT CMAKE_BUILD_TYPE)
#    set(CMAKE_BUILD_TYPE Release)
#endif ()
#
## Warnings as errors should be imported here -- do not move this line
#include(cmake/CompilerWarnings.cmake)

include(FetchContent)
FetchContent_Declare(
        googletest
        URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
##########################################################
# Project files, packages, libraries and so on
##########################################################

#! Project main executable source compilation

message(MESSAGE "dir: ${CMAKE_CURRENT_SOURCE_DIR}")


set(GAME_INCLUDE
        include/simdjson.h
        include/simdjson.cpp
        include/ThreadQueue.h
)


add_library(${GAME_LIB_NAME} SHARED
        ${GAME_INCLUDE}
        src/TacticalGame.hpp
        src/clock/GameClock.hpp
        src/clock/GameClock.cpp
        src/TacticalGameState.cpp
        src/TacticalGameState.hpp
        src/request/TacticalGameRequest.cpp
        src/request/TacticalGameRequest.hpp
        src/GameFacade.cpp
        src/GameFacade.hpp
        src/Player.cpp
        src/Player.hpp
        src/map/GameMap.cpp
        src/map/GameMap.hpp
        src/map/PhysicalMap.cpp
        src/map/PhysicalMap.hpp
        src/map/Path.cpp
        src/map/Path.hpp
        src/map/Graph.cpp
        src/map/Graph.hpp
        src/repository/faction/unit/TacticalUnit.cpp
        src/repository/faction/unit/TacticalUnit.hpp
        src/repository/faction/unit/UnitData.hpp
        src/repository/faction/unit/UnitStatsFactory.cpp
        src/repository/faction/unit/UnitStatsFactory.hpp
        src/GameServer.cpp
        src/GameServer.hpp
        src/GameClient.cpp
        src/GameClient.hpp
        src/Game.cpp
        src/Game.hpp
        src/repository/faction/Faction.cpp
        src/repository/faction/Faction.hpp
        src/repository/Repository.hpp
        src/repository/GameLoader.cpp
        src/repository/GameLoader.hpp)
#set_target_properties(game PROPERTIES PUBLIC_HEADER game/src/TacticalGame.hpp)
target_include_directories(${GAME_LIB_NAME} PRIVATE src/)
target_include_directories(${GAME_LIB_NAME} PUBLIC include/)

find_package(Boost REQUIRED COMPONENTS graph)
find_package(glm REQUIRED)

target_include_directories(${GAME_LIB_NAME} PRIVATE ${Boost_INCLUDE_DIR})
target_link_libraries(${GAME_LIB_NAME} Boost::graph)

target_link_libraries(${GAME_LIB_NAME} glm::glm)





set(TEST_FILES test/repository/TestGameLoader.cpp)
add_executable(Tests ${TEST_FILES})
target_include_directories(Tests PRIVATE test/)
target_include_directories(Tests PRIVATE src/)

target_link_libraries(
        Tests
        GTest::gtest_main
        game
)

target_link_libraries(${GAME_LIB_NAME} pthread)
#export(TARGETS ${PROJECT_NAME})

#! Put path to your project headers
#target_include_directories(${PROJECT_NAME} PRIVATE options_parser)
#target_link_libraries(${PROJECT_NAME} pthread)
##! Add external packages
## options_parser requires boost::program_options library
#
##target_link_libraries(${PROJECT_NAME} Boost::program_options Boost::system)
#
##find_package(Qt5 REQUIRED COMPONENTS Widgets)
##target_link_libraries(${PROJECT_NAME} PRIVATE )
#
##find_package(OpenCV REQUIRED)
##target_link_libraries( ${PROJECT_NAME} ${OpenCV_LIBS} Qt5::Widgets)
###########################################################
## Fixed CMakeLists.txt part
###########################################################
#
#INSTALL(PROGRAMS
#        $<TARGET_FILE:${PROJECT_NAME}> # ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}
#        DESTINATION bin)
#
## Define ALL_TARGETS variable to use in PVS and Sanitizers
#set(ALL_TARGETS ${PROJECT_NAME})
#
## Include CMake setup
#include(cmake/main-config.cmake)
