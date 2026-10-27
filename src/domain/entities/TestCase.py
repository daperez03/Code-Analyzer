from domain.entities import File, Weakness
from collections import defaultdict
from pydantic import BaseModel
from typing import List, Dict

class TestCase(BaseModel):
    name: str
    weaknesses: Dict[File, List[Weakness]] = defaultdict(list)
    files: List[File] = []

    def __str__(self):
        test_case_str = f"Test Case: {self.name}\n"
        test_case_str += f"Files:\n"
        for file in self.files:
            test_case_str += f"File Path: {file.file_path}\n"
            test_case_str += f"Weaknesses:\n"
            for weakness in self.weaknesses[file]:
                test_case_str += f"{str(weakness)}\n"
        return test_case_str

    def add_case(self, file: File, weaknesses: List[Weakness]):
        if file not in self.files:
            self.files.append(file)
            self.weaknesses[file].extend(weaknesses)
        else:
            raise ValueError(
                f"File {file.file_path} already exists in the test case.")
