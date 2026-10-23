from google import genai

from infrastructure.repositories import ILLMRepository
import logging

class GoogleLLMRepository(ILLMRepository):
    def __init__(self):
        self.client = genai.Client()
        self.logger = logging.getLogger(__name__)

    def create_message(
        self, 
        model: str,
        user_prompt: str, 
        system_prompt: str = None,
        parameters: dict = {}) -> str:

        self.logger.info(f"Creating message with model {model}\n")
        self.logger.info(f"User prompt: \n{user_prompt}")
        if system_prompt:
            self.logger.info(f"System prompt: \n{system_prompt}")
        self.logger.info(f"Parameters: \n{parameters}")


        interaction = self.client.interactions.create(
            model=model,
            system_instruction=system_prompt,
            input=user_prompt,
            generation_config=parameters
        )

        self.logger.info(f"Response from LLM:\n{interaction.output_text}")

        return interaction.output_text