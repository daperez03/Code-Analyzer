
import os

LLM_MODEL = os.getenv("LLM_MODEL")
LLM_TEMPERATURE = float(os.getenv("LLM_TEMPERATURE"))
LLM_MAX_TOKENS = int(os.getenv("LLM_MAX_TOKENS"))
LLM_TOP_P = float(os.getenv("LLM_TOP_P"))
LLM_DO_SAMPLE = bool(int(os.getenv("LLM_DO_SAMPLE")))
LLM_PARAMETERS = {
    "max_new_tokens": LLM_MAX_TOKENS,
    "temperature": LLM_TEMPERATURE,
    "top_p": LLM_TOP_P,
    "do_sample": LLM_DO_SAMPLE,
}

LOCAL_LLM_SUPPORTED_MODELS = [
    "Qwen2.5-0.5B-Instruct",
    "Qwen3-0.6B",
    "Qwen3-8B"
]

GOOGLE_LLM_SUPPORTED_MODELS = [
    "gemini-3.8-flash"
]