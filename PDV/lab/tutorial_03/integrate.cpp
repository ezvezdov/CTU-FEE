#include "integrate.h"

#include <omp.h>
#include <cmath>

double
integrate_sequential(std::function<double(double)> integrand, double a, double step_size, int step_count) {

    // Promenna kumulujici obsahy jednotlivych obdelniku
    double acc = 0.0;
    for(int i = 0; i < step_count; i++) {
        auto center = a + i * step_size + step_size/2;
        auto y = integrand(center) * step_size;
        acc += y;
    }

    // Celkovy obsah aproximuje hodnotu integralu funkce
    return acc;
}

double
integrate_omp_critical(std::function<double(double)> integrand, double a, double step_size, int step_count) {
    //TODO
    // Rozdelte celkovy interval na podintervaly prislusici jednotlivym vlaknum
    // Identifikujte kritickou sekci, kde musi dojit k zajisteni konzistence mezi vlakny
    double acc = 0.0;

#pragma omp parallel for
    for(int i = 0; i < step_count; i++) {
        auto center = a + i * step_size + step_size/2;
        auto y = integrand(center) * step_size;
#pragma omp critical
        {
            acc += y;
        }

    }

    return acc;
}

double
integrate_omp_atomic(std::function<double(double)> integrand, double a, double step_size, int step_count) {
    double acc = 0.0;

    //TODO
    // Rozdelte celkovy interval na podintervaly prislusici jednotlivym vlaknum
    // Identifikujte kritickou sekci, kde musi dojit k zajisteni konzistence mezi vlakny

#pragma omp parallel for
    for(int i = 0; i < step_count; i++) {
        auto center = a + i * step_size + step_size/2;
        auto y = integrand(center) * step_size;

#pragma omp atomic
        acc += y;
    }

    return acc;
}

double integrate_omp_reduction(std::function<double(double)> integrand, double a, double step_size, int step_count) {
    double acc = 0.0;

    //TODO
    // Rozdelte celkovy interval na podintervaly prislusici jednotlivym vlaknum
    // Identifikujte kritickou sekci, kde musi dojit k zajisteni konzistence mezi vlakny

#pragma omp parallel for reduction(+:acc)
    for(int i = 0; i < step_count; i++) {
        auto center = a + i * step_size + step_size/2;
        auto y = integrand(center) * step_size;
        acc += y;
    }


    return acc;
}

double integrate_omp_for_static(std::function<double(double)> integrand, double a, double step_size, int step_count) {
    // Promenna kumulujici obsahy jednotlivych obdelniku
    double acc = 0.0;

    //TODO
    //rozsirte integrate_omp_reduction o staticke rozvrhovani

#pragma omp parallel for reduction(+:acc) schedule(static)
    for(int i = 0; i < step_count; i++) {
        auto center = a + i * step_size + step_size/2;
        auto y = integrand(center) * step_size;
        acc += y;
    }

    // Celkovy obsah aproximuje hodnotu integralu funkce
    return acc;
}

double integrate_omp_for_dynamic(std::function<double(double)> integrand, double a, double step_size, int step_count) {
    // Promenna kumulujici obsahy jednotlivych obdelniku
    double acc = 0.0;

    //TODO
    //rozsirte integrate_omp_reduction o dynamicke rozvrhovani

#pragma omp parallel for reduction(+:acc) schedule(dynamic)
    for(int i = 0; i < step_count; i++) {
        auto center = a + i * step_size + step_size/2;
        auto y = integrand(center) * step_size;
        acc += y;
    }


    // Celkovy obsah aproximuje hodnotu integralu funkce
    return acc;
}
