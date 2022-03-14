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
#include <stdlib.h>

#define MAX_LEN 500000

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

/**
 * @brief alloca un vettore di dimensione random (tra 0 e MAX_LEN) e lo popola di numeri pseudo-casuali anche negativi
 * 
 * @param A vettore da allocare e popolare
 * @return int lunghezza vettore
 */
int randomVector(int *A){

    srand( ( unsigned )time( NULL ) ); // seme casuale

    int len = rand()%MAX_LEN;    // dimensione del vettore casuale (max MAX_LEN)

    A = ( ( int * )malloc( ( len ) * sizeof( int ) ) ); // allocazione dinamica memoria vettore

    for( int i = 0; i < len; i++) {

        int j = rand();     // genera un numero casuale

        if(rand()%2 != 0){
            j = 0 - j;      // pseudo-casualmente rende il numero negativo
        }

        A[ i ] = j;         // inserisce il numero casuale nel vettore
    }

    return len;
}