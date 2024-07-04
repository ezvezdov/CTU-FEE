#include <vector>
#include <iostream>
#include <algorithm>
#include <immintrin.h>
#include "simd/vectors.hpp"
#include "pdv_lib.hpp"

// surprisingly enough, C++ standard before C++20 does not define PI in the stdlib, so we define
//  it ourselves (POSIX does define it, but e.g. Microsoft STL is not POSIX)
constexpr float PI = 3.14159265358979323846;

// Pro ucely porovnani v nasem kodu pouzivame jednoduchou aproximaci
// funkce exp(x) - jak pro skalarni tak vektorizovanou verzi.
inline float exp_scalar(float x) {
    float x_plus_3 = x + 3.0f;
    float x_minus_3 = x - 3.0f;
    return (x_plus_3 * x_plus_3 + 3.0f) / (x_minus_3 * x_minus_3 + 3.0f);
}

// Vektorova implementace funkce 'exp_scalar(...)'
inline __m256 exp_vec(__m256 x) {
    __m256 three = _mm256_set1_ps(3.0f);
    __m256 addthree = _mm256_add_ps(x, three);
    __m256 subthree = _mm256_sub_ps(x, three);

    return _mm256_div_ps(_mm256_add_ps(_mm256_mul_ps(addthree, addthree), three),
                         _mm256_add_ps(_mm256_mul_ps(subthree, subthree), three));
}

// `exp_vec` zabalena v C++ kabatku...
inline __m256 exp_vec_cpp(simd::vec_f32_8 x) {
    using vec = simd::vec_f32_8;
    vec three{3.0f};
    vec x_plus_3 = x + three;
    vec x_minus_3 = x - three;
    return (x_plus_3 * x_plus_3 + three) / (x_minus_3 * x_minus_3 + three);
}


// Skalarni implementace funkce, ktera vypocte hodnotu hustotni funkce
// normalniho rozdeleni (s parametry mu a sigma) nad polem dat 'data'.
void normaldist_scalar(float mu, float sigma, std::vector<float>& data) {
    float expdiv = -2 * sigma * sigma;
    float normalizer = std::sqrt(2 * PI * sigma * sigma);

    for (size_t i = 0; i < data.size(); i++) { // NOLINT(modernize-loop-convert)
        float sc_data = data[i] - mu;
        sc_data = sc_data * sc_data;
        sc_data = sc_data / expdiv;
        sc_data = exp_scalar(sc_data);
        sc_data = sc_data / normalizer;
        data[i] = sc_data;
    }
}

// Vektorova implementace funkce 'normaldist_scalar(...)'.
void normaldist_vec(float mu, float sigma, std::vector<float>& data) {
    // Obdobne jako ve skalarni verzi vypoctu hustoty normalniho rozdeleni
    // (pocitane funkci 'normaldist_scalar'), budeme ve vektorovem vypoctu
    // potrebovat nekolik konstant. V pripade vektoroveho vypoctu je nutne,
    // aby tyto konstanty byly vektory (vektory obsahujici stejne hodnoty
    // na vsech pozicich. K tomu vyuzijeme funkci '_mm256_set1_ps(...)'.
    __m256 mm_expdiv = _mm256_set1_ps(-2 * sigma * sigma);
    __m256 mm_normalizer = _mm256_set1_ps(std::sqrt(2 * PI * sigma * sigma));
    // Vsimnete si, ze hodnotu konstant pocitame skalarne (tedy standartne
    // ve floatech). Az pote, co si skalrni hodnotu konstanty spocteme ji
    // ulozime do vektoru.

    // Krome techto konstant potrebujeme i vektorovou verzi konstanty 'mu':
    __m256 mm_mu = _mm256_set1_ps(mu);

    for (size_t i = 0; i < data.size(); i += 8) {
        // Nejprve si nacteme 8 prvku zacinajicich na i-te pozici pole data.
        __m256 mm_data = _mm256_loadu_ps(&data[i]);

        // S nactenym vektorem muzeme delat operace obdobne jako se skalarnim
        // typem ve funkci 'normaldist_scalar'. Misto infixovych operatoru
        // pouzijeme _mm256_ funkce v prefixove notaci:
        mm_data = _mm256_sub_ps(mm_data, mm_mu);
        mm_data = _mm256_mul_ps(mm_data, mm_data);
        mm_data = _mm256_div_ps(mm_data, mm_expdiv);
        // Pro vypocet exponencialy pouzijeme aproximaci funkci 'exp_vec(...)'.
        mm_data = exp_vec(mm_data);
        mm_data = _mm256_div_ps(mm_data, mm_normalizer);
        // Na zaver musime zpracovana data nahrat zpet do pameti.
        _mm256_storeu_ps(&data[i], mm_data);
    }
}

// `normaldist_vec`, ale za pouziti `simd` C++ knihovny
void normaldist_vec_cpp(float mu, float sigma, std::vector<float>& data) {
    using vec = simd::vec_f32_8;

    vec expdiv{-2 * sigma * sigma};
    vec normalizer{std::sqrt(2 * PI * sigma * sigma)};
    vec mu_vec{mu};

    for (size_t i = 0; i < data.size(); i += vec::size()) {
        vec sc_data = vec{}.load(&data[i]) - mu_vec;
        sc_data = sc_data * sc_data;
        sc_data = sc_data / expdiv;
        sc_data = exp_vec_cpp(sc_data);
        sc_data = sc_data / normalizer;
        sc_data.store(&data[i]);
    }
}

int main() {
    constexpr size_t N = 16 * 10000000;
    // Zkontrolujeme, ze N je delitelne 8, tedy pri pouziti vektoru nemusime resit "leftovers"
    static_assert(N % 8 == 0);

    // Vygenerujeme testovaci data
    std::cout << "Generating random test data...\n";
    std::vector<float> data = pdv::generate_random_vector<float>(N, 0.0, 3.0);

    // Vytvorime kopie dat
    auto data_scalar = data;
    auto data_vec = data;
    auto data_vec_cpp = data;

    // Otestujeme implementace
    pdv::benchmark("Scalar", 10, [&] {
        normaldist_scalar(0.0, 1.0, data_scalar);
    });

    pdv::benchmark("SIMD raw", 10, [&] {
        normaldist_vec(0.0, 1.0, data_vec);
    });

    pdv::benchmark("SIMD C++", 10, [&] {
        normaldist_vec_cpp(0.0, 1.0, data_vec_cpp);
    });

    // Spocitame rozdily v approximaci
    double diff_raw = 0.0;
    double diff_cpp = 0.0;
    for (size_t i = 0; i < N; i++) {
        diff_raw += abs(data_scalar[i] - data_vec[i]);
        diff_cpp += abs(data_scalar[i] - data_vec_cpp[i]);
    }

    // Pokud je odchylka SIMD metod vypoctu moc velka, vypiseme varovani
    constexpr double MAX_APPROXIMATION_ERROR = 100;
    if (diff_raw > MAX_APPROXIMATION_ERROR || diff_cpp > MAX_APPROXIMATION_ERROR) {
        std::cout << "\n";
        std::cout << std::fixed << std::setprecision(2);
        if (diff_raw > MAX_APPROXIMATION_ERROR) {
            std::cerr << "Ve 'SIMD raw' je pravdepodobne chyba. Absolutni chyba vypoctu: "
                      << diff_raw << "\n";
        }
        if (diff_cpp > MAX_APPROXIMATION_ERROR) {
            std::cerr << "V 'SIMD C++' je pravdepodobne chyba. Absolutni chyba vypoctu: "
                      << diff_cpp << "\n";
        }
    }

    return 0;
}