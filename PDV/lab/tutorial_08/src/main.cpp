#include <iostream>
#include <random>
#include "sparse_matrix.hpp"
#include "multiply.hpp"
#include "../pdv_lib/pdv_lib.hpp"

constexpr double NONZERO_ROW_PROBABILITY = 0.75;
constexpr double NONZERO_CELL_PROBABILITY = 0.05;

constexpr size_t MATRIX_ROWS = 200000;
constexpr size_t MATRIX_COLUMNS = 10000;

pdv::uniform_random<double> probability_dist{0.0, 1.0};
pdv::uniform_random<double> entry_value_dist{0.0, 5.0};

void fill_random_sparse_vector(sparse_vector& vec) {
    // a bit faster (~30%) alternative would be to generate a list of unique random indices,
    //  since NONZERO_CELL_PROBABILITY is quite low
    for (size_t i = 0; i < MATRIX_COLUMNS; i++) {
        if (probability_dist() <= NONZERO_CELL_PROBABILITY) {
            vec.set(i, entry_value_dist());
        }
    }
}

void fill_random_sparse_matrix(sparse_matrix& matrix) {
    for (size_t i = 0; i < MATRIX_ROWS; i++) {
        if (probability_dist() <= NONZERO_ROW_PROBABILITY) {
            matrix_row& row = matrix.emplace_back(i);
            fill_random_sparse_vector(row);
        }
    }
}

int main() {
    sparse_matrix A{};
    sparse_vector x{};

    // Nejprve vygenerujeme nahodny obsah matice A a vektoru x
    std::cout << "Generating random test data...\n";
    fill_random_sparse_matrix(A);
    fill_random_sparse_vector(x);

    // A otestujeme rychlost sekvencni a paralelni implementace
    sparse_vector sequential_result;
    pdv::benchmark("Sequential computation of A*x", [&] {
        sequential_result = multiply_sequential(A, x);
    });

    sparse_vector parallel_result;
    pdv::benchmark("Parallel computation of A*x", [&] {
        parallel_result = multiply_parallel(A, x);
    });

    if (sequential_result != parallel_result) {
        std::cerr << "V paralelni verzi je pravdepodobne chyba, "
                     "vysledek se neshoduje s vysledkem sekvencni verze!\n";
    }

    return 0;
}
