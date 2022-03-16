/**
 * @file asymptotic_analysis.c
 * @author Capone, Della Rovere, Gortani, Fior
 * @brief stima dei tempi di esecuzione dei 3 diversi algoritmi di selezione
 * 
 */
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>

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

/**
 * @brief genera dimensioni seguendo una distribuzione esponenziale
 *        per i tra 0 ... 99, ni = A * 2^( B*i )
 *        con A e B costanti e tali per cui:
 *        n0 = 100, n99 = 50000000 
 *        quindi posso prendere A = 100, B = 0.157673137
 * 
 * @return int[] vettore contenente 100 valori che indicano la dimensione
 */
int[] generateSamples () {

  int n[100];
  int A = 100;
  double B = 0.157673137;

  // 100 campioni
  for (int i = 0; i <= 99; i++)
  {
    n[i] = A * pow(2, B * i);
    // generare uno o più vettori di dimensione ni
  }

  return n;
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

  int[] samples = generateSamples();
  return 0;
}
