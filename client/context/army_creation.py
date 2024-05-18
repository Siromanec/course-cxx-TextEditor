from .context import Context
class ArmyCreationContext(Context):
    def __init__(self):
        super().__init__()
    def parse(self, words: list[str]) -> bytearray | None:
        return
    def __str__(self):
        return "army_creation"