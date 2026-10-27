from infrastructure.repositories import IBenchmarkRepository, IEvaluatorRepository
from infrastructure.repositories import LocalLLMRepository, GoogleLLMRepository
from infrastructure.repositories import EvaluatorRepository
from domain.entities import TestCase, Dataset, File, Weakness
from domain.constants import EVALUATOR_SUPPORTED_EXTENSIONS, EVALUATOR_CWES
from domain.constants.LLM import LOCAL_LLM_SUPPORTED_MODELS, GOOGLE_LLM_SUPPORTED_MODELS
from collections import defaultdict
from typing import List, Dict
from tqdm import tqdm
import logging
import json
import copy
import os

class BenchmarkRepository(IBenchmarkRepository):
    def __init__(self):
        self.llm_repositories = {}
        self.logger = logging.getLogger(__name__)

    def _get_evaluator(
        self, 
        model: str, 
        user_prompt: str, 
        system_prompt: str, 
        parameters: dict) -> IEvaluatorRepository:
        llm_repository = None
        if model in LOCAL_LLM_SUPPORTED_MODELS:
            if LocalLLMRepository.__name__ not in self.llm_repositories:
                self.llm_repositories[LocalLLMRepository.__name__] = LocalLLMRepository()
            llm_repository = self.llm_repositories[LocalLLMRepository.__name__]
        elif model in GOOGLE_LLM_SUPPORTED_MODELS:
            if GoogleLLMRepository.__name__ not in self.llm_repositories:
                self.llm_repositories[GoogleLLMRepository.__name__] = GoogleLLMRepository()
            llm_repository = self.llm_repositories[GoogleLLMRepository.__name__]
        else:
            raise ValueError(f"Model {model} is not supported.")
        return EvaluatorRepository(
            model=model,
            user_prompt=user_prompt,
            system_prompt=system_prompt,
            parameters=parameters,
            llm_repository=llm_repository)

    def _read_test_case(self, test_case_metadata: dict, dataset_name: str) -> TestCase:
        test_case = TestCase(name = test_case_metadata["identifier"])
        weaknesses: Dict[File, List[Weakness]] = defaultdict(list)

        for run in test_case_metadata["sarif"]["runs"]:
            for result in run["results"]:
                weakness_id = result["ruleId"]
                description = result["message"]["text"]
                if not weakness_id in EVALUATOR_CWES: continue
                for location in result["locations"]:
                    file_path = os.path.join(
                        "datasets", 
                        dataset_name,
                        location["physicalLocation"]["artifactLocation"]["uri"]
                    )
                    start_line = location["physicalLocation"]["region"]["startLine"] - 1
                    end_line = start_line + 1
                    if "endLine" in location["physicalLocation"]["region"]:
                        end_line = location["physicalLocation"]["region"]["endLine"] + 1

                    content = ""
                    with open(file_path, "r") as f:
                        content = f.read()
                    file = File(file_path=file_path,content=content)

                    code = (content.splitlines(keepends=True)[start_line:end_line])
                    if not code: continue
                    code = "".join(code)
                    weaknesses[file].append(
                        Weakness(
                            type="N/A",
                            weakness=weakness_id,
                            description=description,
                            severity="N/A",
                            code=code,
                            justification=description
                        )
                    )

        for file, weakness_list in weaknesses.items():
            if weakness_list:
                test_case.add_case(file=file, weaknesses=weakness_list)
        return test_case if test_case.files else None

    def _read_dataset(self, dataset_name: str) -> Dataset:
        path = os.path.join("datasets", dataset_name, "sarifs.json")
        dataset = Dataset(name=dataset_name)
        metadata = {}
        with open(path, "r") as f:
            metadata = json.load(f)
        for test_case_metadata in metadata["testCases"]:
            test_case = self._read_test_case(test_case_metadata, dataset_name)
            if test_case: dataset.test_cases.append(test_case)
        return dataset

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
        self.logger.info(f"Starting benchmark {id_prefix}")
        dataset = self._read_dataset(dataset)
        datasets = [copy.deepcopy(dataset) for _ in range(runs)]
        progress_bar = tqdm(
            total=runs * sum(len(tc.files) for tc in dataset.test_cases),
            desc="Executing benchmark",
            unit="File"
        )
        for run in range(runs):
            root_logger = logging.getLogger()
            id = f"{id_prefix}_{run+1}"
            handler = logging.FileHandler(
                f"logs/{id}.log", mode='w', encoding='utf-8')
            handler.setFormatter(
                logging.Formatter(
                '%(asctime)s - %(levelname)s - %(name)s - %(message)s'))
            root_logger.addHandler(handler)
            self.logger.info(f"Starting run {id}")
            dataset: Dataset = datasets[run]
            self.logger.info(f"Selected dataset is {dataset.name}")
            model = models[run]
            self.logger.info(f"Selected model is {model}")
            user_prompt = user_prompts[run]
            system_prompt = system_prompts[run]
            parameter = parameters[run]
            self.logger.info(f"Used parameters are {parameter}")
            evaluator = self._get_evaluator(
                model=model,
                user_prompt=user_prompt,
                system_prompt=system_prompt,
                parameters=parameter
            )
            for test_case in dataset.test_cases:
                self.logger.info(f"Executing test case {test_case.name}")
                for file in test_case.files:
                    self.logger.info(f"Executing file {file.file_path}")
                    progress_bar.update(1)
                    evaluator.evaluate(file)
            result_path = os.path.join(save_path, f"{id}.json")
            os.makedirs(save_path, exist_ok=True)
            with open(result_path, "w") as f:
                dataset_json = dataset.model_dump_json(indent=2)
                f.write(dataset_json)
                self.logger.info(
                    f"Saved results for run {run+1}/{runs} to {result_path}")
            root_logger.removeHandler(root_logger.handlers[-1])
            