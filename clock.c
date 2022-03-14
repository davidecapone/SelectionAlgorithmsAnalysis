/**
 * @file clock.c
 * @author Capone, Della Rovere, Gortani, Fior
 * @brief 
 * @version 0.1
 * @date 2021-12-07
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include <stdio.h>
#include <time.h>

double duration(struct timespec start, struct timespec end) {
    return end.tv_sec - start.tv_sec
         + ((end.tv_nsec - start.tv_nsec ) / (double) 1000000000.0);
}

double getResolution(){
    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);
    do {
        clock_gettime(CLOCK_MONOTONIC, &end);
    } while (duration(start, end) == 0.0);
    return duration(start, end);
}

int main () {
    // risoluzione stimata
    double R = getResolution();

    // valore relativo massimo ammissibile
    double E = 0.001;

    // tempo minimo misurabile
    double Tmin = R * ( 1/E + 1 );

    printf("%f\n", Tmin);
    return 0;
}