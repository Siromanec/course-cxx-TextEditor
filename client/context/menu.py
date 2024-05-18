from .context import Context

class Menu(Context):
    def __init__(self):
        super().__init__()

    def parse(self, words: list[str]) -> bytearray | Context | None:
        match words[0]:
            case "join_lobby":
                from .lobby import JoinLobby
                self.next_context = JoinLobby()
                return
            case "new_lobby":
                from .lobby import CreateLobby
                self.next_context = CreateLobby()
                return
            case "help":
                print("\tjoin_lobby: lobby joining and discovery")
                print("\tnew_lobby: create lobby")
        pass

    def __str__(self):
        return "menu"
