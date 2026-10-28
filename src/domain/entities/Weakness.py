from domain.constants import EVALUATOR_TYPES, EVALUATOR_CWES, EVALUATOR_SEVERITIES
from pydantic import BaseModel

class Weakness(BaseModel):
    type: str
    weakness: str
    description: str
    severity: str
    code: str
    justification: str

    def __init__(
        self, 
        type: str, 
        weakness: str, 
        description: str, 
        severity: str, 
        code: str, 
        justification: str):
        selected_type = next((t for t in EVALUATOR_TYPES if t in type.capitalize()), None)
        selected_severity = next((s for s in EVALUATOR_SEVERITIES if s in severity.capitalize()), None)
        if not selected_type:
            raise ValueError(f"Type {type} is not valid.")
        # if weakness not in EVALUATOR_CWES:
        #     raise ValueError(f"Weakness {weakness} is not valid.")
        if not selected_severity:
            raise ValueError(f"Severity {severity} is not valid.")
        if not code:
            raise ValueError("Code cannot be empty.")
        if not justification:
            raise ValueError("Justification cannot be empty.")
        super().__init__(
            type=type, 
            weakness=weakness, 
            description=description, 
            severity=severity, 
            code=code, 
            justification=justification)
        
    def __str__(self):
        weakness_str = f"Type: {self.type}\n"
        weakness_str += f"Weakness: {self.weakness}\n"
        weakness_str += f"Description: {self.description}\n"
        weakness_str += f"Severity: {self.severity}\n"
        weakness_str += f"Code: {self.code}\n"
        weakness_str += f"Justification: {self.justification}\n"
        return weakness_str