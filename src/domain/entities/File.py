from domain.constants import EVALUATOR_SUPPORTED_EXTENSIONS
from domain.entities.Weakness import Weakness
from pydantic import BaseModel
from typing import List


class File(BaseModel):
    path: str
    extension: str
    content: str
    weaknesses: List[Weakness] = []

    def __init__(self, path: str, content: str):
        extension = path.split(".")[-1].lower()
        # if extension not in EVALUATOR_SUPPORTED_EXTENSIONS:
        #     raise ValueError(f"Extension {extension} is not supported.")
        super().__init__(
            path=path, 
            extension=extension, 
            content=content)
        
    def __eq__(self, other):
        if not isinstance(other, File):
            return NotImplemented
        return self.path == other.path

    def __hash__(self):
        return hash(self.path)

    def __str__(self):
        file_str = f"path: {self.path}\n"
        file_str += f"Weaknesses:\n"
        for weakness in self.weaknesses:
            file_str += f"{str(weakness)}\n"
        return file_str