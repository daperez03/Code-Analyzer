from application.use_cases import IBenchmarkUseCase
from infrastructure.repositories import IBenchmarkRepository
from typing import List

class BenchmarkUseCase(IBenchmarkUseCase):
    def __init__(self, benchmark_repository: IBenchmarkRepository):
        self.benchmark_repository: IBenchmarkRepository = benchmark_repository

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
        if len(models) != runs:
            raise ValueError("The length of models must be equal to the number of runs.")
        
        if len(user_prompts) != runs:
            raise ValueError("The length of user_prompts must be equal to the number of runs.")
        if len(system_prompts) != runs:
            raise ValueError("The length of system_prompts must be equal to the number of runs.")
        if len(parameters) != runs:
            raise ValueError("The length of parameters must be equal to the number of runs.")
        self.benchmark_repository.execute(
            id_prefix=id_prefix,
            runs=runs,
            models=models,
            user_prompts=user_prompts,
            system_prompts=system_prompts,
            parameters=parameters,
            dataset=dataset,
            save_path=save_path,
        )