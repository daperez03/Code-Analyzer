from infrastructure.repositories import IEvaluatorRepository
from infrastructure.repositories import ILLMRepository
from infrastructure.parsers import MarkdownParser
from domain.entities import File, Weakness
import logging

class EvaluatorRepository(IEvaluatorRepository):

    def __init__(
        self, 
        model: str, 
        user_prompt: str, 
        system_prompt: str,
        parameters: dict, 
        llm_repository: ILLMRepository):
        self.model = model
        self.user_prompt = user_prompt
        self.system_prompt = system_prompt
        self.parameters = parameters
        self.llm_repository = llm_repository
        self.markdown_parser = MarkdownParser()
        self.logger = logging.getLogger(__name__)


    def evaluate(self, file: File) -> File:
        self.logger.info(f"Evaluating file {file.file_path}")
        user_prompt = self.user_prompt.format(code=file.content)
        response = self.llm_repository.create_message(
            model=self.model,
            user_prompt=user_prompt,
            system_prompt=self.system_prompt,
            parameters=self.parameters,
        )
        weaknesses = self.markdown_parser.parse(response)
        for weakness_dict in weaknesses:
            weakness = Weakness(
                type=weakness_dict.get("type"),
                weakness=weakness_dict.get("weakness"),
                description=weakness_dict.get("description"),
                severity=weakness_dict.get("severity"),
                code=weakness_dict.get("code"),
                justification=weakness_dict.get("justification")
            )
            file.weaknesses.append(weakness)
            self.logger.info(f"{weakness.weakness} weakness found")
            self.logger.info(f"Weakness type is {weakness.type}")
            self.logger.info(f"Weakness severity is {weakness.severity}")
            self.logger.info(f"Weakness description is {weakness.description}")
            self.logger.info(f"Weakness justification is {weakness.justification}")
            self.logger.info(f"The weakness is in the following code: {weakness.code}")
        return file
