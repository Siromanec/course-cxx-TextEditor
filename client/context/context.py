class Context: ...

class Context:
    def __init__(self):
        self.next_context = None
    def parse(self, words: list[str]) -> bytearray | None:
        raise NotImplementedError
    def get_next_context(self) -> Context | None:
        return self.next_context

    def __str__(self):
        raise NotImplementedError

