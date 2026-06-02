#!/usr/bin/env python3
"""
twistedgen.py  -  Pre-build code generator for Twisted Engine reflection.

Usage
-----
  # Generate for a single header (build-time incremental)
  python twistedgen.py file <header.h> <out_dir> <include_root>

  # Scan a whole include tree (configure-time full pass)
  python twistedgen.py scan <include_dir> <out_dir> <cmake_out> <include_root>
"""

import re
import sys
import argparse
from pathlib import Path
from dataclasses import dataclass, field
from typing import List, Optional, Tuple

# ──────────────────────────────────────────────────────────────
# Configuration tables
# ──────────────────────────────────────────────────────────────

BASE_KIND_MAP = {
    "AComponent":  "Component",
    "SystemBase":  "System",
    "ManagerBase": "Manager",
}

REGISTER_METHOD = {
    "Component": "RegisterComponent",
    "System":    "RegisterSystem",
    "Manager":   "RegisterManager",
}

# Maps C++ type names to Twisted::PropertyType enum values
PROPERTY_TYPE_MAP = {
    "float":       "Twisted::PropertyType::Float",
    "int":         "Twisted::PropertyType::Int",
    "bool":        "Twisted::PropertyType::Bool",
    "std::string": "Twisted::PropertyType::String",
    "Vec2f":       "Twisted::PropertyType::Vec2f",
    "Vec3f":       "Twisted::PropertyType::Vec3f",
    "Vec4f":       "Twisted::PropertyType::Vec4f",
    "Quat":        "Twisted::PropertyType::Quat",
}


# ──────────────────────────────────────────────────────────────
# Data
# ──────────────────────────────────────────────────────────────

@dataclass
class PropertyDef:
    type_name:    str            # e.g. "float", "Vec3f", "EntityID"
    field_name:   str            # e.g. "rotationSpeed"
    display_name: str            # e.g. "Rotation Speed"
    prop_type:    Optional[str]  # None for types not in PROPERTY_TYPE_MAP (YAML-only, no editor widget)


@dataclass
class ClassInfo:
    namespace:  str
    class_name: str
    kind:       str   # "Component" | "System" | "Manager"
    properties: List[PropertyDef] = field(default_factory=list)

    @property
    def full_name(self) -> str:
        return f"{self.namespace}::{self.class_name}" if self.namespace else self.class_name


# ──────────────────────────────────────────────────────────────
# Parsing helpers
# ──────────────────────────────────────────────────────────────

def strip_comments(src: str) -> str:
    """Remove // and /* */ comments, preserving newlines."""
    result = []
    i, n = 0, len(src)
    while i < n:
        if src[i:i+2] == "//":
            while i < n and src[i] != "\n":
                i += 1
        elif src[i:i+2] == "/*":
            i += 2
            while i < n:
                if src[i:i+2] == "*/":
                    i += 2
                    break
                if src[i] == "\n":
                    result.append("\n")
                i += 1
        else:
            result.append(src[i])
            i += 1
    return "".join(result)


def get_namespace_at(src: str, pos: int) -> str:
    """Return the '::'-joined namespace stack active at position pos."""
    ns_parts: List[str] = []
    scope_stack: List[Tuple[str, int]] = []

    i = 0
    while i < pos:
        c = src[i]
        if c == "{":
            look = src[max(0, i - 300): i]
            m = re.search(r"\bnamespace\b\s*([\w:]*)\s*$", look)
            if m:
                raw = m.group(1).strip()
                parts = [p for p in raw.split("::") if p]
                ns_parts.extend(parts)
                scope_stack.append(("ns", len(parts)))
            else:
                scope_stack.append(("other", 0))
        elif c == "}":
            if scope_stack:
                kind, count = scope_stack.pop()
                if kind == "ns" and count:
                    ns_parts = ns_parts[:-count]
        i += 1

    return "::".join(ns_parts)


def find_class_body(src: str, body_start: int) -> str:
    """Return the text between the opening { (at body_start) and its matching }."""
    depth = 1
    i = body_start
    n = len(src)
    while i < n and depth > 0:
        c = src[i]
        if c == "{":
            depth += 1
        elif c == "}":
            depth -= 1
        i += 1
    return src[body_start:i - 1]


def find_opening_brace_before(src: str, pos: int) -> Optional[int]:
    """Scan backward from pos to find the { that encloses pos."""
    depth = 0
    i = pos - 1
    while i >= 0:
        if src[i] == "}":
            depth += 1
        elif src[i] == "{":
            if depth == 0:
                return i
            depth -= 1
        i -= 1
    return None


def camel_to_display(name: str) -> str:
    """Convert camelCase or PascalCase to 'Title Case' for display names."""
    s = re.sub(r"([A-Z])", r" \1", name)
    return s.strip().title()


# Matches TPROPERTY() (zero args) followed by a C++ field declaration:
#   TPROPERTY()
#   float rotationSpeed;
#   TPROPERTY()
#   std::string Name;
_TPROPERTY_FIELD_RE = re.compile(
    r"\bTPROPERTY\s*\(\s*\)"        # TPROPERTY()
    r"\s*"                           # optional whitespace/newlines
    r"([\w:]+(?:<[^>]*>)?)"         # type  (words, ::, optional <...>)
    r"\s+"                           # required whitespace
    r"(\w+)"                         # field name
    r"\s*(?:=[^;]*)?\s*;",           # optional = default, then ;
    re.DOTALL,
)


def find_tproperties(body: str) -> List[PropertyDef]:
    """Return PropertyDef list for every TPROPERTY() + field-decl pair in body."""
    props = []
    for m in _TPROPERTY_FIELD_RE.finditer(body):
        type_name  = m.group(1).strip()
        field_name = m.group(2).strip()
        prop_type  = PROPERTY_TYPE_MAP.get(type_name)  # None for YAML-only types
        props.append(PropertyDef(
            type_name=type_name,
            field_name=field_name,
            display_name=camel_to_display(field_name),
            prop_type=prop_type,
        ))
    return props


def find_annotated_classes(src: str) -> List[ClassInfo]:
    """Return every class/struct that contains TCLASS_BODY() in its body."""
    classes = []

    for body_m in re.finditer(r"\bTCLASS_BODY\s*\(\s*\)", src):
        body_pos = body_m.start()
        ns = get_namespace_at(src, body_pos)

        # Find the { that encloses TCLASS_BODY() — that is the class body's opening brace
        opening = find_opening_brace_before(src, body_pos)
        if opening is None:
            continue

        # The text before that { contains the class/struct declaration
        before = src[max(0, opening - 500): opening]
        cls_decl_m = re.search(r"\b(?:class|struct)\b([^{;]*)$", before)
        if not cls_decl_m:
            continue

        decl_text = cls_decl_m.group(1).strip()

        # Split on ':' to separate "optional_qualifiers ClassName" from inheritance
        if ":" in decl_text:
            name_part, inh_part = decl_text.split(":", 1)
        else:
            name_part, inh_part = decl_text, ""

        # Class name is the last identifier before the colon (or end of decl)
        name_m = re.search(r"(\w+)\s*$", name_part)
        if not name_m:
            continue
        class_name = name_m.group(1)

        # Parse base classes from the inheritance clause
        bases: List[str] = []
        for part in inh_part.split(","):
            part = re.sub(r"\b(?:public|protected|private|virtual)\b", "", part).strip()
            base = part.split("::")[-1].strip()
            if base:
                bases.append(base)

        kind = next((BASE_KIND_MAP[b] for b in bases if b in BASE_KIND_MAP), None)
        if kind is None:
            print(
                f"[TwistedGen] WARNING: {class_name} - unknown base class, skipping",
                file=sys.stderr,
            )
            continue

        # Class body starts one character after the opening {
        body = find_class_body(src, opening + 1)
        properties = find_tproperties(body)

        classes.append(ClassInfo(
            namespace=ns,
            class_name=class_name,
            kind=kind,
            properties=properties,
        ))

    return classes


# ──────────────────────────────────────────────────────────────
# Code generation
# ──────────────────────────────────────────────────────────────

def compute_include_path(header: Path, include_root: Path) -> str:
    try:
        return header.relative_to(include_root).as_posix()
    except ValueError:
        return header.name




def generate_gen_cpp(classes: List[ClassInfo], include_path: str) -> str:
    lines = [
        "// AUTO-GENERATED by twistedgen.py -- DO NOT EDIT",
        f"// Source: {include_path}",
        "",
        f'#include "{include_path}"',
        '#include "WorldRegistry.h"',
        "",
        "// dllexport only when compiling into the engine DLL; empty for consumer-side gen files",
        "#ifdef TWISTED_ENGINE",
        "#  ifdef _WIN32",
        "#    define TWISTED_GEN_EXPORT __declspec(dllexport)",
        "#  else",
        "#    define TWISTED_GEN_EXPORT __attribute__((visibility(\"default\")))",
        "#  endif",
        "#else",
        "#  define TWISTED_GEN_EXPORT",
        "#endif",
        "",
    ]

    for c in classes:
        reg_fn = REGISTER_METHOD[c.kind]
        name   = c.class_name
        full   = c.full_name

        lines.append(f"// -- {full} " + "-" * max(0, 55 - len(full)))
        lines.append("")

        # GetTypeName specialization — TWISTED_GEN_EXPORT = dllexport on DLL side, empty on consumer side
        lines += [
            "template<>",
            f'TWISTED_GEN_EXPORT const char* GetTypeName<{full}>() {{ return "{c.class_name}"; }}',
            "",
        ]

        # YAML specializations — generated for any class that has TPROPERTY fields
        if c.properties:
            lines += [
                "namespace Twisted {",
                "    template<>",
                f"    YAML::Node YamlSerialize<{full}>(const {full}& obj) {{",
                "        YAML::Node node;",
            ]
            for p in c.properties:
                lines.append(f'        node["{p.field_name}"] = obj.{p.field_name};')
            lines += [
                "        return node;",
                "    }",
                "",
                "    template<>",
                f"    void YamlDeserialize<{full}>({full}& obj, const YAML::Node& node) {{",
            ]
            for p in c.properties:
                lines.append(
                    f'        if (auto _n = node["{p.field_name}"]; _n.IsDefined())'
                    f' obj.{p.field_name} = _n.as<{p.type_name}>();'
                )
            lines += [
                "    }",
                "}",
                "",
            ]

        # Anonymous-namespace static: PropertyInfo registration + WorldRegistry registration
        editor_props = [p for p in c.properties if p.prop_type is not None]
        lines += [
            "namespace {",
            f"    struct {name}_GenReg {{",
            f"        {name}_GenReg() {{",
        ]

        if editor_props:
            lines.append(f"            auto& props = Twisted::GetTypeProperties<{full}>();")
            for p in editor_props:
                accessor = (
                    f"[](void* obj) -> void* "
                    f"{{ return &static_cast<{full}*>(obj)->{p.field_name}; }}"
                )
                lines.append(
                    f'            props.push_back({{ "{p.display_name}", "{p.field_name}", '
                    f"{p.prop_type}, {accessor} }});"
                )

        lines += [
            f"            Twisted::WorldRegistry::GetInstance().{reg_fn}<{full}>();",
            "        }",
            "    };",
            f"    static {name}_GenReg s_{name}_reg;",
            "}",
            "",
        ]

    return "\n".join(lines)


# ──────────────────────────────────────────────────────────────
# File I/O
# ──────────────────────────────────────────────────────────────

def write_if_changed(path: Path, content: str) -> bool:
    if path.exists():
        try:
            if path.read_text(encoding="utf-8") == content:
                return False
        except OSError:
            pass
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_text(content, encoding="utf-8")
    return True


# ──────────────────────────────────────────────────────────────
# Modes
# ──────────────────────────────────────────────────────────────

def process_file(
    header: Path, out_dir: Path, include_root: Path
) -> Optional[Path]:
    raw  = header.read_text(encoding="utf-8", errors="replace")
    src  = strip_comments(raw)
    classes = find_annotated_classes(src)

    if not classes:
        return None

    include_path = compute_include_path(header, include_root)
    stem         = header.stem

    cpp_content = generate_gen_cpp(classes, include_path)
    cpp_file    = out_dir / (stem + ".gen.cpp")

    changed = write_if_changed(cpp_file, cpp_content)
    if changed:
        print(f"[TwistedGen] Generated  {cpp_file.name}")
    else:
        print(f"[TwistedGen] Up-to-date {cpp_file.name}")

    return cpp_file


def scan_directory(
    include_dir: Path,
    out_dir:     Path,
    cmake_out:   Path,
    include_root: Path,
) -> None:
    gen_files: List[Path] = []

    for header in sorted(include_dir.rglob("*.h")):
        result = process_file(header, out_dir, include_root)
        if result:
            gen_files.append(result)

    cmake_lines = [
        "# AUTO-GENERATED by twistedgen.py -- DO NOT EDIT",
        "set(TWISTED_GEN_SOURCES",
    ]
    for f in gen_files:
        cmake_lines.append(f'    "{f.as_posix()}"')
    cmake_lines.append(")")

    cmake_content = "\n".join(cmake_lines) + "\n"
    write_if_changed(cmake_out, cmake_content)
    print(f"[TwistedGen] cmake list -> {cmake_out.name}  ({len(gen_files)} file(s))")


# ──────────────────────────────────────────────────────────────
# Entry point
# ──────────────────────────────────────────────────────────────

def main() -> None:
    parser = argparse.ArgumentParser(description="Twisted Engine code generator")
    sub    = parser.add_subparsers(dest="mode", required=True)

    fp = sub.add_parser("file", help="Process a single header")
    fp.add_argument("header",        type=Path)
    fp.add_argument("out_dir",       type=Path)
    fp.add_argument("include_root",  type=Path)

    sp = sub.add_parser("scan", help="Scan an include directory")
    sp.add_argument("include_dir",   type=Path)
    sp.add_argument("out_dir",       type=Path)
    sp.add_argument("cmake_out",     type=Path)
    sp.add_argument("include_root",  type=Path)

    args = parser.parse_args()

    if args.mode == "file":
        process_file(args.header, args.out_dir, args.include_root)
    else:
        scan_directory(args.include_dir, args.out_dir, args.cmake_out, args.include_root)


if __name__ == "__main__":
    main()
