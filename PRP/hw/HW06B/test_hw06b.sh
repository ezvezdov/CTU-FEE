for i in 1 2 3 4 5 6 7 8 9
do
    ./hw06b-b0b36prp < data/pub0$i.in > tests/pub0$i.out
    diff data/pub0$i.out  tests/pub0$i.out
done
    ./hw06b-b0b36prp < data/pub10.in > tests/pub10.out
    diff data/pub10.out  tests/pub10.out

for i in 1 2 3 4 5 6 7 8 9 10
do
    ./hw06b-b0b36prp < files/hw06b-$i.in > tests/hw06b-$i.out
    diff files/hw06b-$i.out tests/hw06b-$i.out
done

