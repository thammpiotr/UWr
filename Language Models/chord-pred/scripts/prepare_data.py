from datasets import load_dataset
from pathlib import Path
import re

OUT_FILE = Path("data/processed/chords.txt")
OUT_FILE.parent.mkdir(parents=True, exist_ok=True)

def normalize(text):
    text = re.sub(r"\s+", " ", text)
    return text.strip()

def main():
    ds = load_dataset("ailsntua/Chordonomicon")

    total = 0

    with open(OUT_FILE, "w", encoding="utf-8") as f:
        for example in ds["train"]:
            chords = example["chords"]
            if isinstance(chords, str):
                line = normalize(chords)
                if line:
                    f.write(line + "\n")
                    total += 1

    print(f"Saved: {total} to {OUT_FILE}")

if __name__ == "__main__":
    main()
