from infrastructure.repositories import IEvaluatorRepository
from application.use_cases import IEvaluatorUseCase
from domain.entities import File

class EvaluatorUseCase(IEvaluatorUseCase):
    def __init__(self, evaluator_repository: IEvaluatorRepository):
        self.evaluator_repository = evaluator_repository

    def evaluate(self, file: File) -> File:
        return self.evaluator_repository.evaluate(file)