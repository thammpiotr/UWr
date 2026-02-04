import torch
from pathlib import Path
from transformers import AutoTokenizer, AutoModelForCausalLM
from peft import PeftModel

BASE_MODEL = "eryk-mazus/polka-1.1b"
LORA_DIR = Path(__file__).parent / "polka_lora"

tokenizer = AutoTokenizer.from_pretrained(LORA_DIR)
model = AutoModelForCausalLM.from_pretrained(
    BASE_MODEL,
    torch_dtype=torch.float32,
    device_map="cpu",
)

model = PeftModel.from_pretrained(model, LORA_DIR)
model.eval()

def generate(prompt):
    inputs = tokenizer(prompt, return_tensors="pt")
    with torch.no_grad():
        out = model.generate(
            **inputs,
            max_new_tokens=80,
            temperature=0.9,
            top_k=50,
            top_p=0.95,
            repetition_penalty=1.2,
            do_sample=True,
            pad_token_id=tokenizer.eos_token_id,
        )
    return tokenizer.decode(out[0], skip_special_tokens=True)

print("Prompt:")
while True:
    try:
        prompt = input("\n> ")
        if prompt.lower() == "q":
            break
        if not prompt.strip():
            continue
        result = generate(prompt)
        print(f"\n{result}")
    except KeyboardInterrupt:
        break
