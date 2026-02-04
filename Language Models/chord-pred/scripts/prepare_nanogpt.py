import numpy as np
import pickle
from pathlib import Path
from chord_tokenizer.tokenizer import ChordTokenizer

DATA_FILE = Path("data/processed/chords.txt")
OUT_DIR = Path("nanoGPT/data/chords")
OUT_DIR.mkdir(parents=True, exist_ok=True)

def main():
    with open(DATA_FILE, "r", encoding="utf-8") as f:
        lines = [l.strip() for l in f if l.strip()]

    sequences = [line.split(" ") for line in lines]
    all_tokens = [tok for seq in sequences for tok in seq]

    tokenizer = ChordTokenizer(all_tokens)

    encoded = [tokenizer.encode(seq) for seq in sequences]

    data = np.array([i for seq in encoded for i in seq], dtype=np.uint16)

    split = int(0.9 * len(data))
    train_ids = data[:split]
    val_ids = data[split:]

    train_ids.tofile(OUT_DIR / "train.bin")
    val_ids.tofile(OUT_DIR / "val.bin")

    meta = {
        "vocab_size": tokenizer.vocab_size,
        "token2id": tokenizer.token2id,
        "id2token": tokenizer.id2token,
    }
    with open(OUT_DIR / "meta.pkl", "wb") as f:
        pickle.dump(meta, f)

if __name__ == "__main__":
    main()
