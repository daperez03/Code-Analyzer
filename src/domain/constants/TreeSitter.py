
TREE_SITTER_FUNCTION_TYPES = {
    "python": {"function_definition"},
    "javascript": {"function_declaration", "generator_function_declaration"},
    "typescript": {"function_declaration", "generator_function_declaration"},
    "java": {"method_declaration", "constructor_declaration"},
    "go": {"function_declaration", "method_declaration"},
    "rust": {"function_item"},
    "c": {"function_definition"},
    "cpp": {"function_definition"},
    "c_sharp": {"method_declaration", "constructor_declaration"},
    "php": {"function_definition", "method_declaration"},
}

TREE_SITTER_CLASS_TYPES = {
    "python": {"class_definition"},
    "javascript": {"class_declaration"},
    "typescript": {"class_declaration", "abstract_class_declaration"},
    "java": {"class_declaration", "interface_declaration", "enum_declaration"},
    "go": set(),
    "rust": {"struct_item", "enum_item", "trait_item", "impl_item"},
    "c": set(),
    "cpp": {"class_specifier", "struct_specifier"},
    "c_sharp": {"class_declaration", "struct_declaration", "interface_declaration"},
    "php": {"class_declaration", "interface_declaration"},
}
TREE_SITTER_IMPORT_TYPES = {
    "import_statement",
    "import_from_statement",
    "import_declaration",
    "preproc_include",
    "using_directive",
}

TREE_SITTER_GLOBAL_TYPES = {
    "declaration",
    "lexical_declaration",
    "variable_declaration",
    "const_declaration",
    "assignment",
    "expression_statement",
}

TREE_SITTER_SUPPORTED_EXTENSIONS = {
    "py": "python",
    "php": "php",
    "c": "c",
    "h": "c",
    "cpp": "cpp",
    "hpp": "cpp",
    "cc": "cpp",
    "cxx": "cpp",
    "java": "java",
}