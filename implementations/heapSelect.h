#ifndef HEAPSELECT
#define HEAPSELECT

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_LINE_SIZE 5000 // maximum size of a line of input
#define ALLOC_ARRAY(number, type)\
	(type*) calloc((number) *sizeof(type), sizeof(type))
    
/**
 * @brief nodo della heap, contiene attributi chiave e indice
 * 
 */
typedef struct {
    int index;
    int key;
} Node;

typedef enum {
    MaxHeap,
    MinHeap
} Heap;

/**
 * @brief posizione del figlio sinistro
 *
 * @param i genitore
 * @return int posizione del figlio sinistro
 */
int left(int i) { return (2 * i) + 1; }

/**
 * @brief posizione del figlio destro
 *
 * @param i genitore
 * @return int posizione del figlio destro
 */
int right(int i) { return (2 * i) + 2; }

/**
 * @brief posizione del genitore
 *
 * @param i figlio destro/sinistro
 * @return int posizione del genitore
 */
int parent(int i) { return (i - 1) / 2; } 

/**
 * @brief scambia 2 elementi in A
 *
 * @param A vettore
 * @param i indice primo elemento
 * @param j indice secondo elemento
 */
void swapNodes(Node A[], int i, int j) {
    Node temp = A[i];
    A[i] = A[j];
    A[j] = temp;
}

/**
 * @brief preservare le propietà della heap A
 *
 * @param A heap
 * @param heapsize dimensione della heap
 * @param i posizione ove stiamo analizzando l'errore nell'albero
 * @param type tipo della heap (MaxHeap o MinHeap)
 */
void heapify(Node A[], int heapsize, int i, Heap type)
{
    if (type == MaxHeap) {
        int done = -1;
        while (done != 0) {
            int biggest = i;
            int l = left(i);
            int r = right(i);

            if (l < heapsize && A[l].key > A[i].key) biggest = l; 
            else biggest = i;
            if (r < heapsize && A[r].key > A[biggest].key) biggest = r;

            if (biggest != i) {
                swapNodes(A, i, biggest);
                i = biggest;
            }
            else done = 0;

        }
    } else if (type == MinHeap) {
        int done = -1;
        while (done != 0) {
            int smallest = i;
            int l = left(i);
            int r = right(i);

            if (l < heapsize && A[l].key < A[i].key) smallest = l;
            else smallest = i;
            if (r < heapsize && A[r].key < A[smallest].key) smallest = r;
            
            if (smallest != i) {
                swapNodes(A, i, smallest);
                i = smallest;
            }
            else done = 0;
        }
    }
}

/**
 * @brief costruire heap a partire da vettore non heap
 *
 * @param A vettore non heap
 * @param dim dimensione vettore A
 * @param type tipo heap
 */
void buildHeap(Node A[], int len, Heap type) {
    for (int i = len / 2; i >= 0; i--) {
        heapify(A, len, i, type);
    }

    // assegnazione indici ai nodi
    for (int i = 0; i < len; i++) {
        A[i].index = i;
    }
}

/**
 * @brief estrazione radice della heap
 *
 * @param A heap
 * @param heapsize dimensione heap
 * @param type tipo heap
 * @return radice
 */
Node extractHeap(Node A[], int *heapsize, Heap type) {
    Node root = A[0];
    swapNodes(A, 0, *heapsize - 1);
    *heapsize = *heapsize - 1;
    heapify(A, *heapsize, 0, type);
    return root;
}

/**
 * @brief inserisce chiave nella heap
 *
 * @param A heap
 * @param heapsize dimensione heap (in seguito all'aggiunta viene aumentata)
 * @param k chiave
 * @param index ??
 * @param type tipo heap
 */
void heapInsert(Node A[], int *heapsize, int key, int index, Heap type) {
    // aumenta la dimensione:
    *heapsize = (*heapsize + 1);

    // i: ultimo indice della heap
    int i = (*heapsize - 1);

    A[i].key = key;
    A[i].index = index;
    if(type == MinHeap) {
        while (i > 0 && A[i].key < A[ parent(i) ].key) {
            swapNodes(A, i, parent(i));
            i = parent(i);
        }
    } else if (type == MaxHeap) {
        while (i > 0 && A[i].key > A[parent(i)].key) {
            swapNodes(A, i, parent(i));
            i = parent(i);
        }
    }
}

/**
 * @brief selezione del k-esimo elemento più piccolo
 *
 * @param H1 vettore fornito in input
 * @param p indice inizale
 * @param q indice finale
 * @param k indice del k-esimo elemento da trovare
 * @return int elemento in posizione k se H1 fosse ordinato
 */
int heapSelect(Node H1[], int p, int q, int k, Heap type) {
    if (k < p || k > q) return INT_MIN;

    int heapsize = q - p + 1;
    buildHeap(H1, heapsize, type);          // costruisco una heap da H1
    int heapsize2 = 0;                      // H2 vuota
    Node *H2 = NULL;
    Node root_h2;
    int size, end;

    if(type == MinHeap) { size = k+1; end = k; }
    else if (type == MaxHeap) { size = heapsize-k; end = heapsize-k-1; }

    H2 = ALLOC_ARRAY(size, Node);
    heapInsert(H2, &heapsize2, H1[0].key, H1[0].index, type);   // inizialmente H2 contiene la radice di H1

    for (int i = 1; i <= end; i++) {
        // estrazione radice H2
        root_h2 = extractHeap(H2, &heapsize2, type);
        // indici dei figli sinistro e destro
        int leftSon = left(root_h2.index);
        int rightSon = right(root_h2.index);

        // inserimento in H2 dei figli di i a partire da H1, se esistono
        if (rightSon < heapsize) {
            heapInsert( H2, &heapsize2, H1[leftSon].key, H1[leftSon].index, type);
            heapInsert( H2, &heapsize2, H1[rightSon].key, H1[rightSon].index, type);
        }
        else if (leftSon < heapsize) 
            heapInsert( H2, &heapsize2, H1[leftSon].key, H1[leftSon].index, type);
    }

    int var = H2[0].key;
    free(H2);
    // k-esimo elemento
    return var;
}
#endif 
