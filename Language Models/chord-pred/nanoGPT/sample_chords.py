import torch
import pickle
from model import GPTConfig, GPT

ckpt = torch.load("out/ckpt.pt", map_location="cpu")
config = GPTConfig(**ckpt["model_args"])
model = GPT(config)
model.load_state_dict(ckpt["model"])
model.eval()

with open("data/chords/meta.pkl", "rb") as f:
    meta = pickle.load(f)
id2token = meta["id2token"]
token2id = meta["token2id"]

def decode(ids):
    return " ".join(id2token[i] for i in ids)

def encode(text):
    tokens = text.strip().split()
    return [token2id[t] for t in tokens if t in token2id]

def generate(start_tokens):
    idx = torch.tensor([start_tokens], dtype=torch.long)
    with torch.no_grad():
        out = model.generate(
            idx,
            max_new_tokens=80,
            temperature=1.0,
            top_k=20,
        )
    return decode(out[0].tolist())

print("Prompt:")

while True:
    try:
        prompt = input("\n> ")
        if prompt.lower() == "q":
            break
        if not prompt.strip():
            continue
        
        tokens = encode(prompt)
        if not tokens:
            print("Uknown token")
            continue
        result = generate(tokens)
        print(f"\n{result}")
    except KeyboardInterrupt:
        break
