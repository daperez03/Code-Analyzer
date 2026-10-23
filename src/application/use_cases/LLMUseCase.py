from infrastructure.repositories import ILLMRepository
from application.use_cases import ILLMUseCase

class LLMUseCase(ILLMUseCase):

    def __init__(self, llm_repository: ILLMRepository):
        self.llm_repository = llm_repository
    
    def create_message(
        self, 
        model: str, 
        user_prompt: str, 
        system_prompt: str = None, 
        parameters: dict = None) -> str:
        return self.llm_repository.create_message(
            model=model,
            user_prompt=user_prompt,
            system_prompt=system_prompt,
            parameters=parameters
        )