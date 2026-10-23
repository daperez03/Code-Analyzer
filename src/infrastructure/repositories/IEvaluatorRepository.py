
from domain.entities.File import File
from abc import ABC, abstractmethod

class IEvaluatorRepository(ABC):
    @abstractmethod
    def evaluate(self, file: File) -> File:
        raise NotImplementedError(
            "This method should be implemented in subclasses.")