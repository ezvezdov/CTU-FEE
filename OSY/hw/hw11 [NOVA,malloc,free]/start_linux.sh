gcc -g -Og -Wall -I./user/lib -o user/malloc_test.linux user/malloc_test.c user/mem_alloc.c
./user/make_all.sh
./user/malloc_test.linux
# make r