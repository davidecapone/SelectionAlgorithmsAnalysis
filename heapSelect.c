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
void swap(int A[], int i, int j){
  
  int key = A[i];
  
  A[i] = A[j];
  A[j] = key;
}

/**
 * @brief preservare le propietà della min-heap A
 * 
 * @param A min-heap 
 * @param heapsize dimensione della heap (rimane invariata nella procedura)
 * @param i posizione in è presente l'errore
 */
void heapify( int A[], int heapsize, int i ) {
  int left = left( i );
  int right = right( i );
  int max = i;

  if (left < heapsize && A[ left ] > A[ max ]) max = left;
  if (right < heapsize && A[ right ] > A[ max ]) max = right;

  if (max != i) {
    swap(A, i, max);
    heapify(A, heapsize, max);
  }
}

/**
 * @brief costruire una min-heap a partire da un vettore A
 * 
 * @param A vettore non min-heap
 * @param dim dimensione della heap
 */
void buildMinHeap( int A[], int dim ) {}

/**
 * @brief chiave del figlio sinistro
 * 
 * @param A heap
 * @param i genitore
 * @return int chiave del figlio sinistro
 */
int left( int A[], int i ) { return A[ ( 2 * i ) + 1 ]; }

/**
 * @brief chiave del figlio destro
 * 
 * @param A heap
 * @param i genitore
 * @return int chiave del figlio destro
 */
int right( int A[], int i ) { return A[ ( 2 * i ) + 2 ]; }

/**
 * @brief chiave del genitore
 * 
 * @param A heap
 * @param i figlio destro/sinistro
 * @return int chiave del genitore
 */
int parent( int A[], int i ) { return A[ ( i - 1 ) / 2 ]; } // passando all'enumerazione degli indici di c, è necessario togliere 1 all'indice del figlio per ottenere l'indice del genitore

int main () {
  // scan sullo standard input per definire il vettore
  int A[MAX_LINE_SIZE];
  int *p = &A[0];
  int l = scanArray(p);

  return 0;
}