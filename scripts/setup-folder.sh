#! /usr/bin/bash

help() {
    echo "setup-folder.sh: setup-folder.sh <source> <destination> [-d|--define <name>=<value>]"
    echo 
    echo "Arguments:"
    echo "    <source>                    The source template directory"
    echo "    <destination>               The destination directory to create"
    echo 
    echo "    -d|--define <key>=<value>   Define a replacement"
}

error() {
    echo $1 >&2;
    help;
    exit 1;
}

countDir() {
    find $1 -mindepth 1 -maxdepth 1 | wc -l
}

declare -A defines;
REST=()

for f in "$@"; do
    if [[ $READ_DEFINE ]]; then
        KEY=${f%=*}
        VALUE=${f#*=}

        if [[ -v defines[$KEY] ]]; then
            error "Replacement for '$KEY' was already defined";
        fi

        defines["${KEY}_UP_"]=${VALUE^^};
        defines["${KEY}_DOWN_"]=${VALUE,,};
        defines["$KEY"]=$VALUE;

        READ_DEFINE=
        continue
    fi

    case $f in
        "-d" | "--define")
            READ_DEFINE=1
            ;;

        *)
            REST+=($f)
            ;;
    esac
done

if (( ${#REST[@]} != 2 )); then
    error "Expected exactly 2 positional arguments. Received ${#REST[@]}";
fi

SOURCE=${REST[0]}
DESTINATION=${REST[1]}

PROGRAM=$(for f in "${!defines[@]}"; do
    echo -n "s/${f}/${defines[$f]}/g;"
done)

if [[ -d $DESTINATION ]] && (( $(countDir $DESTINATION) > 0 )); then
    error "Output directory '$DESTINATION' already exists, and is not empty. Remove the directory first."
fi

doSubdir() {
    local SOURCE=$1
    local DESTINATION=$2

    mkdir -p $DESTINATION;

    local FILES=$(find $SOURCE -mindepth 1 -maxdepth 1 -type f -printf "%f\n")
    local DIRS=$(find $SOURCE -mindepth 1 -maxdepth 1 -type d -printf "%f\n")

    
    echo "$FILES" | while IFS= read line; do
        if [[ "$line" == "" ]]; then continue; fi

        cat "$SOURCE/$line" | sed "$PROGRAM" > "$DESTINATION/$(echo -n "$line" | sed "$PROGRAM")"
    done

    echo "$DIRS" | while IFS= read line; do
        if [[ "$line" == "" ]]; then continue; fi

        doSubdir "$SOURCE/$line" "$DESTINATION/$(echo -n "$line" | sed "$PROGRAM")";
    done
}

doSubdir $SOURCE $DESTINATION
