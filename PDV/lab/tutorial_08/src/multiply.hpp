#pragma once

#include "sparse_matrix.hpp"

sparse_vector multiply_sequential(const sparse_matrix& A, const sparse_vector& x);
sparse_vector multiply_parallel(const sparse_matrix& A, const sparse_vector& x);
