from typing import List

class ChordTokenizer:
    def __init__(self, tokens: List[str]):
        uniq = sorted(set(tokens))
        self.token2id = {t: i for i, t in enumerate(uniq)}
        self.id2token = {i: t for t, i in self.token2id.items()}
        self.vocab_size = len(uniq)

    def encode(self, tokens: List[str]):
        return [self.token2id[t] for t in tokens]

    def decode(self, ids: List[int]):
        return [self.id2token[i] for i in ids]
