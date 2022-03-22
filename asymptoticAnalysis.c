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
#include "heapSelect.c"
//#include "medianMediansSelect.c"

// alloca number spazio al vettore di tipo int
#define MALLOC_ARRAY(number, type) \
    ((type *)malloc((number) * sizeof(type)))

enum Algorithm {
  QuickSelect,
  HeapSelect,
  MedianMediansSelect
};

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
 * @brief calcola il tempo minimo misurabile dalla macchina
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
 * @brief calcola il tempo medio da un vettore di misurazioni di tempi
 * 
 * @param A vettore
 * @param len dimensione vettore
 * @return double media degli elementi del vettore
 */
double averageTime(double A[], int len){

  double average = 0;

  for (int i = 0; i < len; i++) {
    average += A[i];
  }

  average = average / len;

  return average;
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
    random = (-1 + rand() % 2) * rand(); // espediente per ottenere i numeri negativi (da risolvere)
    A[i] = random;
  }
}

/**
 * @brief 
 * 
 * @param A 
 * @param size 
 * @param k 
 * @param Tmin 
 */
void testAsymptotic ( int A[], int size, int k, int Tmin, FILE* fptr) {

  int kSmallest;
  struct timespec start, end;
  int i = 0;
  double times[5];
  double period = 0;

  clock_gettime(CLOCK_MONOTONIC, &start);
  while ( i < 5 ) {
    kSmallest = quickSelect(A, 0, size-1, k);
    clock_gettime(CLOCK_MONOTONIC, &end);

    // tolgo tempo misurato precedentemente per ottenere intervallo attuale
    period = duration(start, end) - period;

    if (period > Tmin) {
      times[i] = period;
      i++;
    }
  }

  fprintf(fptr, "quickSelect, %d, %f\n", size, averageTime(times, 5));
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
  FILE* fptr = NULL;

  fptr = fopen("./asymptotic_times.csv", "a");

  // 100 campioni
  for (int i = 0; i <= 10; i++) {
    
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
      testAsymptotic( array, ni, 0, Tmin, fptr);

      // liberare spazio occupato da array
      free(array);
      array = NULL;
    }
  }

  fclose(fptr);
}

int main () {
  srand(time(NULL));

  FILE *fptr = fopen("./asymptotic_times.csv", "w");
  fprintf(fptr, "algorithm, size, time\n");
  fclose(fptr);
  
  generateSamples();

  return 0;
}
