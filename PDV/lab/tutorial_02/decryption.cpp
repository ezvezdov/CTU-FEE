#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include "decryption.h"

using namespace std;

void decrypt_sequential(const PDVCrypt& crypt,
                        std::vector<std::pair<std::string, enc_params>>& encrypted, unsigned int) {
    // V sekvencni verzi je pocet vlaken (numThreads) ignorovany
    const auto size = static_cast<unsigned long>(encrypted.size());
    for (unsigned long i = 0; i < size; i++) {
        auto& enc = encrypted[i];
        crypt.decrypt(enc.first, enc.second);
    }
}

void
decrypt_openmp(const PDVCrypt& crypt, std::vector<std::pair<std::string, enc_params>>& encrypted,
               unsigned int numThreads) {
    const long size = static_cast<long>(encrypted.size());

#pragma omp parallel for num_threads(numThreads)
    for (long i = 0; i < size; i++) {
        auto& enc = encrypted[i];
        crypt.decrypt(enc.first, enc.second);
    }
}

void
decrypt_threads_1(const PDVCrypt& crypt, std::vector<std::pair<std::string, enc_params>>& encrypted,
                  unsigned int numThreads) {
    const unsigned long size = static_cast<unsigned long>(encrypted.size());
    unsigned long i = 0;

    // process je "vnorena" funkce (lambda funkce) void process(), ktera ma pristup
    // ke vsem promennym
    auto process = [&]() {
        while (i < size) {
            auto& enc = encrypted[i];
            i++;
            crypt.decrypt(enc.first, enc.second);
        }
    };

    std::vector<std::thread> threads{};
    for (size_t j = 0; j < numThreads; j++) {
        threads.emplace_back(process);
    }
    for (auto& t: threads) {
        t.join();
    }
}

void
decrypt_threads_2(const PDVCrypt& crypt, std::vector<std::pair<std::string, enc_params>>& encrypted,
                  unsigned int numThreads) {
    const unsigned long size = static_cast<unsigned long>(encrypted.size());
    unsigned long i = 0;

    std::mutex m{};

    // process je "vnorena" funkce (lambda funkce) void process(), ktera ma pristup
    // ke vsem promennym
    auto process = [&]() {
        std::unique_lock<std::mutex> lock(m);
        while (i < size) {
            auto& enc = encrypted[i];
            i++;
            lock.unlock();
            crypt.decrypt(enc.first, enc.second);
            lock.lock();
        }
    };

    std::vector<std::thread> threads{};
    for (size_t j = 0; j < numThreads; j++) {
        threads.emplace_back(process);
    }
    for (auto& t: threads) {
        t.join();
    }
}

void
decrypt_threads_3(const PDVCrypt& crypt, std::vector<std::pair<std::string, enc_params>>& encrypted,
                  unsigned int numThreads) {
    const auto size = static_cast<unsigned long>(encrypted.size());
    std::atomic<unsigned long> i(0);

    // process je "vnorena" funkce (lambda funkce) void process(), ktera ma pristup
    // ke vsem promennym
    auto process = [&]() {
        while (true) {
            auto local_i = i++;
            if (local_i >= size) {
                break;
            }
            auto& enc = encrypted[local_i];
            crypt.decrypt(enc.first, enc.second);
        }
    };

    std::vector<std::thread> threads{};
    for (size_t j = 0; j < numThreads; j++) {
        threads.emplace_back(process);
    }
    for (auto& t: threads) {
        t.join();
    }
}

void
decrypt_threads_4(const PDVCrypt& crypt, std::vector<std::pair<std::string, enc_params>>& encrypted,
                  unsigned int numThreads) {

    // Doplnte logiku vypoctu disjunktnich rozsahu pro jednotliva vlakna
    // Kod pro spusteni vlaken (a predani rozsahu) je jiz hotovy. Vasim
    // cilem je spocitat hodnoty promennych 'begin' a 'end' pro kazde
    // vlakno.
    const auto size = static_cast<unsigned long>(encrypted.size());
    const auto size_per_thread = size / numThreads + 1;
    auto start_i = 0ul;
    vector<thread> threads;
    for (unsigned int t = 0; t < numThreads; t++) {
        // Doplnte vypocet rozsahu pro t-te vlakno zde:
        auto begin = start_i;
        auto end = std::min(size, start_i + size_per_thread);
        start_i += size_per_thread;

        threads.emplace_back([&](unsigned long begin, unsigned long end) {
            for (unsigned int i = begin; i < end; i++) {
                auto& enc = encrypted[i];
                crypt.decrypt(enc.first, enc.second);
            }
        }, begin, end);
    }
    for (unsigned int t = 0; t < numThreads; t++) {
        threads[t].join();
    }
}
