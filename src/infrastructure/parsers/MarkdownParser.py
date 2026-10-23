
import re

class MarkdownParser:
    """Parse Markdown weakness reports into a list of dictionaries."""

    FIELDS = (
        "type",
        "description",
        "severity",
        "justification",
    )

    def parse(self, markdown: str) -> list[dict[str, str]]:
        """Parse a Markdown report and return its weaknesses."""

        if not markdown or not markdown.strip():
            return []

        if re.search(r"^# Weaknesses\s*\[\s*\]\s*$",
                     markdown.strip(), re.DOTALL):
            return []

        # Find each weakness section identified by its CWE ID.
        pattern = r"^##\s+(CWE-\S+)\s*$"
        matches = list(re.finditer(pattern, markdown, re.MULTILINE))

        weaknesses = []

        for index, match in enumerate(matches):
            start = match.end()
            end = (
                matches[index + 1].start()
                if index + 1 < len(matches)
                else len(markdown)
            )

            section = markdown[start:end]

            weakness = {"weakness": match.group(1)}

            for field in self.FIELDS:
                weakness[field] = self._extract_field(section, field)

            weakness["code"] = self._extract_code(section)

            weaknesses.append(weakness)

        return weaknesses

    @staticmethod
    def _extract_field(section: str, field: str) -> str:
        """Extract a field value until the next Markdown subsection."""

        pattern = (
            rf"^###\s+{re.escape(field)}\s*\n"
            r"(.*?)"
            r"(?=^###\s|\Z)"
        )

        match = re.search(
            pattern,
            section,
            re.IGNORECASE | re.MULTILINE | re.DOTALL,
        )

        return match.group(1).strip() if match else ""

    @staticmethod
    def _extract_code(section: str) -> str:
        """Extract the source code from the Code subsection."""

        pattern = (
            r"^###\s+Code\s*\n"
            r"\s*```[^\n]*\n"
            r"(.*?)"
            r"\n[ \t]*```"
        )

        match = re.search(
            pattern,
            section,
            re.IGNORECASE | re.MULTILINE | re.DOTALL,
        )

        return match.group(1) if match else ""
