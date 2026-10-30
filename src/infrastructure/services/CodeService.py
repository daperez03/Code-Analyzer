from domain.constants import TREE_SITTER_FUNCTION_TYPES, TREE_SITTER_CLASS_TYPES
from domain.constants import TREE_SITTER_IMPORT_TYPES, TREE_SITTER_GLOBAL_TYPES
from domain.constants import TREE_SITTER_SUPPORTED_EXTENSIONS
from tree_sitter_language_pack import get_parser
from domain.services import ICodeService
from domain.entities import File
import re


class CodeService(ICodeService):

    def remove_comments(self, file: File) -> File:
        language = TREE_SITTER_SUPPORTED_EXTENSIONS.get(file.extension)

        if language is None:
            raise ValueError(f"Unsupported extension: {file.extension}")

        parser = get_parser(language)
        source_bytes = file.content.encode("utf-8")
        tree = parser.parse(source_bytes)

        ranges = []

        def visit(node):
            if node.type in {"comment", "line_comment", "block_comment"}:
                ranges.append((node.start_byte, node.end_byte))

            for child in node.children:
                visit(child)

        visit(tree.root_node)

        result = bytearray(source_bytes)

        for start, end in ranges:
            for i in range(start, end):
                if result[i] not in (10, 13):  # Preserve LF and CR
                    result[i] = 32

        file.content = result.decode("utf-8")
        return file


    def extract_code_units(self, file: File) -> list[str]:
        language = TREE_SITTER_SUPPORTED_EXTENSIONS.get(file.extension)
        if language is None:
            raise ValueError(f"Unsupported extension: {file.extension}")

        code = file.content.encode("utf-8")
        parser = get_parser(language)
        tree = parser.parse(code)

        if tree is None:
            raise ValueError(f"Could not parse file: {file.path}")

        root = tree.root_node
        function_types = TREE_SITTER_FUNCTION_TYPES.get(language, set())
        class_types = TREE_SITTER_CLASS_TYPES.get(language, set())

        units = []

        def get_text(node):
            return code[node.start_byte:node.end_byte].decode("utf-8")

        for node in root.named_children:
            if node.type in function_types or node.type in class_types:
                units.append(node)

        results = []

        for node in units:
            unit = get_text(node)
            results.append(unit)
        return results

    def extract_imports(self, file: File) -> list[str]:
        language = TREE_SITTER_SUPPORTED_EXTENSIONS.get(file.extension)
        if language is None:
            raise ValueError(f"Unsupported extension: {file.extension}")

        code = file.content.encode("utf-8")
        parser = get_parser(language)
        tree = parser.parse(code)

        if tree is None:
            raise ValueError(f"Could not parse file: {file.path}")

        root = tree.root_node
        imports = []

        def get_text(node):
            return code[node.start_byte:node.end_byte].decode("utf-8")

        for node in root.named_children:
            if node.type in TREE_SITTER_IMPORT_TYPES:
                imports.append(get_text(node))

        return imports

    def extract_global_variables(self, file: File) -> list[str]:
        language = TREE_SITTER_SUPPORTED_EXTENSIONS.get(file.extension)
        if language is None:
            raise ValueError(f"Unsupported extension: {file.extension}")

        code = file.content.encode("utf-8")
        parser = get_parser(language)
        tree = parser.parse(code)

        if tree is None:
            raise ValueError(f"Could not parse file: {file.path}")

        root = tree.root_node
        globals_ = []

        def get_text(node):
            return code[node.start_byte:node.end_byte].decode("utf-8")

        for node in root.named_children:
            if node.type in TREE_SITTER_GLOBAL_TYPES:
                globals_.append(get_text(node))

        return globals_