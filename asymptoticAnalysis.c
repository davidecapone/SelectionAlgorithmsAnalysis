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

// alloca 'number' spazio al vettore di tipo int
#define MALLOC_ARRAY(number, type) \
    ((type *)malloc((number) * sizeof(type)))

typedef enum {
  QuickSelect,
  HeapSelect,
  MedianMediansSelect
} Algorithm;

double Tmin;

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
 *        a partire dalla risoluzione stimata e dall'errore massimo ammissibile
 * 
 * @return double tempo minimo misurabile
 */
double getTmin () {
  double R = getResolution();
  double E = 0.001;
  return R * ( 1/E + 1 );
}

/**
 * @brief tempo medio da un vettore di misurazioni
 * 
 * @param A vettore
 * @param len dimensione vettore
 * @return double valore medio
 */
double averageTime(double A[], int len){

  double average = 0;
  for (int i = 0; i < len; i++) average += A[i];
  average = average / len;
  return average;
}

/**
 * @brief popola vettore dimensione len con valori pseudocasuali compresi tra [INT_MIN, INT_MAX]
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
 * @param type 
 * @param A 
 * @param size 
 * @param k 
 * @return double* 
 */
double getMeanDuration ( Algorithm type, int A[], int size, int k ) {

  int i = 0;
  double period = 0;
  int kSmallest;
  struct timespec start, end;
  double times[5];
  
  /**
   * PROBLEMA: ho notato che ci sono tempi di esecuzione medi inferiori a Tmin
   * inoltre cosa succede se questa funzione ritorna direttamente average time?
   */
  clock_gettime(CLOCK_MONOTONIC, &start);
  while ( i < 5 ) {

    switch (type) {

      case QuickSelect:
        kSmallest = quickSelect(A, 0, size-1, k);
        break;
      
      case HeapSelect:
        kSmallest = heapSelect(A, 0, size-1, k);
        break;

      case MedianMediansSelect:
        break;
    }
    
    clock_gettime(CLOCK_MONOTONIC, &end);
    // tolgo tempo misurato precedentemente per ottenere intervallo attuale
    period = duration(start, end) - period;
    // sse period è maggiore del tempo minimo misurabile posso avanzare con l'analisi
    if ( period > Tmin ) {
      times[i] = period;
      i++;
    }
  }

  return averageTime(times, 5);

}

/**
 * @brief valuta 3 diversi algoritmi di selezione e scrive in rislutati ottenuti in un csv
 * 
 * @param A array di numeri pseudocasuali
 * @param size dimensione array
 * @param k 
 * nota: per questioni di efficienza il FILE viene aperto e chiuso NON QUA ma in generateSamples
 */
void testAsymptotic ( int A[], int size, FILE* fptr) {

  // k-esimo elemento ( FISSATO ) da usare nei test
  int k = 0;
  double averageTime;
  Algorithm type;

  type = QuickSelect;
  averageTime = getMeanDuration ( type, A, size, k );
  fprintf(fptr, "quickSelect, %d, %f\n", size, averageTime);
  printf("quickSelect, size %d, average time %fs\n", size, averageTime);

  type = HeapSelect;
  averageTime = getMeanDuration ( type, A, size, k);
  fprintf(fptr, "heapSelect, %d, %f\n", size, averageTime);
  printf("heapSelect, size %d, average time %fs\n", size, averageTime);
}

/**
* @brief DA RISCRIVERE
* 
*/
void generateSamples () {

  int * sample = NULL;
  int A = 100;
  double B = 0.157673137;
  int ni;
  int nArray;
  /* da chiedere: quanti ne generiamo per ogni campione al massimo? */
  int MAX_N_ARRAY = 5;
  FILE* fptr = NULL;

  fptr = fopen("./asymptotic_times.csv", "a");

  // 100 campioni di diverse dimensioni
  /**
   * - generare 100 campioni
   * - dimensione crescente segue una distribuzione esponenziale
   *   costanti A, B in modo tale che le dimensioni varino nell'intervallo [ 0, 5000000 ]
   * - scelta pseudocasuale del numero di array da generare per ogni dimensione
   */
  for (int i = 0; i <= 99; i++) {

    ni = A * pow(2, B * i);
    nArray = (rand() % MAX_N_ARRAY) + 1;

    /**
     * - allocare lo spazio per nArray di dimensione ni ciascuno
     * - popolare ciascuno di numeri pseudocasuali
     * - valutare il tempo d'esecuzione
     * - libeare lo spazio allocato
     */
    for (int i = 0; i < nArray; i++) {

      sample = MALLOC_ARRAY( ni, int );
      populate( sample, ni );
      testAsymptotic( sample, ni, fptr );
      free( sample );
      sample = NULL;
    }
  }

  fclose( fptr );
}

int main () {
  srand(time(NULL));

  // istanzio la var. globale 
  Tmin = getTmin();
  printf("\nTmin = %fs\n", Tmin);
  // file dei tempi medi sovrascritto e aggiunte intestazioni
  FILE *fptr = fopen("./asymptotic_times.csv", "w");
  fprintf(fptr, "algorithm, size, time\n");
  fclose(fptr);
  
  generateSamples();
  return 0;
}
