'''
alessatool/merge:

merge objdiff units. this is helper script that takes a list of objdiff.json
files from `alessatool generate` and combines them into one.

see `alessatool merge --help` for more information.
'''

from pathlib import Path
from dataclasses import dataclass
from json import load, dumps

from utils import ensure_path_and_write

@dataclass
class MergeArgs:
    mode: str
    objdiff_fragments: list[Path]
    categories_path: Path
    objdiff_output_path: Path
    d_path: Path
    verbose: bool
    project: Path

def merge_objdiff_units(args: MergeArgs):
    units = []

    for path in args.objdiff_fragments:
        fragment = open(path, "r")
        new_units = load(fragment)
        fragment.close()

        assert isinstance(new_units, list), f"expected 'list', got '{type(new_units)}'"
        units.extend(new_units)

    units.sort(key=lambda unit: unit["name"])

    progress_categories = None
    if args.categories_path is not None:
        with open(args.categories_path, "r") as progress_categories_json:
            progress_categories = load(progress_categories_json)

    result = dumps({
        "$schema": "https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json",
        "build_base": True,
        "build_target": False,
        "custom_args": [f"PROJECT={args.project}", "NON_MATCHING=1"],
        "watch_patterns": [
            "*.c",
            "*.h"
        ],
        "progress_categories": progress_categories,
        "units": units,
    })

    ensure_path_and_write(args.objdiff_output_path, result)

    if args.verbose:
        print(f"🟣 alessatool/merge: wrote objdiff.json")

def merge_fragments(args: MergeArgs):
    match args.mode:
        case "objdiff":
            merge_objdiff_units(args)

        case _:
            raise Exception(f"unknown merge mode {args.mode}")
