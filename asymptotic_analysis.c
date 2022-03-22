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
#include <limits.h>
#include <string.h>

#include "quickSelect.c"

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
 * @brief Get the Tmin object
 * 
 * @return double 
 */
double getTmin () {
  // risoluzione stimata
  double R = getResolution();
  // errore relativo massimo ammissibile
  double E = 0.001;
  // tempo minimo misurabile
  return R * ( 1/E + 1 );
}

/**
 * @brief popola vettore di dimensione len con valori pseudo-casuali compresi tra [INT_MIN, INT_MAX]
 * 
 * @param A vettore
 * @param len dimensione
 */
void populate ( int A[], int len ) {

  int random;
  for (int i = 0; i < len; i++) {

    // il primo mebro fornisce il segno
    random = (-1 + rand() % 2) * rand();
    A[i] = random;
  }
}

void testAsymptotic ( int A[], int size, int k, int Tmin ) {

  int kSmallest;
  struct timespec start, end;
  int i = 0;
  double B[5];
  double period = 0;

  clock_gettime(CLOCK_MONOTONIC, &start);
  while ( i < 5 ) {
    kSmallest = quickSelect(A, 0, size-1, k);
    clock_gettime(CLOCK_MONOTONIC, &end);

    // tolgo tempo misurato precedentemente per ottenere intervallo attuale
    period = duration(start, end) - period;

    if (period > Tmin) {
      B[i] = period;
      i++;
    }
  }

  printf("dimension %d ", size);
  double average = 0;
  for (int i = 0; i < 5; i++)
  {
    average += B[i];
  }
  average = average / 5;
  printf("average time: %f \n", average);

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
void generateSamples () {

  int * array = NULL;
  int A = 100;
  double B = 0.157673137;
  int ni;
  int nArray;
  int MAX_N_ARRAY = 10; // n.ro di array per ogni campione 
  double Tmin = getTmin();

  // 100 campioni
  for (int i = 0; i <= 99; i++) {
    
    // dimensione del campione
    ni = A * pow(2, B * i);

    // q.ta array casuale (1..MAX_N_ARRAY)
    nArray = (rand() % MAX_N_ARRAY) + 1;

    for (int i = 0; i < nArray; i++) {

      // allocare spazio per array dimensione ni
      array = MALLOC_ARRAY(ni, int);

      // popolo array numeri casuali
      populate(array, ni);

      // test tempi esecuzione algoritmi
      testAsymptotic( array, ni, 0, getTmin() );

      // liberare spazio occupato da array
      free(array);
      array = NULL;
    }
  }
}


int main () {
  srand(time(NULL));
  int *A = NULL;

  //int[] samples = generateSamples();
  //populate();
  //generateSamples();
  
  /*
  double b = clock(); 
  printf("%d", 8);
  double e = clock(); 

  printf(" %f \n", (e-b)/CLOCKS_PER_SEC);
  printf("tmin: %f \n", Tmin);
  */

  generateSamples();
  return 0;
}
