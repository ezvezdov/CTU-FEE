for i in 1 2 3 4 5 6 7 8 9
do
    ./hw06-b0b36prp < data/man/pub0$i-m.in > pub0$i-m.out
    ./hw06-b0b36prp < data/opt/pub0$i-o.in > pub0$i-o.out
    diff data/man/pub0$i-m.out  pub0$i-m.out
    diff data/opt/pub0$i-o.out  pub0$i-o.out
done

for i in 1 2 3 4
do
    ./hw06-b0b36prp < files/hw06-$i.in > hw06-$i.out
    ./hw06-b0b36prp < files/hw06opt-$i.in > hw06opt-$i.out
    diff files/hw06-$i.out hw06-$i.out
    diff files/hw06opt-$i.out hw06opt-$i.out
done
