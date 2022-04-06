#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <string.h>
#include "implementations/quickSelect.c"
#include "implementations/heapSelect.c"
#include "implementations/medianMediansSelect.c"

#define MALLOC_ARRAY(number, type)\
	((type*) malloc((number) *sizeof(type)))

// filename
char random_k_file[] = {"dataset/random_k_times.csv"};
char fixed_k_file[] = {"dataset/fixed_k_times.csv"};
char worst_case_qk[] = {"dataset/worst_case_qk.csv"};

double E = 0.001;
double Tmin;

const int A = 100;
const double B = 0.157673137;

typedef enum {
  Sorted,
  Random
} ArrayType;

typedef enum {
	QuickSelect,
	HeapSelect,
	MedianMediansSelect
} Algorithm;

typedef enum {
  QuickSelectWorst,
  FixedK,
  RandomK
} Analysis;

double duration(struct timespec start, struct timespec end) {

	return end.tv_sec - start.tv_sec +
		((end.tv_nsec - start.tv_nsec) / (double) 1000000000.0);
}

double getResolution() {

	struct timespec start, end;
	clock_gettime(CLOCK_MONOTONIC, &start);
	do {
		clock_gettime(CLOCK_MONOTONIC, &end);
	} while (duration(start, end) == 0.0);
	return duration(start, end);
}

/**
 * @brief Ottenere il tempo minimo misurabile dalla macchina in base al'errore minimo ammissibile
 * 
 * @return double : tempo minimo misurabile in secondi
 */
double getTmin() {

	double R = getResolution();
	return (R*(1/E+1));
}

/**
 * @brief Generare dimensioni per array che crescono esponenzialmente
 * 
 * @param i numero da usare come esponente
 * @return int : dimensione 
 */
int expDistribution(int i) {

	return ( A * pow(2, B*i) );
}

/**
 * @brief Generare numeri interi pseudocasuali
 * 
 * @return int : numero compreso tra -INT_MAX e +INT_MAX
 */
int randomInt() {

  int randomUnsigned = rand();
  int sign = rand() % 2;
  return ( sign == 0 ) ? ( -1 * randomUnsigned ) : randomUnsigned;
}

/**
 * @brief Popola l'array di interi
 * 
 * @param A array da popolare
 * @param size dimensione array
 * @param type tipo di riempimento
 */
void populate( int A[], int size, ArrayType type ) {

  if ( type == Sorted ) {
    for (int i = 0; i < size; i++) {
      A[i] = i;
    }
  } else {
    for (int i = 0; i < size; i++) {
      A[i] = randomInt();
    }
  }
}

/**
 * @brief Calcola il tempo di esecuzione del campione
 * 
 * @param type tipo di algoritmo
 * @param A array
 * @param size dimensione array
 * @param k posizione da determinare
 * @return int : Tempo di esecuzione in secondi
 */
double timeExecution( Algorithm type, int A[], int size, int k ) {

  struct timespec start, end;
  struct timespec backup_start, backup_end;
  double period;
  double backupTime = 0;
  int count = 0;
  int kSmallest;

  /*
  creo una copia di backup dell'array A
  in questo modo, se il ciclo effettua più iterazioni:
  applico gli algoritmi di selezione sull'istanza di vettore originale
  per evitare di calcolare i tempi di esecuzione su istanze più ordinate
  */

  int * copy = NULL;
  copy = MALLOC_ARRAY(size, int);
  memcpy(copy, A, size*sizeof(int));

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
				kSmallest = MoMSelect(A, 0, size-1, k);
				break;
		}

		clock_gettime(CLOCK_MONOTONIC, &end);
		period = duration(start, end);

    if (period <= Tmin) {
      // A sarà la vecchia istanza di A (di cui abbiamo fatto il backup)
      clock_gettime(CLOCK_MONOTONIC, &backup_start);
      memcpy(A, copy, size*sizeof(int));
      clock_gettime(CLOCK_MONOTONIC, &backup_end);
      backupTime += duration(backup_start, backup_end);
    }
    
		count++;
	} while (period <= Tmin);


  free(copy);
	return ((double) ((period - backupTime) / count));
}

/**
 * @brief Compila il file con i dati 
 * 
 * @param analysisType tipo di analisi
 * @param size dimensione degli array
 * @param nSamples numero di array
 * @param k posizione da determinare
 * @param ptr puntatore al file
 */
void executeSamples( Analysis analysisType, int size, int nSamples, int k, ArrayType type, FILE * ptr ) {
  
	int *sample = NULL;
  double quickSelectTime;
  double heapSelectTime;
  double medianSelectTime;

  // creare 100 campioni dimensione size, calcolare tempo medio esecuzione
  for (int i = 1; i <= nSamples; i++) {
    sample = MALLOC_ARRAY(size, int);

    // popolamento ordinato o pseudocasuale del vettore
    populate(sample, size, type);

    if (analysisType == QuickSelectWorst) {
      quickSelectTime = timeExecution( QuickSelect, sample, size, k );
      printf("\t size: %d, quickselect %f\n", size, quickSelectTime);
      fprintf(ptr, "quickSelect, %d, %f\n", size, quickSelectTime);
    } else {
      quickSelectTime = timeExecution( QuickSelect, sample, size, k );
      heapSelectTime = timeExecution( HeapSelect, sample, size, k );
      medianSelectTime = timeExecution( MedianMediansSelect, sample, size, k );

      printf("\t size: %d, quickselect %f, heapselect %f, medianmediansselect %f\n", size, quickSelectTime, heapSelectTime, medianSelectTime);
      fprintf(ptr, "quickSelect, %d, %f\n", size, quickSelectTime);
      fprintf(ptr, "heapSelect, %d, %f\n", size, heapSelectTime);
      fprintf(ptr, "medianMediansSelect, %d, %f\n", size, medianSelectTime);
    }
    
    free(sample);
  }
}

/**
 * @brief Inizializza il file dei dati e lo compila
 * 
 * @param type tipo di analisi
 * @param nSamples numero di array
 */
void analysis( Analysis type, int nSamples ) {

  int size, k;
  FILE * ptr;
  ArrayType populationMode;

  if (type == QuickSelectWorst) { 
    populationMode = Sorted;
    ptr = fopen(worst_case_qk, "w");
    fprintf(ptr, "algorithm, size, time\n");
	  fclose(ptr);
    ptr = fopen(worst_case_qk, "a");
  } 
  else { 
    populationMode = Random; 
    if (type == FixedK) {
      ptr = fopen(fixed_k_file, "w");
      fprintf(ptr, "algorithm, size, time\n");
	    fclose(ptr);
      ptr = fopen(fixed_k_file, "a");
    } 
    else {
      ptr = fopen(random_k_file, "w");
      fprintf(ptr, "algorithm, size, time\n");
	    fclose(ptr);
      ptr = fopen(random_k_file, "a");
    }
  }
  
  for (int i = 0; i <= 99; i++) {
    // dimensione dei campioni (segue dist. exp. al variare di i)
    size = expDistribution( i );

    if (type == FixedK || type == QuickSelectWorst) k = 0; 
    else k = rand() % 100;

    // tempi d'esecuzione su nSamples campioni di dimensione size
    executeSamples( type, size, nSamples, k, populationMode, ptr );
  }

  fclose(ptr);
}

int main () {
	
  srand(time(NULL));
  Tmin = getTmin();
  int nSamples = 20;
  /**
  Analisi asintotica degli algoritmi di selezione
  con k = 0, fisso
  20 campioni per ogni dimensione generata
  */
  printf("[starting analysis] fixed k, %d samples for each dimension\n", nSamples);
  analysis( FixedK, nSamples );

  /**
  Analisi asintotica degli algoritmi di selezione
  con k variabile tra 0 e 99
  20 campioni per ogni dimensione generata
  */
  printf("[starting analysis] random k, %d samples for each dimension\n", nSamples);
  analysis( RandomK, nSamples );

  /**
  Analisi asintotica quickSelect nel caso peggiore
  con k = 0, fisso, vettore ordinato
  20 campioni per ogni dimensione generata
  */
  printf("[starting analysis] quick select worst case, %d samples for each dimension\n", nSamples);
  analysis( QuickSelectWorst, nSamples );

  return (EXIT_SUCCESS);
}