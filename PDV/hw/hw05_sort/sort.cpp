#include "sort.h"
#include <iostream>

void radix_ompv(std::vector<std::string *> &vector_to_sort, const MappingFunction &mappingFunction,
                unsigned long alphabet_size, unsigned long string_lengths, int current_index);

// implementace vaseho radiciho algoritmu. Detalnejsi popis zadani najdete v "sort.h"
void radix_par(std::vector<std::string *> &vector_to_sort, const MappingFunction &mappingFunction,
               unsigned long alphabet_size, unsigned long string_lengths) {
#pragma omp parallel
#pragma omp single
    radix_ompv(vector_to_sort,mappingFunction,alphabet_size,string_lengths,0);

    // abeceda se nemeni. jednotlive buckety by mely reprezentovat znaky teto abecedy. poradi znaku v abecede
    // dostanete volanim funkce mappingFunction nasledovne: mappingFunction((*p_retezec).at(poradi_znaku))

    // vytvorte si spravnou reprezentaci bucketu, kam budete retezce umistovat

    // pro vetsi jednoduchost uvazujte, ze vsechny retezce maji stejnou delku - string_lengths. nemusite tedy resit
    // zadne krajni pripady

    // na konci metody by melo byt zaruceno, ze vector pointeru - vector_to_sort bude spravne serazeny.
    // pointery budou serazeny podle retezcu, na ktere odkazuji, kdy retezcu jsou serazeny abecedne
}

void radix_ompv(std::vector<std::string *> &vector_to_sort, const MappingFunction &mappingFunction,
                unsigned long alphabet_size, unsigned long string_lengths, int current_index) {

    if(current_index >= string_lengths){
        return;
    }

    int symbol_index;
    std::vector< std::vector<std::string *> > buckets{ alphabet_size };

    for(int j = 0; j < vector_to_sort.size(); j++){
        symbol_index = mappingFunction((*vector_to_sort[j]).at(current_index));
        buckets[symbol_index].push_back(vector_to_sort[j]);
    }

    for(int i = 0; i < alphabet_size; i++) {
        if (buckets[i].size() == 0) {
            continue;
        } else if (buckets[i].size() < 4) {
            radix_ompv(buckets[i], mappingFunction, alphabet_size, string_lengths, current_index + 1);
        } else {
            std::vector<std::string *> *shared_vector = &buckets[i];
#pragma omp task shared( shared_vector )
            radix_ompv(*shared_vector, mappingFunction, alphabet_size, string_lengths, current_index + 1);
        }
    }

#pragma  omp taskwait

    // Merge vectors
    int vts_index = 0;
    for(int j = 0; j < alphabet_size; j++){
        for(int k = 0; k < buckets[j].size();k++){
            vector_to_sort[vts_index] = buckets[j][k];
            vts_index++;
        }
    }
}