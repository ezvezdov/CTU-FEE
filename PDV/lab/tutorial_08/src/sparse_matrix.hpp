#pragma once

#include <vector>
#include <cmath>
#include <iostream>
#include <stdexcept>
#include <string>

/** Trida zabalujici jednotlive prvky vektoru / radky matice. */
class entry {
public:
    size_t index; // Pozice nenuloveho prvku v ramci vektoru
    double value; // Jeho hodnota

    entry(size_t index, double value) : index(index), value(value) {}

    // Pri porovnavani rovnosti dvou prvku vektoru dovolujeme urcitou nepresnost
    // pri vypoctu floating-point hodnoty.
    bool operator==(const entry& other) const {
        return index == other.index && std::abs(value - other.value) < 1e-6;
    }
};

class sparse_vector {
private:
    // Sparse operace nad vektory/maticemi funguji dobre proto, ze jsou nenulove
    // prvky vektoru/radku matice ulozeny v serazenem poradi (podle indexu prvku).
    // V nasem pripade je radime vzestupne. Abychom Vam pomohli, kontrolujeme, ze
    // prvky skutecne v tomto poradi vkladate. V promenne 'last_set_index' je index
    // posledniho vlozeneho prvku (nebo -1, pokud jeste zadny vlozeny nebyl).
    ptrdiff_t last_set_index = -1;

    // Vlastni seznam jednotlivych nenulovych prvku. K tomuto vektoru nemate primo
    // pristup. Pro ziskani seznamu nenulovych prvku pouzijte funkci 'entries(...)'.
    std::vector<entry> data{};

public:
    sparse_vector() = default;

    // Metodu 'set(...)' pouzivejte pro nastavovani nenulovych prvku vektoru.
    // POZOR! Prvky skutecne musite vkladat ve vzestupnem poradi, nebo dojde
    // k vyhozeni vyjimky!
    void set(size_t i, double value) {
        if ((ptrdiff_t)i < last_set_index) {
            throw std::invalid_argument("Tried to add value at index " + std::to_string(i)
                    + ", but the last value was added at index " + std::to_string(last_set_index)
                    + ". You must add elements in ascending order.");
        }
        last_set_index = (ptrdiff_t)i;
        data.emplace_back(i, value);
    }

    /** Vraci seznam nenulovych prvku radku matice/vektoru. */
    [[nodiscard]] const std::vector<entry>& entries() const {
        return data;
    }

    bool operator==(const sparse_vector& other) const {
        return data == other.data;
    }

    bool operator!=(const sparse_vector& other) const {
        return data != other.data;
    }

    void reserve(size_t size) {
        data.reserve(size);
    }
};

/** Radek matice - `sparse_vector` s pridanou informaci, o kolikaty radek matice se jedna. */
class matrix_row : public sparse_vector {
public:
    /** Index udavajici o kolikaty radek matice se jedna. */
    size_t index;

public:
    explicit matrix_row(size_t index) : sparse_vector{}, index(index) {}
};

/** Ridka matice - vektor ridkych radku ('matrix_row'). */
using sparse_matrix = std::vector<matrix_row>;
