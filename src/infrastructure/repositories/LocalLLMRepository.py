from transformers import AutoTokenizer, AutoModelForCausalLM
from transformers.utils import logging as hf_logging
hf_logging.disable_progress_bar()
from typing import Tuple
import logging
import torch
import os

from infrastructure.repositories import ILLMRepository

class LocalLLMRepository(ILLMRepository):
    def __init__(self):
        self.models = {}
        hf_logging.disable_progress_bar()
        self.logger = logging.getLogger(__name__)
    
    def _load_model(self, model: str) -> Tuple[AutoTokenizer, AutoModelForCausalLM]:
        if model in self.models:
            return self.models[model]

        model_path = os.path.join("models", model)
        tokenizer = AutoTokenizer.from_pretrained(
            model_path,
            local_files_only=True
        )
        llm = AutoModelForCausalLM.from_pretrained(
            model_path,
            local_files_only=True,
            torch_dtype="auto",
            device_map="auto",
        )
        llm.eval()
        self.models[model] = (tokenizer, llm)
        return tokenizer, llm

    def create_message(
        self, 
        model: str,
        user_prompt: str, 
        system_prompt: str = None,
        parameters: dict = {}) -> str:
        self.logger.info(f"Creating message with model {model}")
        self.logger.info(f"User prompt: \n{user_prompt}")
        if system_prompt:
            self.logger.info(f"System prompt: \n{system_prompt}")
        self.logger.info(f"Parameters: \n{parameters}")

        # Load the model and tokenizer
        tokenizer, llm = self._load_model(model)

        # Prepare the messages for the model input
        messages = []
        if system_prompt:
            messages.append({
                "role": "system",
                "content": system_prompt,
            })
        messages.append({
            "role": "user",
            "content": user_prompt,
        })
        inputs = tokenizer.apply_chat_template(
            messages,
            tokenize=True,
            add_generation_prompt=True,
            return_tensors="pt",
            return_dict=True,
            enable_thinking=False,
        )
        inputs = inputs.to(llm.device)


        # Generate the response
        with torch.inference_mode():
            outputs = llm.generate(
                **inputs,
                **parameters,
            )
        
        input_length = inputs["input_ids"].shape[-1]
        new_tokens = outputs[0, input_length:]
        response = tokenizer.decode(
            new_tokens,
            skip_special_tokens=True,
        )

        self.logger.info(f"Response from LLM:\n{response}")

        return response