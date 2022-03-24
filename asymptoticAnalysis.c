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

double Tmin = 0;
FILE* fptr;

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
 * @brief genera un numero casuale compreso tra -RAND_MAX e RAND_MAX 
 *  genero inizialmente un numero random tra 0 e 1
 *  se questo è 0: moltiplico -1 per il numero random generato tra 0 ... RAND_MAX
 *  altrimenti: moltiplico 1 per numero random generato (non modificando quindi il segno)
 * @return int numero random compreso tra [-RAND_MAX ... RAND_MAX]
 */
int generateRandomInt() {
    
    int randomUnsigned = rand();
    int sign = rand() % 2;
    if (sign == 0) sign = -1;
    return randomUnsigned * sign;
}

/**
 * @brief popola vettore dimensione len con valori pseudocasuali compresi tra [INT_MIN, INT_MAX]
 * 
 * @param A vettore
 * @param len dimensione
 */
void populate ( int A[], int len ) {

  for (int i = 0; i < len; i++) A[i] = generateRandomInt();
}

/**
 * @brief esecuzione in secondi di un algoritmo di selezione su un dato vettore
 * 
 * @param type tipo di algoritmo di selezione da utilizzare
 * @param A vettore di interi pseudocasuali
 * @param size dimensione del vettore
 * @param k indice da usare nelle valutazioni degli algoritmi
 * @return double tempo in secondi
 */
double getExecutionTime ( Algorithm type, int A[], int size, int k ) {

  double period = 0;
  struct timespec start, end;
  int kSmallest; // inutile ai fine dell'analisi

  clock_gettime(CLOCK_MONOTONIC, &start);
  do {

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
    period = duration(start, end) - period;
  } while ( period <= Tmin );

  return period;
}

void appendToCSV ( Algorithm type, int size, double time ) {

  switch (type) {

    case QuickSelect:
      fprintf(fptr, "quickSelect, %d, %f\n", size, time);
      printf("[appended CSV] quickSelect, size %d, average time %fs\n", size, time);
      break;

    case HeapSelect:
      fprintf(fptr, "heapSelect, %d, %f\n", size, time);
      printf("[appended CSV] heapSelect, size %d, average time %fs\n", size, time);
      break;

    case MedianMediansSelect:
      //fprintf(fptr, "medianMediansSelect, %d, %f\n", size, time);
      //printf("[appended CSV] medianMediansSelect, size %d, average time %fs\n", size, time);
      printf("[appended CSV] medianMediansSelect coming soon...\n");
      break;
    
    default:
      break;
  }
}

/**
 * @brief DA SCRIVERE
 * 
 * @param i 
 * @return int 
 */
int expDistribution( int i ) {
  const int A = 100;
  const double B = 0.157673137;
  return A * pow(2, B * i);
}

void executeSamples ( int ni ) {

  double quickSelectAvg = 0;
  double heapSelectAvg = 0;
  double medianSelectAvg = 0;

  int * sample = NULL;
  for ( int i = 1; i <= 100; i++ ) {
    sample = MALLOC_ARRAY( ni, int );
    populate( sample, ni );

    // sommatoria tempi esecuzione per un campione dimensione ni
    quickSelectAvg += getExecutionTime(QuickSelect, sample, ni, 0);
    heapSelectAvg += getExecutionTime(HeapSelect, sample, ni, 0);
    medianSelectAvg += getExecutionTime(MedianMediansSelect, sample, ni, 0);

    free( sample );
  }

  // calcolo media tempo esecuzione da 100 campioni di dimensione ni
  quickSelectAvg = quickSelectAvg / 100;
  heapSelectAvg = heapSelectAvg / 100;
  medianSelectAvg = medianSelectAvg / 100;

  appendToCSV( QuickSelect, ni, quickSelectAvg );
  appendToCSV( HeapSelect, ni, heapSelectAvg );
  appendToCSV( MedianMediansSelect, ni, medianSelectAvg );

}

/**
* @brief DA RISCRIVERE
* 
*/
void generateSamples () {

  fptr = fopen("./asymptotic_times.csv", "a");
  int ni;
  for (int i = 0; i <= 99; i++) {

    ni = expDistribution( i );
    executeSamples( ni );
  }
  fclose( fptr );
}

int main () {
  srand(time(NULL));

  // istanzio la var. globale 
  Tmin = getTmin();
  printf(" tmin: %f \n", Tmin);

  // sovrascrivo CSV (tempi medi) e aggiungo le intestazioni
  fptr = fopen("./asymptotic_times.csv", "w");
  fprintf(fptr, "algorithm, size, time\n");
  fclose(fptr);
  
  generateSamples();
  return 0;
}
