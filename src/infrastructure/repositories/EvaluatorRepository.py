from infrastructure.repositories import IEvaluatorRepository
from infrastructure.repositories import ILLMRepository
from infrastructure.parsers import MarkdownParser
from domain.entities import File, Weakness
from domain.services import ICodeService
import logging

class EvaluatorRepository(IEvaluatorRepository):

    def __init__(
        self, 
        model: str, 
        user_prompt: str, 
        system_prompt: str,
        parameters: dict, 
        llm_repository: ILLMRepository,
        code_service: ICodeService,
        attempts: int = 5):
        self.model: str = model
        self.user_prompt: str = user_prompt
        self.system_prompt: str = system_prompt
        self.parameters: dict = parameters
        self.llm_repository: ILLMRepository = llm_repository
        self.code_service: ICodeService = code_service
        self.attempts: int = attempts
        self.markdown_parser: MarkdownParser = MarkdownParser()
        self.logger = logging.getLogger(__name__)

    def _build_weakness(self, weakness_dict: dict) -> Weakness:
        weakness = Weakness(
            type=weakness_dict.get("type"),
            weakness=weakness_dict.get("weakness"),
            description=weakness_dict.get("description"),
            severity=weakness_dict.get("severity"),
            code=weakness_dict.get("code"),
            justification=weakness_dict.get("justification")
        )
        self.logger.info(f"{weakness.weakness} weakness found")
        self.logger.info(f"Weakness type is {weakness.type}")
        self.logger.info(f"Weakness severity is {weakness.severity}")
        self.logger.info(f"Weakness description is {weakness.description}")
        self.logger.info(f"Weakness justification is {weakness.justification}")
        self.logger.info(f"The weakness is in the following code: {weakness.code}")
        return weakness


    def _evaluate(self, path: str, code_unit: str) -> list[Weakness]:
        weaknesses = []
        for _ in range(self.attempts):
            try:
                user_prompt = self.user_prompt.format(code=code_unit)
                response = self.llm_repository.create_message(
                    model=self.model,
                    user_prompt=user_prompt,
                    system_prompt=self.system_prompt,
                    parameters=self.parameters,
                    )
                weaknesses = self.markdown_parser.parse(response)
                for weakness_dict in weaknesses:
                    weakness = self._build_weakness(weakness_dict)
                    weaknesses.append(weakness)
                return weaknesses
            except Exception as e:
                self.logger.error(f"Error evaluating file {path}: {e}")
                self.logger.error("Trying to evaluate the file again...")
        raise Exception(f"Failed to evaluate file {path} after {self.attempts} attempts.")

    def evaluate(self, file: File) -> File:
        code_units = self.code_service.extract_code_units(file)
        imports = "\n".join(self.code_service.extract_imports(file))
        globals_ = "\n".join(self.code_service.extract_global_variables(file))
        code_unit = f"{imports}\n{globals_}"
        code_units.append(code_unit)
        self.logger.info(f"Evaluating file {file.path}")
        for code_unit in code_units:
            weaknesses = self._evaluate(file.path, code_unit)
            file.weaknesses.extend(weaknesses)
        return file
