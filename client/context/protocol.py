from enum import Enum


class ContextEnum(Enum):
    LOBBY = bytearray((0x01,))


class LobbyContextCommands(Enum):
    CREATE = bytearray((0x01,))
    JOIN = bytearray((0x02,))
    GET = bytearray((0x03,))
    LEAVE = bytearray((0x04,))
    PLAYERS = bytearray((0x05,))


class Time(Enum):
    DAY = bytearray((0x0,))
    NIGHT = bytearray((0x1,))
    DUSK = bytearray((0x2,))


class Map(Enum):
    PLAIN = bytearray((0x0,))


class Weather(Enum):
    CLEAR = bytearray((0x0,))
    RAIN = bytearray((0x1,))
    WIND = bytearray((0x2,))

COMM_PROTOCOL = bytearray((0x70, 0x42, 0xE1, 0x1C))
HEADER = bytearray(16)
