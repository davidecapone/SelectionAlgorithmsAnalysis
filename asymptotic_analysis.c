/**
 * @file asymptotic_analysis.c
 * @author Capone, Della Rovere, Gortani, Fior
 * @brief stima dei tempi di esecuzione dei 3 diversi algoritmi di selezione
 * 
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

#define MAX_LEN_FOR_TEST 500000

// alloca number spazio al vettore di tipo int
#define MALLOC_ARRAY(number, type) \
    ((type *)malloc((number) * sizeof(type)))

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

void populate ( int A[], int len ) {

  int random;
  for( int i = 0; i < len; i++) {

      // ottenere anche numeri negativi
      int random = rand() - (RAND_MAX/2);     
      A[ i ] = random;         
  }
}


int main () {
  srand(time(NULL));
  int *A = NULL;

  // risoluzione stimata
  double R = getResolution();

  // valore relativo massimo ammissibile
  double E = 0.001;

  // tempo minimo misurabile
  double Tmin = R * ( 1/E + 1 );

  // genera numeri interi positivi (dimensione dei vettori)
  int len = (unsigned)rand() % MAX_LEN_FOR_TEST;
  A = MALLOC_ARRAY(len, int);

  if (A != NULL)
    populate(A, len);

  for (size_t i = 0; i < len; i++) {
    printf(" %i ", A[i]);
  }

  // liberare memoria precedenztemente allocata
  free(A);

  return 0;
}