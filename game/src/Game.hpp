//
// Created by ADMIN on 03-Mar-24.
//

#ifndef MOTION_GAME_HPP
#define MOTION_GAME_HPP


#include "repository/faction/Faction.hpp"

enum GameState {
  MENU,
  /**
   * host selects a map,
   * number of players,
   * number of teams,
   * number of ai players,
   * defines a game budget,
   *
   * after the host opens the game to world other players can accept
   *
   * server creates new game instance from these parameters
   *
   * server should be stateless, aka it should only handle and send requests
   *
   * all game states would have a IRegistrableManger interface
   * which includes:
   * identifying sequence, that explains to server when a particular registrable class is referred
   * IRegistrableManger could be statically linked with the game server
   * It means variadic template with a sequence of IRegistrableManger classes
   * They all have to be stored somewhere and resemble a map of [sequence -> IRegistrableManger]
   * IRegistrableManger may statically store all instances of states, manage access to them from the server
   *
   * Also server should statically check if all IRegistrableManger have unique sequence
   *
   * Server-specific headers should be stripped and passed down (aks, sequence number, player id, game id, etc.)
   *
   * server can give information on the sender of request
   *
   * should server (with its interface () ) be injected into the game states? (or to be more exact, send/recv interface)
   * How would server notify the game the state running in a game loop?
   *
   * problems of network communiactions:
   * conversion of bytestreams to commands
   * deciding who handles the commands
   *
   * what alternatives are to message queue. Should input be handeled with higher priority, than output (game loop)?
   * the issue is that the commands should not modify the game state that is being computed right now
   *
   * anyway, what can those commands change? (in the context of this game (my-total-war) )
   * actions, that impact unit movement, which affects
   * actions, that toggle some unit state
   * actions, related to session managemet
   *
   * two buffers?
   * common case:
   *
   * do fill buffer b1          | is b2 ready?
   *                            |   b2 is used
   * b2 is not used?            |   do output buffer b2
   * swap(b1,b2)                |   b2 is not used
   * b2 is ready                |   b2 is not ready
   *
   * with better semantics:
   *
   *    do fill buffer b1          |
   *    acquire(b2)                |    acquire(b2)
   *    swap(b1,b2)                |    do output buffer b2
   *    release(b2)                |    release(b2)
   *
   * only one buffer must be accquired
   *
   * game state should be polled
   *
   * but this is some kind of final serialized representation
   *
   * what about the intermediate representation (used by game internally)?
   *
   * having two buffers would mean a copy on every iteration. - easy O(mn)
   *
   * the main question is whether the game loop should be stopped to handle user input or can it be modified on the go
   *
   * partial writes would be detramental to game logic
   *
   * should there be a limit on how many commands can be handled per loop? perhaps
   * should there be expiry time in queue
   * should it actually be a priority queue?
   *
   * higher priority for more recent commands that perform exclusive actions (toggle the same state for the same unit, give order to the same unit)
   * what will differ in the command? the command arguments? ids and command types must stay the same
   * there could be a no_override flag, that chains the commands - but it requires additional tracking - a unit can notify, that it finnished an action
   * or unit can store internally what command it executes - may be impractal on large scale
   * + in ecs there is no such thing as unit
   * beware of race conditions!
   *
   * chain of responsibility :3
   *
   * how a command would look in the context of ecs?
   * a value at index ijk is modified
   * another table (component) is added
   *
   * how can a command be notified that it has finished?
   *  polling (obvious)
   *  notify on detachment
   *
   * all game states from single game should have same id
   * how server resources are shared between game states? especially when they may have their own game loops
   * could os scheduler manage it?
   *
   * there should be raised an error if the server receives a zero id
   *
   * should server transform user ip to user id?
   * */
  LOBBY_CREATION,
  /**
   * here the server tries to manage joining players
   * they must tell which team they want to accept
   * host tells AIs to accept a certain team
   * they can request for team distribution
   * when they agree with the distribution they send affirmation
   * player identification may happen using usernames at first (server may store them as their hash)
   *
   * when all players have agreed, faction data and budgets are sent out to them
   * */
  PLAYER_JOINING,
  /**
   * given faction data (which includes unit data) players may select:
   * a faction;
   * units.
   * locally, they build their army and track if they have exceeded the limit.
   * when finished, they send the composed army to the server.
   * server additionally checks if the constrains are satisfied.
   * host may build army for AIs
   * when everyone has finished, they confirm it
   * players may see what their allies are selecting
   * so with every change in army composition, general representation must change
   * (aka client after every local change sends the result or difference to the server)
   *
   * at the end the game map file location is sent out to them, possibly with object that contains random alterations
   * */
  ARMY_CREATION,
  /**
   * at this point players must see what their allies are doing
   * and not see what enemies are doing
   * the same strategy of recording changes is used as in previous state
   * players once again confirm that they are ready
   * move and reshape orders are instant
   *
   * at the end the enemy location is revealed, game clock starts
   * */
  DEPLOYMENT,
  /**
   * new orders are available (attack)
   * acceptable coordinates for order are extended
   * pathfinding is enabled
   * and units run a state machine that controls the soldiers
   * and takes time to execute.
   * heavy calculations begin.
   * units activate their intrinsic state (e.g. alive soldiers)
   * simulation snapshots are sent on demand (as clients request) {or broadcasted???}
   * clients handle the simulation state and render it on their machines
   * */
  BATTLE,
  /* TODO describe what the simulation state is and what does it contain
   * simulation state is the position of all dynamic entities,
   * their velocities (since state is a snapshot of the simulation, motion information could be omitted)
   * but unit position would require also direction information
   * if animations are planned the state of the unit and time since last change of state are required
   *
   * in broader sense, (sendable) game state is associated with internal game state
   * and is just enough to render the game correctly for all unit snapshots and possible user inputs
   *
   * read / write locks for game state
   * */
};
/*
 * this->state = state.handle();
 * handle() {
 *  return *this or new state
 * }
 * what are the state-specific data?
 *
 * how to ensure atomicity of message type and state?
 * */
class Game {
  std::vector<Faction> factions;
public:
  explicit Game(std::vector<Faction> &&factions):factions{factions} {
  }

   [[nodiscard]] const std::vector<Faction>& getFactions() const {
    return factions;
  }


};


#endif //MOTION_GAME_HPP
