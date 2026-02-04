import torch
from datasets import load_dataset
from transformers import (
    AutoTokenizer,
    AutoModelForCausalLM,
    TrainingArguments,
    Trainer,
    DataCollatorForLanguageModeling,
)
from peft import LoraConfig, get_peft_model

MODEL_NAME = "eryk-mazus/polka-1.1b"
DATA_FILE = "data/processed/chords.txt"
OUT_DIR = "gpt2/polka_lora"

def main():
    tokenizer = AutoTokenizer.from_pretrained(MODEL_NAME)
    tokenizer.pad_token = tokenizer.eos_token

    dataset = load_dataset("text", data_files={"train": DATA_FILE})

    def tokenize(batch):
        return tokenizer(
            batch["text"],
            truncation=True,
            max_length=128,
        )

    tokenized = dataset.map(
        tokenize,
        batched=True,
        remove_columns=["text"],
    )

    model = AutoModelForCausalLM.from_pretrained(
        MODEL_NAME,
        dtype=torch.float32,
        device_map="cpu",
    )

    lora_config = LoraConfig(
        r=8,
        lora_alpha=32,
        target_modules=["q_proj", "v_proj"],
        lora_dropout=0.05,
        bias="none",
        task_type="CAUSAL_LM",
    )

    model = get_peft_model(model, lora_config)
    model.print_trainable_parameters()

    training_args = TrainingArguments(
        output_dir=OUT_DIR,

        per_device_train_batch_size=1,
        gradient_accumulation_steps=8,

        max_steps=400,
        learning_rate=1e-4,
        max_grad_norm=1.0,

        logging_steps=20,
        save_steps=200,
        save_total_limit=1,

        report_to="none",
        remove_unused_columns=False,
    )

    data_collator = DataCollatorForLanguageModeling(
        tokenizer=tokenizer,
        mlm=False,
    )

    trainer = Trainer(
        model=model,
        args=training_args,
        train_dataset=tokenized["train"],
        data_collator=data_collator,
    )

    trainer.train()

    model.save_pretrained(OUT_DIR)
    tokenizer.save_pretrained(OUT_DIR)

if __name__ == "__main__":
    main()
