# Strategy game

most interesting code is in server folder

compile with clion the following target:
```
run_game_server
```

the game server starts on udp port 8080

to utilize the game server, you can use the `client.py`.
It already has all the required command parsing currently available. It just transforms commands from human readable format to bytestreams and sends them to the server.
Client is currently Ubuntu-only.

```
$ python3 ./client.py
(b'pB\xe1\x1c\x01\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00\x00', ('127.0.0.1', 8080))
> help
        lobby: lobby commands; `lobby help` for more info

```
lobby commands;
```
> lobby help
        usage: lobby <create|get|join|parse_leave|players> ... args
                create: create a new lobby
                get: get all lobbies
                join <lobby_id>: join a lobby
                leave: parse_leave the current lobby
                players <lobby_id>: get all players in a lobby

```
server has almost no logs
all data is sent back to client, but is not parsed yet on the client side
## protocol diagram

![image](https://github.com/Siromanec/course-cxx-real-time-multiplayer-strategy-game/assets/91615647/f44f38ca-84d7-42cd-93ba-feda67588f68)

