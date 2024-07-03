#!/usr/bin/env bash

return_value=0

# Check -h option
if [ "$1" = "-h" ]; then
    printf "path [OPTION]\n-h get help message.\n-a output all .pdf files in the current directory.\n-b read from stdin and print lines what starts with number, but without this number\n-c split text from stdin for sentences.\n-d <prefix> change libraries in C/C++ file from stdin.\n\t #include \"lib\" ---> #include \"<prefix>lib\"\n" 
elif [ "$1" = "-a" ]; then
    ls -a | grep -Ei '\.pdf$'
elif [ "$1" = "-b" ]; then
    input=""
    while read -r input
    do
        line=$(echo "$input" | grep -E '^(-|\+){0,1}[0-9]+')
        match=$(echo "$input" | grep -Eo '^(-|\+){0,1}[0-9]+')
        if test -n "$line"; then
            echo "${line//${match}/}"
        fi
    done
elif [ "$1" = "-c" ]; then
    input=""
    text=""
    IFS=''

    while read -r input
    do
        text="$text$input\n"
    done
    sentences=( $(printf "$text" | tr '\n' ' ' | grep -Po "[A-Z\p{Lu}]+[^\.\?\!]+[\.|\!|\?]+") )
    echo "${sentences[*]}"
    
elif [ "$1" = "-d" ] && test -n "$2"; then

    text=""
    
    while IFS='' read -r input
    do
        text="$text$input\n"
    done

    IFS_OLD=$IFS
    IFS=$'\n'

    # list with #include lines
    includes=( $(printf "$text" | grep -E '.*#[[:space:]]*include[[:space:]]*[<"][^>"]*[>"]+.*') )
    # list with prefixes of #include lines
    preincludes=( $(printf "$text" | grep -Eo '.*#[[:space:]]*include[[:space:]]*[<"]+') ) 

    IFS=$IFS_OLD

    elements_amount="${#includes[*]}"
    (( elements_amount-- ))

    for i in $(seq 0 "${elements_amount}")
    do
        cur_include="${includes[${i}]}"
        cur_preinclude="${preincludes[${i}]}"
        
        # body + suffix
        body_and_postinclude="${cur_include//${cur_preinclude}/}"
        # suffix
        cur_postinclude="$(echo "${body_and_postinclude}" | grep -Eo '(>|"){1,1}.*')"

        #checking if right <> and "" (continue when >lib< or <lib")
        if [ "${cur_preinclude: -1}" = "<" ] && [ "${cur_postinclude:0:1}" = ">" ]; then
            true
        elif [ "${cur_preinclude: -1}" = "\"" ] && [ "\"" = "${cur_postinclude:0:1}" ]; then
            true
        else
            continue
        fi
        
        # line without prefix
        body="${body_and_postinclude%"$cur_postinclude"}"
        # line withous suffix (body)
        body="${body_and_postinclude//${cur_postinclude}/}"
        # new body with prefix, library_prefix, body, suffix
        new_line="$cur_preinclude$2$body$cur_postinclude"

        # replace in text cur_inclue to new_line
        text="${text//"${cur_include}"/"${new_line}"}"


    done
    printf "$text"

else
    return_value=1
fi

exit $return_value

