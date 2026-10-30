from abc import ABC, abstractmethod
from domain.entities import File

class ICodeService(ABC):

    @abstractmethod
    def remove_comments(self, file: File) -> File:
        pass

    @abstractmethod
    def extract_code_units(self, file: File) -> list[str]:
        pass

    @abstractmethod
    def extract_imports(self, file: File) -> list[str]:
        pass

    @abstractmethod
    def extract_global_variables(self, file: File) -> list[str]:
        pass