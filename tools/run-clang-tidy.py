#!/bin/python

import sys
import subprocess
from pathlib import Path


SOURCE_FILTER='src/**/*.[h|c]pp'
HEADER_FILTER='.*;-.*gen.*'


def print_help():
    print(
        'Run clang-tidy with some default options from specific working'
            ' directory\n\n'
        'USAGE: run-clang-tidy.py [working_dir]\n'
        '\n'
        'OPTIONS:\n'
        'working_dir\t- The directory from which run clang-tidy (default: ../)'
    )


def main():
    if len(sys.argv) < 2:
        working_dir = Path('../')
        extra_args_index = 1
    else:
        if sys.argv[1] == '-h' or sys.argv[1] == '--help':
            print_help()
            return
        working_dir = Path(sys.argv[1])
        extra_args_index = 2

    args = get_default_args()
    args += sys.argv[extra_args_index:]

    print(f'Arguments: "{" ".join(args)}"')

    args += load_sources(working_dir)

    print(f'Run clang-tidy...')
    res = subprocess.run(args, stdout=sys.stdout.buffer,
            stderr=sys.stdout.buffer, cwd=working_dir)
    print(f'clang-tidy exited with {res.returncode}')


def get_default_args():
    args = []
    args.append('/bin/clang-tidy')
    args.append(f"--header-filter='{HEADER_FILTER}'")
    args.append('-p=build/')
    return args


def load_sources(working_dir):
    srcs = []
    for src_path in working_dir.glob(SOURCE_FILTER):
        print(f'Found file: {src_path.relative_to(working_dir)}')
        src_abs_path = src_path.resolve()
        srcs.append(str(src_abs_path))
    return srcs


if __name__ == '__main__':
    main()

