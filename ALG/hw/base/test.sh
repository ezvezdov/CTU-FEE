#!/bin/bash

# make program
make

# make directory if it doesn't exist
mkdir -p datapub_output

for ((i=1;i<=10;i++));
do
    if [ $i -lt 10 ]; then
        ./main < datapub/pub0$i.in > datapub_output/pub0$i.out
        diff datapub/pub0$i.out  datapub_output/pub0$i.out
    elif [ $i -ge 10 ]; then
        ./main < datapub/pub$i.in > datapub_output/pub$i.out
        diff datapub/pub$i.out  datapub_output/pub$i.out
    fi
done
