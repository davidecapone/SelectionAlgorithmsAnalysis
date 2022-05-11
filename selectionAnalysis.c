#include <time.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "implementations/quickSelect.h"
#include "implementations/heapSelect.h"
#include "implementations/medianMediansSelect.h"

#define ALLOC_ARRAY(number, type)\
	(type*) calloc((number) *sizeof(type), sizeof(type))

const double E = 0.001;
double Tmin;

int A = 100;
double B = 0.157673137;

// tipo di ordinamento
typedef enum {
  True,
  False
} ArrayOrdered;

// algoritmi di selezione
typedef enum {
	QuickSelect,
	MinHeapSelect,
  MaxHeapSelect,
	MedianMediansSelect
} Algorithm;

// analisi effettuate
typedef enum {
  square_n,               // k = sqrt(n)
  half_n,                 // k = n/2
  hundred_n,              // k = n/100
  random_k,               // k random
  quickselect_worstcase,  // k = 0
  static_size             // k crescente
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
 * @brief ottenere tempo minimo misurabile in base al'errore minimo ammissibile E
 * 
 * @return double tempo minimo misurabile (secondi)
 */
double get_t_min() {
	double R = getResolution();
	return (R*(1/E+1));
}

/**
 * @brief generare interi da una distribuzione esponenziale
 *        variabili A, B definite globalmente
 *        segue la formula: A * 2^( B * i )
 * @param i indice
 * @return int : dimensione ottenuta
 */
int exp_distribution(int i) {
	return ( A * pow(2, B*i) );
}

/**
 * @brief genera numeri (interi) pseudocasuali
 * @return int : numero compreso tra -INT_MAX e +INT_MAX
 */
int get_random_int() {
  int randomUnsigned = rand();
  int sign = rand() % 2;
  return ( sign == 0 ) ? ( -1 * randomUnsigned ) : randomUnsigned;
}

/**
 * @brief popola un vettore di interi
 * @param A vettore
 * @param size dimensione vettore
 * @param order tipo popolamento (True: ordinato, False: random)
 */
void populate( int A[], int size, ArrayOrdered order ) {
  if ( order == False ) for (int i = 0; i < size; i++) A[i] = get_random_int();
  else for (int i = 0; i < size; i++) A[i] = i;
}

/**
 * @brief conversione vettore di interi in vettore di nodi
 * @param A vettore di interi da convertire
 * @param size dimensione vettore
 * @param A_node vettore convertito
 */
void toNode(int A[], int size, Node A_node[]) {
    for (int i = 0; i < size; i++) {
        A_node[i].key = A[i];
        A_node[i].index = 0;
    }
}

/**
 * @brief calcola tempo di esecuzione per un campione
 * @param type scelta dell'algoritmo di selezione
 * @param A vettore, campione
 * @param size dimensione vettore
 * @param k parametro di selezione
 * @return int tempo esecuzione
 */
double get_execution_time( Algorithm type, int A[], int size, int k ) {
  struct timespec start, end;
  struct timespec backup_start, backup_end;
  double period = 0.0;
  double backupTime = 0.0;

  // n.ro di iterazioni (misurazioni) effettuate sul campione:
  int count = 0;

  // backup del campione in 'copy':
  int *copy = NULL;
  copy = ALLOC_ARRAY(size, int);
  memcpy(copy, A, size*sizeof(int));

  // creazione array di nodi per heapSelect
  Node *A_node = NULL;
  if(type == MinHeapSelect || type == MaxHeapSelect) {
    A_node = ALLOC_ARRAY(size, Node);
    toNode(A, size, A_node);
  }

  int kSmallest;
	clock_gettime(CLOCK_MONOTONIC, &start);
	do {
		switch (type) {
			case QuickSelect:
				kSmallest = quickSelect(A, 0, size-1, k);
				break;
			case MinHeapSelect:
				kSmallest = heapSelect(A_node, 0, size-1, k, MinHeap);
				break;
      case MaxHeapSelect:
				kSmallest = heapSelect(A_node, 0, size-1, k, MaxHeap);
				break;
			case MedianMediansSelect:
				kSmallest = MoMSelect(A, 0, size-1, k);
				break;
      default:
        break;
		}

		clock_gettime(CLOCK_MONOTONIC, &end);
		period = duration(start, end);

    // A sarà la vecchia istanza di A (vecchia copia):
    if (period <= Tmin) {
      clock_gettime(CLOCK_MONOTONIC, &backup_start);
      memcpy(A, copy, size*sizeof(int));
      if(type == MinHeapSelect || type == MaxHeapSelect){
        toNode(A, size, A_node);
      }
      clock_gettime(CLOCK_MONOTONIC, &backup_end);

      // TEMPO EFFETTUATO PER IL BACKUP, da sottrarre al termine:
      backupTime += duration(backup_start, backup_end);
    }
    
		count++;
	} while (period <= Tmin);

  free(copy);
  if(type == MinHeapSelect || type == MaxHeapSelect) free(A_node);
	return ((double) ((period - backupTime) / count));
}

/**
 * @brief effettua l'analisi per n_samples campioni e scrive i tempi ottenuti su file
 * @param type analisi asintotica effettuata
 * @param size dimensione dei campioni
 * @param n_samples numero di campioni
 * @param k parametro di selezione
 * @param ptr puntatore al file
 */
void execute_samples( Analysis type, int size, int n_samples, ArrayOrdered order, FILE * ptr ) {
  double quickSelectTime = 0, heapSelectTime = 0, medianSelectTime = 0;
	int *sample = NULL;
  int *backup = NULL;
  sample = ALLOC_ARRAY(size, int);
  backup = ALLOC_ARRAY(size, int);
  int k;

  // scelta parametro k (per k random, scelta effettuata per ogni campione di dimensione size)
  if (type == quickselect_worstcase) k = 0;
  else if (type == half_n) k = size / 2;
  else if (type == square_n) k = sqrt(size);
  else if (type == hundred_n) k = size/100;

  for (int i = 1; i <= n_samples; i++) {
    populate(sample, size, order);
    memcpy(backup, sample, size*sizeof(int));

    if (type == random_k) k = rand() % size;
    if ( type == quickselect_worstcase ) {

      quickSelectTime = get_execution_time( QuickSelect, sample, size, k );
      fprintf(ptr, "%d, %f\n", size, quickSelectTime);
      printf("size: %d, quickSelect: %f\n", size, quickSelectTime);

    } else {

      quickSelectTime = get_execution_time( QuickSelect, sample, size, k );
      memcpy(sample, backup, size*sizeof(int));

      heapSelectTime = get_execution_time( MinHeapSelect, sample, size, k );
      memcpy(sample, backup, size*sizeof(int));

      medianSelectTime = get_execution_time( MedianMediansSelect, sample, size, k );

      fprintf(ptr, "%d, %f, %f, %f\n", size, quickSelectTime, heapSelectTime, medianSelectTime);
      printf("size: %d, quickSelect: %f - heapSelect: %f - medianMediansSelect: %f \n", size, quickSelectTime, heapSelectTime, medianSelectTime);
    
    }
  }

  free(sample);
  free(backup);
}

/**
 * @brief imposta le intestazioni e apre il file in append in base all'analisi scelta
 * @param type tipo di analisi da effettuare
 * @return FILE* puntatore al file (aperto in append) 
 */
FILE * setup_csv ( Analysis type ) {
  char filename[50];
  FILE * ptr;
  switch (type) {
    case square_n:
      strcpy(filename, "dataset/square_n.csv");
      break;
    case half_n:
      strcpy(filename, "dataset/half_n.csv");
      break;
    case random_k:
      strcpy(filename, "dataset/random_k.csv");
      break;
    case quickselect_worstcase:
      strcpy(filename, "dataset/quickselect_worst.csv");
      break;
    case static_size:
      strcpy(filename, "dataset/static_size.csv");
      break;
    case hundred_n:
      strcpy(filename, "dataset/hundred_n.csv");
      break;
    default:
      break;
  }

  // intestazioni file csv:
  ptr = fopen(filename, "w");
  if (type == quickselect_worstcase) fprintf(ptr, "size,quickselect\n");
  else if (type == static_size) fprintf(ptr, "k,quickselect,minheapselect,maxheapselect,medianmediansselect\n");
  else fprintf(ptr, "size,quickselect,heapselect,medianmediansselect\n");
  fclose(ptr);

  // apertura in append:
  ptr = fopen(filename, "a");
  return ptr;
}

/**
 * @brief ottiene la dimensione dei campioni e fa partire l'analisi
 *        imposta inoltre la dimensione finale da raggiungere
 * @param type analisi da effettuare
 * @param n_samples numero di campioni da generare
 */
void analysis( Analysis type, int n_samples ) {
  // dimensione finale (default) da raggiungere: 5 milioni
  int size, final = 99;

  // apertura del file e aggiunta delle intestazioni necessarie:
  FILE * ptr = setup_csv(type);

  ArrayOrdered order = False ;

  /**
   * caso pessimo di quick select
   *  - ordinamento crescente del vettore
   *  - riduciamo la dimensione finale da raggiungere
  */
  if (type == quickselect_worstcase) {
    order = True;
    final = 39;
  }

  for (int i = 0; i <= final; i++) {
    size = exp_distribution( i );
    execute_samples( type, size, n_samples, order, ptr );
  }

  fclose(ptr);
}

/**
 * @brief analisi aggiuntiva: k crescente e dimensione fissa
 * @param size dimensione vettore
 * @param threshold limite superiore per parametro k crescente
 * @param n_samples campioni da generare per ogni dimensione
 */
void analysis_static_size(int size, int threshold, int n_samples) {
  double quickSelectTime, minHeapSelectTime, maxHeapSelectTime, medianSelectTime;
  int *sample = NULL;
  int *backup = NULL;
  
  sample = ALLOC_ARRAY(size, int);
  backup = ALLOC_ARRAY(size, int);      // vettore copia

  FILE * ptr = setup_csv(static_size);
  ArrayOrdered order = False;

  for (int i = 0; i <= threshold; i++) {
    for(int j = 0; j < n_samples; j++) {
      populate(sample, size, order);
      memcpy(backup, sample, size*sizeof(int));
      // k crescente, k = i
      quickSelectTime = get_execution_time( QuickSelect, sample, size, i );
      memcpy(sample, backup, size*sizeof(int));

      minHeapSelectTime = get_execution_time( MinHeapSelect, sample, size, i );
      memcpy(sample, backup, size*sizeof(int));

      maxHeapSelectTime = get_execution_time( MaxHeapSelect, sample, size, i );
      memcpy(sample, backup, size*sizeof(int));

      medianSelectTime = get_execution_time( MedianMediansSelect, sample, size, i );

      fprintf(ptr, "%d, %f, %f, %f, %f\n", i, quickSelectTime, minHeapSelectTime, maxHeapSelectTime, medianSelectTime);
      printf("k: %d, quickSelect: %f - minHeapSelect: %f - maxHeapSelect: %f - medianMediansSelect: %f \n", i, quickSelectTime, minHeapSelectTime, maxHeapSelectTime, medianSelectTime);
    }    
  }
  fclose(ptr);
}

int main () {
  srand(time(NULL));
  printf("\e[1;1H\e[2J");
  Tmin = get_t_min();

  int n_samples = 40;   // n.ro di campioni per ogni dimensione
  
  // analisi k = sqrt(n)
  analysis(square_n, n_samples);

  // analisi k = n/2
  analysis(half_n, n_samples);

  // analisi k = n/100
  analysis(hundred_n, n_samples);

  // k = random
  analysis(random_k, n_samples);

  // caso pessimo quick select
  // analysis(quickselect_worstcase, n_samples);

  // analisi tempi di esecuzione con dimensione fissata e k crescente
  int size = 5000;          // dimensione vettore
  int threshold = size-1;   // limite massimo per k
  analysis_static_size(size, threshold, 30);

  return (EXIT_SUCCESS);
}