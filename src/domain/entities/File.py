from domain.constants import EVALUATOR_SUPPORTED_EXTENSIONS
from domain.entities.Weakness import Weakness
from pydantic import BaseModel
from typing import List


class File(BaseModel):
    file_path: str
    extension: str
    content: str
    weaknesses: List[Weakness] = []

    def __init__(self, file_path: str, content: str):
        extension = file_path.split(".")[-1].lower()
        # if extension not in EVALUATOR_SUPPORTED_EXTENSIONS:
        #     raise ValueError(f"Extension {extension} is not supported.")
        super().__init__(
            file_path=file_path, 
            extension=extension, 
            content=content)
        
    def __eq__(self, other):
        if not isinstance(other, File):
            return NotImplemented
        return self.file_path == other.file_path

    def __hash__(self):
        return hash(self.file_path)

    def __str__(self):
        file_str = f"file_path: {self.file_path}\n"
        file_str += f"Weaknesses:\n"
        for weakness in self.weaknesses:
            file_str += f"{str(weakness)}\n"
        return file_str