from typing import Literal

from .context import Context
from .protocol import Map, Time, Weather, ContextEnum, LobbyContextCommands


class LobbyBuilder:
    def __init__(self):
        self.map: bytearray = Map.PLAIN.value
        self.time: bytearray = Time.DAY.value
        self.weather: bytearray = Weather.CLEAR.value
        self.max_players: bytearray = bytearray((0x2,))
        self.max_units = (20).to_bytes(1, "little")
        self.budget = (1000).to_bytes(4, "little")
        self.ai_players: dict[int, int] = None # dict<team_id, cnt>len|cnt|team_id|...|cnt|team_id or 0
    def set_map(self, map: str):
        # match case for map to bytes
        map = map.upper()
        if map not in Map.__members__: raise ValueError("Invalid map")
        self.map = Map[map].value
    def set_weather(self, weather: Literal['clear', 'rain', 'wind']):
        weather = weather.upper()
        if weather not in Weather.__members__: raise ValueError("Invalid weather")
        self.weather = Weather[weather].value
    def set_time(self, time: Literal['day', 'night', 'dusk']):
        time = time.upper()
        if time not in Time.__members__: raise ValueError("Invalid time")
        self.time = Time[time].value
    def set_max_players(self, max_players: int):
        self.max_players = max_players.to_bytes(1, "little")
    # def add_ai_player(self, team_id: int):
    #     if team_id < 0 or team_id >= int(self.max_players):
    #         raise ValueError("Invalid team")
    #     if self.ai_players is None:
    #         self.ai_players = dict()
    #     self.ai_players[team_id] = self.ai_players.get(team_id, 0) + 1
    # def remove_ai_player(self, team_id: int):
    #     if self.ai_players is None:
    #         return
    #     if team_id not in self.ai_players:
    #         return
    #     self.ai_players[team_id] -= 1
    #     if self.ai_players[team_id] == 0:
    #         del self.ai_players[team_id]
    # def ai_dict_to_bytes(self) -> bytes | bytearray:
    #     if self.ai_players is None:
    #         return b"\x00"
    #     res = len(self.ai_players).to_bytes(1, "little")
    #     for team_id, cnt in self.ai_players.items():
    #         res += team_id.to_bytes(1, "little") + cnt.to_bytes(1, "little")
    #     return res
    def build(self):
        return self.time + self.map + self.weather + self.max_players + self.max_units + self.budget #+ self.ai_dict_to_bytes()

class CreateLobby(Context):
    def __init__(self):
        super().__init__()
        self.lobby_builder = LobbyBuilder()

    def parse(self, words: list[str]) -> bytearray| None:
        match words[0]:
            case "map":
                if len(words) != 2:
                    print("usage: map <map_name>")
                    return
                try:
                    self.lobby_builder.set_map(words[1])
                except ValueError as e:
                    print(e)
            case "weather":
                if len(words) != 2:
                    print("usage: weather <clear|rain|wind>")
                    return
                try:
                    self.lobby_builder.set_weather(words[1])
                except ValueError as e:
                    print(e)
            case "time":
                if len(words) != 2:
                    print("usage: time <day|night|dusk>")
                    return
                try:
                    self.lobby_builder.set_time(words[1])
                except ValueError as e:
                    print(e)
            case "max_players":
                if len(words) != 2:
                    print("usage: max_players <number> # excludes AI players")
                    return
                try:
                    self.lobby_builder.set_max_players(int(words[1]))
                except ValueError as e:
                    print(e)
            case "max_teams":
                if len(words) != 2:
                    print("usage: max_teams <number>")
                    return
                try:
                    self.lobby_builder.set_max_teams(int(words[1]))
                except ValueError as e:
                    print(e)
            # case "ai":
            #     match words[1]:
            #         case "add":
            #             if len(words) != 3:
            #                 print("usage: ai add <team_id>")
            #                 return
            #             self.lobby_builder.add_ai_player(int(words[2]))
            #         case "remove":
            #             if len(words) != 3:
            #                 print("usage: ai remove <team_id>")
            #                 return
            #             self.lobby_builder.remove_ai_player(int(words[2]))
            case "create":
                # somehow return bytes [AND join the lobby] AND change state to army creation
                from .army_creation import ArmyCreationContext
                self.next_context = ArmyCreationContext()
                return ContextEnum.LOBBY.value + LobbyContextCommands.CREATE.value + self.lobby_builder.build()
            case "back":
                from .menu import Menu
                self.next_context = Menu()
                return
            case "help":
                print("\tusage:")
                print("\t\tmap <map_name>: set map")
                print("\t\tweather <clear|rain|wind>: set weather")
                print("\t\ttime <day|night|dusk>: set time")
                print("\t\tmax_players <number>: set max players")
                print("\t\tmax_teams <number>: set max teams")
                print("\t\tai add <team_id>: add AI player")
                print("\t\tai remove <team_id>: remove AI player")
                print("\t\tcreate: create lobby")
                print("\t\tback: go back")
                return

    def __str__(self):
        return "create_lobby"

class JoinLobby(Context):

    def __init__(self):
        super().__init__()

    def parse(self, words: list[str]) -> bytearray | None:

        parsed: bytes = b""
        match words[0]:
            case "join":
                if len(words) != 2:
                    print("usage: join <lobby_id>")
                    return
                lobby_id = int(words[1])
                parsed = lobby_id.to_bytes(8, "little")
                from .army_creation import ArmyCreationContext
                self.next_context = ArmyCreationContext()
            case "get":
                parsed = LobbyContextCommands.GET.value
            case "leave":
                if len(words) != 1:
                    print("usage: leave")
                    return
                parsed = LobbyContextCommands.LEAVE.value
            case "players":
                if len(words) != 2:
                    print("usage: players <lobby_id>")
                    return
                lobby_id = int(words[1])
                parsed = LobbyContextCommands.PLAYERS.value + lobby_id.to_bytes(8, "little")
            case "help":
                print("\tusage:")
                print("\t\tget: get all lobbies")
                print("\t\tjoin <lobby_id>: join a lobby")
                print("\t\tleave: leave the current lobby")
                print("\t\tplayers <lobby_id>: get all players in a lobby")
                return
            case "back":
                from .menu import Menu
                self.next_context = Menu()
                return
        return ContextEnum.LOBBY.value + parsed


    def __str__(self):
        return "join_lobby"