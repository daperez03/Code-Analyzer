from infrastructure.repositories import IBenchmarkRepository, BenchmarkRepository
from infrastructure.repositories import IEvaluatorRepository, EvaluatorRepository
from infrastructure.repositories import ILLMRepository, LocalLLMRepository
from infrastructure.services import CodeService
from application.use_cases import IBenchmarkUseCase, BenchmarkUseCase
from application.use_cases import IEvaluatorUseCase, EvaluatorUseCase
from application.use_cases import ILLMUseCase, LLMUseCase
from domain.constants import EVALUATOR_USER_PROMPT, EVALUATOR_SYSTEM_PROMPT
from domain.constants import LLM_MODEL, LLM_PARAMETERS
from domain.services import ICodeService
import logging
import os

os.makedirs("logs", exist_ok=True)
logging.basicConfig(
    level=logging.INFO, 
    format="%(asctime)s - %(levelname)s - %(name)s - %(message)s",
    handlers=[
        logging.FileHandler("logs/analyzer.log", mode="a", encoding="utf-8"),
    ]
)

code_service: ICodeService = CodeService()

llm_repository: ILLMRepository = LocalLLMRepository()
llm_use_case: ILLMUseCase = LLMUseCase(llm_repository=llm_repository)

evaluator_repository: IEvaluatorRepository = EvaluatorRepository(
    model=LLM_MODEL,
    user_prompt=EVALUATOR_USER_PROMPT,
    system_prompt=EVALUATOR_SYSTEM_PROMPT,
    parameters=LLM_PARAMETERS,
    llm_repository=llm_repository,
    code_service=code_service,
)
evaluator_use_case: IEvaluatorUseCase = EvaluatorUseCase(
    evaluator_repository=evaluator_repository)

benchmark_repository: IBenchmarkRepository = BenchmarkRepository(
    code_service=code_service)
benchmark_use_case: IBenchmarkUseCase = BenchmarkUseCase(
    benchmark_repository=benchmark_repository)