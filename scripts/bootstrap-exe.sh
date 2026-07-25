#! /usr/bin/bash

help() {
    echo "bootstrap-exe.sh: bootstrap-exe.sh <destination> <project name>"
    echo 
    echo "Arguments:"
    echo "    <destination>               The destination directory to create"
    echo "    <name>                      The project name"
}

error() {
    echo $1 >&2;
    help;
    exit 1;
}

declare -A defines;
REST=()

for f in "$@"; do
    case $f in
        *)
            REST+=($f)
            ;;
    esac
done

if (( ${#REST[@]} != 2 )); then
    error "Expected exactly 2 positional arguments. Received ${#REST[@]}";
fi

PROJECT_ROOT=$(git rev-parse --show-toplevel)

DESTINATION=${REST[0]}
NAME=${REST[1]}

"$(dirname -- "${BASH_SOURCE[0]}")/setup-folder.sh" "$PROJECT_ROOT/template/executable" "$DESTINATION" -d "_PROJECT_=$NAME"
