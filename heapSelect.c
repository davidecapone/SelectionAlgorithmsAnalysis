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
 * @brief preservare le propietà della min-heap A (versione ricorsiva)
 * 
 * @param A min-heap 
 * @param heapsize dimensione della heap (rimane invariata nella procedura)
 * @param i posizione ove staimo analizzando l'errore nell'albero
 */
void heapify_noninplace( int A[], int heapsize, int i ) {

  int l = left( i );
  int r = right( i );
  int min = i;

  if ( l < heapsize && A[ l ] < A[ min ] ) min = l;
  if ( r < heapsize && A[ r ] < A[ min ] ) min = r;

  if ( min != i ) {
    swap( A, i, min );
    heapify_noninplace( A, heapsize, min );
  }
}

/**
 * @brief preservare le propietà della min-heap A (versione iterativa)
 * 
 * @param A min-heap 
 * @param heapsize dimensione della heap (rimane invariata nella procedura)
 * @param i posizione ove staimo analizzando l'errore nell'albero
 */
void heapify( int A[], int heapsize, int i ) {

  int done = -1;
  
  while( done != 0 ){

    int smallest = i;
    int l = left(i);
    int r = right(i);

    if( l < heapsize && A[ l ] < A[ i ] ){
      smallest = l; // caso in cui scambio con il figlio sinistro
    } else {
      smallest = i;
    }

    if( r < heapsize && A[ r ] < A[ smallest ] ){
      smallest = r; // caso in cui scambio con il figlio destro
    }

    if( smallest != i ){ // se serve lo scambio,qui viene eseguito, altrimenti la procedura termina
      swap(A, i, smallest);
      i = smallest;
    } else {
      done = 0;
    }
  }
}

/**
 * @brief costruire una min-heap a partire da un vettore A non heap
 * 
 * @param A vettore non min-heap
 * @param dim dimensione del vettore A
 */
void buildMinHeap( int A[], int len ) {  

  // da controllare parte basse / parte alta per dim/2
	for ( int i = (len)/2; i >= 0; i-- ) {
		heapify( A, len, i );
	}
}

/**
 * @brief stampa una min heap
 * 
 * @param A min heap
 * @param heapsize dimensione delle heap
 */
void printMinHeap( int A[], int heapsize ) {
  printf("\n MinHeap : \n ");
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
int extractMinHeap( int A[], int* heapsize ){
  
  int root = A[ 0 ];

  swap( A, 0, *heapsize - 1 );
  *heapsize = *heapsize - 1;
  heapify( A, *heapsize, 0);

  return root;
}

/**
 * @brief inserisce una chiave k nel vettore min heap
 * 
 * @param A min heap
 * @param k chiave da inserire
 * @param heapsize dimensione dell'heap (il valore viene modificato, quindi lo passo come riferimento)
 */
void minHeapInsert( int A[], int* heapsize, int k ) {

  *heapsize = *heapsize + 1;
  A[*heapsize-1] = k;
  int i = *heapsize-1; 

  while ( i > 0 && A[i] < A[ parent(i) ] ) {
    swap( A, i, parent( i ) );
    i = parent( i );
  }

}

/**
 * @brief selezione del k-esimo elemento più piccolo
 * 
 * @param H1 una min heap
 * @param heapsize dimensione della min heap H1
 * @param k k-esimo
 * @return int valore del k-esimo elemento
 */
int heapSelect( int H1[], int* heapsize, int k) {
  if ( k >= 0 && k < *heapsize ) {
    int heapsize2 = 0; 
    int H2[ MAX_LINE_SIZE ];
    
    // inizialmente H2 contiene la radice di H1 (posizione 0)
    minHeapInsert( H2, &heapsize2, H1[0] );

    int root_h2;
    for ( int i = 0; i < k; i++ ) {

      // estrazione radice H2
      root_h2 = extractMinHeap( H2, &heapsize2 );  

      // inserimento in H2 dei figli di i a partire da H1, se esistono
      if ( ( 2 * i + 2 ) < *heapsize) {

        minHeapInsert( H2, &heapsize2, H1[ left( i ) ] );
        minHeapInsert( H2, &heapsize2, H1[ right( i ) ] );
      } 
      else if ( ( 2 * i + 1 ) < *heapsize)
        minHeapInsert( H2, &heapsize2, H1[ left( i ) ] );
      
      
    }
    
    // k-esimo elemento
    return H2[ 0 ];
  }

}

int main () {

  // scan sullo standard input per definire il vettore
  int A[MAX_LINE_SIZE];
  int *p = &A[0];
  printf("\033[2J");
  int len = scanArray(p);

  buildMinHeap(A, len);

  printMinHeap(A, len);

  int k;

  while( k != -1 ){
    printf("k = ");
    scanf("%d", &k);
    printf("\nk: %d, k-esimo elemento: %d\n", k, heapSelect(A, &len, k));
  }
  return 0;
}
