#ifndef DATABASEQUERIES_QUERY_H
#define DATABASEQUERIES_QUERY_H

#include <vector>
#include <functional>
#include <atomic>

template<typename row_t>
using predicate_t = std::function<bool(const row_t &)>;



template<typename row_t>
bool is_satisfied_for_all(std::vector<predicate_t<row_t>> predicates, std::vector<row_t> data_table);

template<typename row_t>
bool is_satisfied_for_any(std::vector<predicate_t<row_t>> predicates, std::vector<row_t> data_table);


/*
 * Sequential
 * true = is_satisfied_for_all(...)              980ms
 * true = is_satisfied_for_any(...)             1303ms
 *
 * false = is_satisfied_for_all(...)              957ms
 * false = is_satisfied_for_any(...)             1267ms
 *
 * Parallel
 * true = is_satisfied_for_all(...)              130ms
 * true = is_satisfied_for_any(...)              268ms
 *
 * false = is_satisfied_for_all(...)              185ms
 * false = is_satisfied_for_any(...)              178ms
 */


template<typename row_t>
bool is_satisfied_for_all(std::vector<predicate_t<row_t>> predicates, std::vector<row_t> data_table) {
    // Doimplementujte telo funkce, ktera rozhodne, zda pro VSECHNY dilci dotazy (obsazene ve
    // vektoru 'predicates') existuje alespon jeden zaznam v tabulce (reprezentovane vektorem
    // 'data_table'), pro ktery je dany predikat splneny.

    // Pro inspiraci si prostudujte kod, kterym muzete zjistit, zda prvni dilci dotaz plati,
    // tj., zda existuje alespon jeden radek v tabulce, pro ktery predikat reprezentovany
    // funkci predicates[i] plati:

    bool return_value = true;

#pragma omp parallel
#pragma omp for
    for(int predicate_index = 0; predicate_index < predicates.size(); predicate_index++) {

        // Funkce reprezentujici predikat 'predicate_index' poddotazu
        auto &predicate = predicates[predicate_index];

        //Kazdy radek tabulky je reprezentovany jednim prvkem vektoru 'data_table'.
        // Velikost vektoru tedy odpovida poctu zaznamu
        unsigned int row_count = data_table.size();

        bool is_one_satisfied = false;


        for (unsigned int i = 0; i < row_count; i++) {
            // i-ty radek tabulky
            auto &row = data_table[i];

            // splnuje 'predicate_index' predikat, pokud funkce predicate vrati true
            bool is_satisfied = predicate(row);

            if (is_satisfied) {
                // Nalezli jsme radek, pro ktery je predikat splneny. Dilci poddotaz tedy plati
                is_one_satisfied = true;
                break;
            }
        }

        // Radek, pro ktery by 'predicate_index' predikat platil, jsme nenasli.
        // Dilci dotaz je tedy nepravdivy.
        if(!is_one_satisfied){
            return_value = false;
#pragma omp cancel for
        }
    }

    return return_value;
}

template<typename row_t>
bool is_satisfied_for_any(std::vector<predicate_t<row_t>> predicates, std::vector<row_t> data_table) {
    // Doimplementujte telo funkce, ktera rozhodne, zda je ALESPON JEDEN dilci dotaz pravdivy.
    // To znamena, ze mate zjistit, zda existuje alespon jeden predikat 'p' a jeden zaznam
    // v tabulce 'r' takovy, ze p(r) vraci true.

    // Zamyslete se nad tim, pro ktery druh dotazu je vhodny jaky druh paralelizace. Vas
    // kod optimalizujte na situaci, kdy si myslite, ze navratova hodnota funkce bude true.
    // Je pro Vas dulezitejsi rychle najit splnujici radek pro jeden vybrany predikat, nebo
    // je dulezitejsi zkouset najit takovy radek pro vice predikatu paralelne?

    bool return_value = false;


    for(int predicate_index = 0; !return_value && predicate_index < predicates.size(); predicate_index++) {

        // Funkce reprezentujici predikat 'predicate_index' poddotazu
        auto &predicate = predicates[predicate_index];

        //Kazdy radek tabulky je reprezentovany jednim prvkem vektoru 'data_table'.
        // Velikost vektoru tedy odpovida poctu zaznamu
        unsigned int row_count = data_table.size();

#pragma omp parallel for
        for (unsigned int i = 0; i < row_count; i++) {
            // i-ty radek tabulky
            auto &row = data_table[i];

            // splnuje 'predicate_index' predikat, pokud funkce predicate vrati true
            bool is_satisfied = predicate(row);

#pragma omp cancellation point for
            if (is_satisfied) {
                // Nalezli jsme radek, pro ktery je predikat splneny. Dilci poddotaz tedy plati
                return_value = true;
#pragma omp cancel for
            }
        }
    }

    return return_value;
}


#endif //DATABASEQUERIES_QUERY_H
