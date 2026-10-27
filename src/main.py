from dotenv import load_dotenv
from pathlib import Path
load_dotenv(Path(__file__).resolve().parent.parent / ".env")

from domain.constants import EVALUATOR_USER_PROMPT, EVALUATOR_SYSTEM_PROMPT
from dependencies import benchmark_use_case

benchmark_use_case.execute(
    id_prefix="wordpress",
    runs=10,
    dataset="2015-03-31-wordpress-v2-0",
    models=["Qwen3-8B"] * 10,
    user_prompts=[EVALUATOR_USER_PROMPT] * 10,
    system_prompts=[EVALUATOR_SYSTEM_PROMPT] * 10,
    parameters=[{"temperature": 0.7, "max_new_tokens": 38912}] * 10,
    save_path="results"
)