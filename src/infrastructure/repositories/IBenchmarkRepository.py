from abc import ABC, abstractmethod
from typing import List

class IBenchmarkRepository(ABC):
    
    @abstractmethod
    def execute(
        self, 
        id_prefix: str,
        dataset: str,
        runs: int,
        models: List[str],
        user_prompts: List[str],
        system_prompts: List[str],
        parameters: List[dict],
        save_path: str,
    ) -> dict:
        raise NotImplementedError(
            "This method should be implemented in subclasses.")