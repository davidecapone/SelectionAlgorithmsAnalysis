/**
 * @file heapSelect.c
 * @author Capone, Della Rovere, Gortani Fior
 * @brief 
    Questo algoritmo di selezione utilizza due min-heap, denominate H1 e H2. 
    La prima heap H1 é costruita a partire dal vettore fornito in input in tempo lineare e non viene modificata. 
    La seconda heap H2 contiene inizialmente un solo nodo, corrispondente alla radice di H1. 
    All'𝑖-esima iterazione, per 𝑖 che va da 1 a 𝑘−1, l'algoritmo estrae la radice di H2, che corrisponde a un nodo 𝑥𝑖 in H1, 
    e reinserisce in H2 i nodi successori (figli sinistro e destro) di 𝑥𝑖 nella heap H1. Dopo 𝑘−1 iterazioni, 
    la radice di H2 corrisponderà al 𝑘-esimo elemento più piccolo del vettore fornito in input.
    L'algoritmo descritto ha complessità temporale 𝑂(𝑛+𝑘𝑙𝑜𝑔𝑘) sia nel caso pessimo che in quello medio. Per 𝑘 sufficientemente piccolo, quindi, l'algoritmo "heap select" sarà preferibile, almeno nel caso pessimo, all'algoritmo "quick select". È possibile implementare una variante che utilizzi opportunamente min-heap o max-heap, a seconda del valore di 𝑘.
 * 
 */
#include <stdio.h>
#define MAX_LINE_SIZE 1000   // maximum size of a line of input

int scanArray(int *A) {
    // scan line of text
    char line[MAX_LINE_SIZE];
    scanf("%[^\n]", line);

    // convert text into array
    int size = 0, offset = 0, numFilled, n;
    do {
        numFilled = sscanf(line + offset, "%d%n", &(A[size]), &n);
        if (numFilled > 0) {
            size++;
            offset += n;
        }
    } while (numFilled > 0);
    
    return size;
}

/**
 * @brief scambia 2 elementi in A
 * 
 * @param A 
 * @param i indice primo elemento
 * @param j indice secondo elemento
 */
void swap( int A[], int i, int j ){
  
  int key = A[ i ];
  
  A[ i ] = A[ j ];
  A[ j ] = key;
}

/**
 * @brief posizione del figlio sinistro
 * 
 * @param i genitore
 * @return int posizione del figlio sinistro
 */
int left( int i ) { return ( 2 * i ) + 1; }

/**
 * @brief posizione del figlio destro
 * 
 * @param i genitore
 * @return int posizione del figlio destro
 */
int right( int i ) { return ( 2 * i ) + 2; }

/**
 * @brief posizione del genitore
 * 
 * @param i figlio destro/sinistro
 * @return int posizione del genitore
 */
int parent( int i ) { return ( i - 1 ) / 2; } // passando all'enumerazione degli indici di c, è necessario togliere 1 all'indice del figlio per ottenere l'indice del genitore

/**
 * @brief preservare le propietà della min-heap A
 * 
 * @param A min-heap 
 * @param heapsize dimensione della heap (rimane invariata nella procedura)
 * @param i posizione in è presente l'errore
 */
void heapify( int A[], int heapsize, int i ) {
  int l = left( i );
  int r = right( i );
  int max = i;

  if ( l < heapsize && A[ l ] > A[ max ] ) max = l;
  if ( r < heapsize && A[ r ] > A[ max ] ) max = r;

  if ( max != i ) {
    swap( A, i, max );
    heapify( A, heapsize, max );
  }
}

/**
 * @brief costruire una min-heap a partire da un vettore A
 * 
 * @param A vettore non min-heap
 * @param dim dimensione della heap
 */
void buildMinHeap( int A[], int len ) {  
  // da controllare parte basse / parte alta per dim/2
	for ( int i = (len)/2; i > 0; i-- ) {
		heapify( A, len, i );
	}
}

void printMinHeap( int A[], int heapsize ) {
  for (int i = 0; i < heapsize; i++) printf( " %d ", A[i] );
  printf("\n");
}

/**
 * @brief estrae la radice dalla min-heap
 * 
 * @param A heap
 * @param heapsize dimensione heap
 * @return int radice estratta dalla min-heap
 */
int extractMinHeap( int A[], int heapsize ){
  
  int radix = A[0];

  swap( A, 0, heapsize - 1 );
  heapsize--;
  heapify( A, heapsize, 0);

  return radix;
}

int main () {
  // scan sullo standard input per definire il vettore
  int A[MAX_LINE_SIZE];
  int *p = &A[0];
  int len = scanArray(p);

  printMinHeap( A, len );
  buildMinHeap( A, len );
  printMinHeap( A, len );
  return 0;
}
