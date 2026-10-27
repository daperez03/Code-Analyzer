from domain.entities import TestCase
from pydantic import BaseModel
from typing import List

class Dataset(BaseModel):
    name: str
    test_cases: List[TestCase] = []

    def __init__(self, name: str):
        self.name = name

    def __str__(self):
        dataset_str = f"Dataset: {self.name}\n"
        dataset_str += f"Test Cases:\n"
        for test_case in self.test_cases:
            dataset_str += f"{str(test_case)}\n"
        return dataset_str