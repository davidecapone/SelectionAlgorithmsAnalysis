/**
 * @file asymptotic_analysis.c
 * @author Capone, Della Rovere, Gortani, Fior
 * @brief stima dei tempi di esecuzione dei 3 diversi algoritmi di selezione
 * 
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>

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
int randomVector( int A[] ){
  free(A);
  int len = (unsigned)rand() % 100;    // dimensione del vettore casuale (max MAX_LEN)
  
  A = ( ( int * )malloc( ( len ) * sizeof( int ) ) ); // allocazione dinamica memoria vettore

  for( int i = 0; i < len; i++) {

      int j = rand();     // genera un numero casuale
      A[ i ] = j;         // inserisce il numero casuale nel vettore
  }

  return len;
}

int main () {
  srand(time(NULL));

  // risoluzione stimata
  double R = getResolution();

  // valore relativo massimo ammissibile
  double E = 0.001;

  // tempo minimo misurabile
  double Tmin = R * ( 1/E + 1 );


  int *A = NULL;
  int size = randomVector(A);

   for (int i = 0; i < size; i++)
   {
     printf(" %i ", A[i]);
   }

  
  return 0;
}