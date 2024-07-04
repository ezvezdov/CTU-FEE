#ifndef SORTING_MERGESORT_PARALLEL_H
#define SORTING_MERGESORT_PARALLEL_H

#include "mergesort.h"
#include "mergesort_sequential.h"

template <typename elem_t>
void mergesort_parallel_worker(elem_t* data, unsigned long size, elem_t* tmp, unsigned long p_size);


template <typename elem_t>
void mergesort_parallel(std::vector<elem_t> & data) {
    const size_t size = data.size();
    std::vector<elem_t> tmp(size);

    // Doimplementujte paralelni verzi algoritmu mergesort za pouziti 'task'
    // v OpenMP. Muzete se inspirovat sekvencni verzi algoritmu v souboru
    // 'mergesort_sequential.h' a muzete take pouzit sekvencni metody 'merge'
    // pro sliti dvou serazenych poli do jednoho (implementovanou v souboru
    // 'mergesort.h').

#pragma omp parallel
#pragma omp single
    mergesort_parallel_worker(&data[0],size,&tmp[0],10000);
}

template <typename elem_t>
void mergesort_parallel_worker(elem_t* data, unsigned long size, elem_t* tmp, unsigned long p_size){
    // Pole velikosti mensi nez 1 je serazene
    if (size <= 1) {
        return;
    }



    size_t h_size = size / 2;

    // Nejprve si pole r    ozdelime na dve casti [a .. b-1] a [b .. end-1]
    elem_t* a = data;             // Pointer na zacatek segmentu
    elem_t* b = data + h_size;    // Pointer doprostred segmentu (konec leve poloviny)
    elem_t* end = data + size;    // Konec segmentu (konec prave poloviny)
    // Stejnym zpusobem si rozdelime i pomocne pole
    elem_t* tmp_a = tmp;
    elem_t* tmp_b = tmp + h_size;

    // Zavolame rekurzivni volani na levou a pravou polovinu
    if(size <= p_size){
        mergesort_sequential_worker(a,static_cast<unsigned long>(h_size), tmp_a);
        mergesort_sequential_worker(b,static_cast<unsigned long>(size - h_size), tmp_b);
    }
    else{
#pragma omp task
        mergesort_parallel_worker(a, static_cast<unsigned long>(h_size), tmp_a,p_size);
#pragma omp task
        mergesort_parallel_worker(b, static_cast<unsigned long>(size - h_size), tmp_b,p_size);
#pragma omp taskwait
    }


    merge(a, b, end, tmp);
}





#endif //SORTING_MERGESORT_PARALLEL_H

