from abc import ABC, abstractmethod

class ILLMRepository(ABC):
    
    @abstractmethod
    def create_message(
            self, 
            model: str,
            user_prompt: str, 
            system_prompt: str = None,
            parameters: dict = None) -> str:
        raise NotImplementedError(
            "This method should be implemented in subclasses.")