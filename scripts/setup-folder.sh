#! /usr/bin/bash

help() {
    echo "setup-folder.sh: setup-folder.sh <source> <destination> [-d|--define <name>=<value>]"
    echo 
    echo "Arguments:"
    echo "    <source>                    The source template directory"
    echo "    <destination>               The destination directory to create"
    echo "    <destination>               The destination directory to create"
    echo 
    echo "    -d|--define <key>=<value>   Define a replacement"
}

error() {
    echo $1 >&2;
    help;
    exit 1;
}

declare -A defines;
declare -A defines;

REST=()

for f in "$@"; do
    if [[ $READ_DEFINE ]]; then
        KEY=${f%=*}
        VALUE=${f#*=}

        echo $KEY $VALUE

        defines[$KEY]=$VALUE;

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

echo "Defines: $SOURCE $DESTINATION"
for k in "${!defines[@]}"; do
    echo "    $k: ${defines[$k]}"
done
