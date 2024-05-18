from .context import Context
from .menu import Menu
from .protocol import COMM_PROTOCOL, HEADER


class Interpreter(Context):
    def __init__(self):
        super().__init__()
        self.context = Menu()

    def get_current_context(self) -> str:
        return str(self.context)

    def parse(self, words: list[str]) -> bytearray | None:
        if words[0] == "exit":
            raise SystemExit

        parsed = self.context.parse(words)
        next_context = self.context.get_next_context()
        if next_context is not None:
            self.context = next_context
        if parsed is not None:
            return COMM_PROTOCOL + HEADER + parsed
    def __str__(self):
        return str(self.context)