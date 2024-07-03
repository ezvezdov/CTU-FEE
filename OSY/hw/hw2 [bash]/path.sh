#!/usr/bin/env bash

return_value=0

# Check -h option
if [ "$1" = "-h" ] || [ "$2" = "-h" ] || [ "$3" = "-h" ]; then
    echo "This script check existence of file/directory/symlink. Type path in format PATH /path/to/file. Option -h to get help. Option -z to tar files."
    exit $return_value
fi

tarball=false

# Check -z option
if [ "$1" = "-z" ]; then
    tarball=true
fi

# Check wrong arguments option
if test -n "$1" && [ "$1" != "-z" ]; then
    exit 2
fi

input=""
PATH_STRING="PATH "
files_to_tar=()

while true
do
    # Break if end of file (ctrl + D)
    if ! read -r input; then
        break
    fi

    # Continue if isn't "PATH" on string start
    if [ "${input#$PATH_STRING}" = "$input" ]; then
        continue
    fi
    
    # Cut off "PATH" string
    path=${input#$PATH_STRING}

    # Check is path to symlink
    link_value=$(readlink "$path")
    if test -n "$link_value"; then
        echo "LINK '$path' '$link_value'"
        continue
    fi

    # Check is path to directory
    if [ -d "$path" ]; then
        echo "DIR '$path'"
        continue
    fi

    # Check is path to file
    if [ -f "$path" ]; then
        lines_amount=$(sed -n '$=' "$path") || exit 2
        if test -z "$lines_amount"; then
            lines_amount=0
        fi
        line1=$(head -1 "$path")
        echo "FILE '$path' $lines_amount '$line1'"
        files_to_tar+=("$path")
        continue
    fi

    # Output to stderr
    echo "ERROR '$path'" >&2
    return_value=1
done

# Tar all if was -z option
if [ $tarball = true ] && [ "${#files_to_tar[*]}" != "0" ]; then
    tar czf output.tgz "${files_to_tar[@]}" || exit 2
fi

exit $return_value
