from abc import ABC, abstractmethod
from typing import List

class IBenchmarkUseCase(ABC):

    @abstractmethod
    def execute(
        self, 
        id_prefix: str,
        runs: int,
        dataset: str,
        models: List[str],
        user_prompts: List[str],
        system_prompts: List[str],
        parameters: List[dict],
        save_path: str,
    ) -> dict:
        raise NotImplementedError(
            "This method should be implemented in subclasses.")