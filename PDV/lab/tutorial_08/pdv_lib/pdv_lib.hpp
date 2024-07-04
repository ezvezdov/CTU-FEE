/**
 * Small library to factor out repeated boilerplate code for benchmarking.
 */
#pragma once

#include <chrono>
#include <iostream>
#include <iomanip>
#include <atomic>
#include <vector>
#include <string>
#include <cassert>
#include <optional>
#include <sstream>
#include <algorithm>
#include <cstdint>
#include <limits>

namespace pdv {
    /** RAII class for saving and restoring stream flags. */
    class stream_flags {
    private:
        std::ios& stream;
        std::ios::fmtflags flags{stream.flags()};

    public:
        explicit stream_flags(std::ios& stream) : stream{stream} {}

        ~stream_flags() {
            stream.flags(flags);
        }

        stream_flags(const stream_flags&) = delete;
        stream_flags& operator=(const stream_flags&) = delete;
    };

    /** Operator overload to support pretty-printing `std::chrono::duration`. */
    template<typename Rep, typename Period>
    inline std::ostream& operator<<(std::ostream& of, std::chrono::duration<Rep, Period> d) {
        using namespace std::chrono;
        using namespace std::chrono_literals;

        // write to an intermediate sstream instead, otherwise formatting flags behave weirdly
        //  (e.g. std::setw only setting width for the number and not for the unit,...)
        std::stringstream s{};
        if (d < 1ms) {
            s << duration_cast<microseconds>(d).count() << " μs";
        } else if (d < 100ms) {
            s << std::setprecision(2) << std::fixed;
            s << duration<double, std::milli>(d).count() << " ms";
        } else {
            s << duration_cast<milliseconds>(d).count() << " ms";
        }

        of << s.str();
        return of;
    }

    // source: https://github.com/martinus/nanobench/blob/b8d337a3bb1a430b5da6661bae430491fceb49c6/src/include/nanobench.h#L1027
    // do_not_optimize_away is a function which convinces the compiler that it's using the argument,
    // but actually is a no-op; useful to prevent the compiler from optimizing away our benchmarks
    #if defined(_MSC_VER)

    #pragma optimize("", off)

    void do_not_optimize_away_sink(const void*) {}

    #pragma optimize("", on)

    template<typename Arg>
    void do_not_optimize_away(const Arg& arg) {
        do_not_optimize_away_sink(&arg);
    }

    #else

    template<typename T>
    inline void do_not_optimize_away(const T& val) {
        asm volatile("" : : "r,m"(val) : "memory");
    }

    template<typename T>
    inline void do_not_optimize_away(T& val) {
        #if defined(__clang__)
        asm volatile("" : "+r,m"(val) : : "memory");
        #else
        asm volatile("" : "+m,r"(val) : : "memory");
        #endif
    }

    #endif

    // namespace for internal implementation details
    namespace _ {
        inline bool show_speedup = true;
        inline std::optional<std::chrono::nanoseconds> speedup_base{};
        inline size_t benchmark_name_width = 20;
    }

    inline void show_speedup() {
        _::show_speedup = true;
    }

    inline void hide_speedup() {
        _::show_speedup = false;
    }

    inline void clear_benchmark_history() {
        _::speedup_base = std::nullopt;
        _::benchmark_name_width = 20;
    }

    /**
     * An exception class, which is thrown from tests that are supposed to be implemented
     * by students. `pdv::benchmark` handles this exception and shows an appropriate message.
     */
    class not_implemented : public std::runtime_error {
    public:
        // TODO: if inside an openmp block `omp_get_level() > 0`, pass the exception some other way
        //  alternatively, add a general mechanism for throwing exceptions through to pdv::benchmark from omp parallel
        not_implemented() : runtime_error("Not yet implemented") {}
    };

    /**
     * Invokes `fn()` `iteration_count` times and returns the average duration of a single
     * iteration. If `warmup_iteration_count` is non-zero, `fn()` is executed before the measurement
     * is started to warmup caches, page in memory pages,...
     */
    template<typename BenchmarkFn>
    [[nodiscard]] inline std::chrono::nanoseconds
    benchmark_raw(size_t warmup_iteration_count, // NOLINT(bugprone-easily-swappable-parameters)
                  size_t iteration_count, BenchmarkFn fn) {
        // run the warmup iterations without measurement
        for (size_t i = 0; i < warmup_iteration_count; i++) {
            fn();
        }

        auto begin = std::chrono::steady_clock::now();

        // surround with barriers to prevent the compiler and CPU from being too clever with reordering
        std::atomic_thread_fence(std::memory_order_seq_cst);
        for (size_t i = 0; i < iteration_count; i++) {
            fn();
            std::atomic_thread_fence(std::memory_order_seq_cst);
        }

        auto end = std::chrono::steady_clock::now();
        return std::chrono::nanoseconds(end - begin) / iteration_count;
    }

    template<typename BenchmarkFn>
    [[nodiscard]] inline auto benchmark_raw(size_t iteration_count, BenchmarkFn fn) {
        return benchmark_raw(0, iteration_count, fn);
    }

    template<typename BenchmarkFn>
    inline void benchmark(std::string_view description, size_t warmup_iteration_count,
                   size_t iteration_count, BenchmarkFn fn) {
        // keep the length of the longest benchmark name
        if (_::benchmark_name_width < description.size()) {
            _::benchmark_name_width = description.size();
        }

        std::cout << std::setw((int)_::benchmark_name_width) << description << ": " << std::flush;
        try {
            // run the benchmark
            auto single_iter_duration = benchmark_raw(warmup_iteration_count, iteration_count, fn);

            bool should_show_speedup = _::show_speedup;

            // store the duration as reference if this is the first benchmark
            if (!_::speedup_base.has_value()) {
                _::speedup_base = single_iter_duration;
                // do not show speedup for the first benchmark
                should_show_speedup = false;
            }

            // print the results
            stream_flags flags(std::cout);
            std::cout << std::left << std::setw(10) << single_iter_duration;
            if (should_show_speedup || iteration_count > 1) {
                std::cout << " (";
                if (should_show_speedup) {
                    auto speedup = (double)_::speedup_base.value().count()
                            / (double)single_iter_duration.count();
                    std::cout << "speedup: " << std::setprecision(2) << std::fixed << speedup
                              << "x";
                }
                if (iteration_count > 1) {
                    std::cout << (should_show_speedup ? ", " : "") << iteration_count
                              << " iterations";
                }
                std::cout << ")";
            }
        } catch (const pdv::not_implemented&) {
            std::cout << "--- not implemented ---";
        }

        std::cout << std::endl;
    }

    template<typename BenchmarkFn>
    inline void benchmark(std::string_view description, size_t iteration_count, BenchmarkFn fn) {
        benchmark(description, 0, iteration_count, fn);
    }

    template<typename BenchmarkFn>
    inline void benchmark(std::string_view description, BenchmarkFn fn) {
        benchmark(description, 0, 1, fn);
    }

    // namespace for internal implementation details
    namespace _ {
        // https://artificial-mind.net/blog/2020/09/26/dont-deduce
        // a "hack" to prevent the compiler from deducing template arguments
        template<typename T>
        using dont_deduce = typename std::common_type<T>::type;

        class lcg_musl {
        private:
            // LCG parameters taken from Musl (https://github.com/bminor/musl/blob/master/src/prng/rand.c)
            // they are nice, because the modulo is 2^64, which is a no-op with 64bit arithmetic
            static constexpr uint64_t ADD_COEF = 1ull;
            static constexpr uint64_t MULTIPLY_COEF = 6'364'136'223'846'793'005ull;

            uint64_t state;

        public:
            // use `seed + 1` to avoid seeding with 0, which would return 1 as the first `state`
            explicit lcg_musl(uint64_t seed) : state{seed + 1} {}

            /**
             * The bottom bits of `state` are very periodic (e.g. the bottom bit alternates
             * between 1 and 0), only use the upper 32 bits for more randomness.
             */
            uint64_t operator()() {
                // the LCG works modulo 2^64, which is implicit for 64-bit unsigned arithmetic
                state = state * MULTIPLY_COEF + ADD_COEF;
                return state;
            }
        };

        class uniform_random_bool {
        private:
            lcg_musl lcg;

        public:
            explicit uniform_random_bool(uint64_t seed) : lcg{seed} {}

            bool operator()() {
                // use the highest bit, it should be the most unpredictable one
                // the bottom bits are quite predictable (e.g. the first bit always alternates)
                return (bool)(lcg() >> 63);
            }
        };

        template<typename T>
        class uniform_random_int {
            static_assert(std::is_integral_v<T>);
        private:
            lcg_musl lcg;
            const T min;
            const uint64_t diff;

        public:
            explicit uniform_random_int(uint64_t seed) : lcg{seed}, min{0}, diff{0} {}

            // prevent template argument deduction from min, max
            uniform_random_int(uint64_t seed, dont_deduce<T> min, dont_deduce<T> max)
                    : lcg{seed}, min{min}, diff{(uint64_t)(max - min)} {
                assert(min < max);
            }

            T operator()() {
                if constexpr (sizeof(T) <= 4) {
                    if (diff == 0) {
                        return (T)generate_u32();
                    } else {
                        return (T)(generate_u32() % diff) + min;
                    }
                } else {
                    if (diff == 0) {
                        return (T)generate_u64();
                    } else {
                        return (T)(generate_u64() % diff) + min;
                    }
                }
            }

        private:
            uint64_t generate_u64() {
                // recover an u64 pseudo-random number by combining two u32 pseudo-random numbers
                return ((uint64_t)generate_u32() << 32) | (uint64_t)generate_u32();
            }

            uint32_t generate_u32() {
                // the bottom bits of `state` are very periodic (e.g. the bottom bit alternates
                //  between 1 and 0), use the upper 32 bits for more randomness
                return lcg() >> 32;
            }
        };

        template<typename T>
        class uniform_random_float {
            static_assert(std::is_floating_point_v<T>);
        private:
            lcg_musl lcg;
            const T min;
            const T diff;

        public:
            // to simplify usage, default float range is <0.0, 1.0)
            explicit uniform_random_float(uint64_t seed)
                    : uniform_random_float(seed, (T)0.0, (T)1.0) {}

            // prevent template argument deduction from min, max
            uniform_random_float(uint64_t seed, dont_deduce<T> min, dont_deduce<T> max)
                    : lcg{seed}, min{min}, diff{max - min} {
                assert(min < max);
            }

            T operator()() {
                return generate_normalized() * diff + min;
            }

        private:
            T generate_normalized() {
                // https://prng.di.unimi.it/, section "Generating uniform doubles in the unit interval"
                if constexpr (std::is_same_v<T, double>) {
                    // double has 53 bits of mantissa
                    return (double)(lcg() >> (64 - 53)) * 0x1.0p-53;
                } else if (std::is_same_v<T, float>) {
                    // float has 24 bits of mantissa
                    return (float)(lcg() >> (64 - 24)) * 0x1.0p-24f;
                } else {
                    // unknown floating point type, let's use as much randomness as we can
                    // use 63 bits instead of 64 bits to improve performance, https://godbolt.org/z/K13nceazj
                    return (T)(lcg() >> 1) * (T)0x1.0p-63;
                }
            }
        };
    }

    /**
     * Simple LCG random number generator from a uniform distribution in <min, max).
     * The seed is fixed, but each instance receives a different one, so the generated
     * sequence is the same between repeated executions of the whole binary.
     *
     * This class exists because C++11 <random> is very slow, dunno why, and we don't need
     * much randomness for these benchmarks.
     *
     * Example usage:
     * ```cpp
     * pdv::uniform_random<uint32_t> random(0, 1000);
     * auto r1 = random();
     * auto r2 = random();
     * ...
     * ```
     */
    template<typename T>
    class uniform_random {
        static_assert(std::is_same_v<T, bool> || std::is_integral_v<T>
                || std::is_floating_point_v<T>);
        static_assert(sizeof(T) <= 8, "pdv::uniform_random supports at most 64 bit result types");
    private:
        // use either bool, int or float version based on T
        using rng_t = std::conditional_t<
                std::is_same_v<T, bool>, _::uniform_random_bool,
                std::conditional_t<std::is_integral_v<T>,
                        _::uniform_random_int<T>,
                        _::uniform_random_float<T>>>;

        // yes, use seed 0, to get reproducible results
        static inline size_t next_seed = 0; // NOLINT(cppcoreguidelines-avoid-non-const-global-variables)

        rng_t rng;

    public:
        // use different (but deterministic) seed for each instance
        uniform_random() : rng{next_seed++} {}

        // prevent template argument deduction from min, max
        uniform_random(_::dont_deduce<T> min, _::dont_deduce<T> max) : rng{next_seed++, min, max} {}

        T operator()() {
            return rng();
        }
    };

    template<typename T, typename ...Ts>
    inline std::vector<T> generate_random_vector(size_t length, Ts&& ...args) {
        // forward all extra arguments after `length` to pdv::uniform_random constructor
        pdv::uniform_random<T> random((T)(args)...);
        std::vector<T> vec(length);
        std::generate(vec.begin(), vec.end(), random);
        return vec;
    }
}