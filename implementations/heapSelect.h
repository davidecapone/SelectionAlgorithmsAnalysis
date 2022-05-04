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
    if(type == MaxHeap)
    {

        int done = -1;

        while (done != 0)
        {

            int biggest = i;
            int l = left(i);
            int r = right(i);

            if (l < heapsize && A[l].key > A[i].key)
            {
                biggest = l; // caso in cui scambio con il figlio sinistro
            }
            else
            {
                biggest = i;
            }

            if (r < heapsize && A[r].key > A[biggest].key)
            {
                biggest = r; // caso in cui scambio con il figlio destro
            }

            if (biggest != i)
            { // se serve lo scambio,qui viene eseguito, altrimenti la procedura termina
                swapNodes(A, i, biggest);
                i = biggest;
            }
            else
            {
                done = 0;
            }
        }
    } else {
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
 * @brief costruire una min-heap a partire da un vettore A
 *
 * @param A vettore
 * @param dim dimensione del vettore A
 * @param type tipo della heap (MinHeap o MaxHeap)
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
 * @brief estrae la radice dalla min-heap
 *
 * @param A heap
 * @param heapsize dimensione heap
 * @param type tipo della heap (MaxHeap o MinHeap)
 * @return radice di H2 estratta
 */
Node extractHeap(Node A[], int *heapsize, Heap type)
{
    Node root = A[0];
    swapNodes(A, 0, *heapsize - 1);
    *heapsize = *heapsize - 1;
    heapify(A, *heapsize, 0, type);
    return root;
}

/**
 * @brief inserisce una chiave k nel vettore min heap
 *
 * @param A min heap
 * @param k chiave da inserire
 * @param heapsize dimensione dell'heap (il valore viene modificato, quindi lo passo come riferimento)
 * @param type tipo della heap (MaxHeap o MinHeap)
 */
void heapInsert(Node A[], int *heapsize, int key, int index, Heap type) {
    // aumento la dimensione della heap
    *heapsize = (*heapsize + 1);
    // i: ultimo indice della heap
    int i = (*heapsize - 1);
    A[i].key = key;
    A[i].index = index;

    if(type == MinHeap){
        while (i > 0 && A[i].key < A[ parent(i) ].key) {
        swapNodes(A, i, parent(i));
        i = parent(i);
        }
    } else {
        while (i > 0 && A[i].key > A[parent(i)].key) {
        swapNodes(A, i, parent(i));
        i = parent(i);
        }
    }
}

/**
 * @brief selezione del k-esimo elemento più piccolo
 *
 * @param H1 una min heap
 * @param p indice inizale
 * @param p indice inizale
 * @param k indice del k-esimo elemento
 * @return int chiave del k-esimo elemento
 */
int heapSelect(Node H1[], int p, int q, int k, Heap type)
{
    if (k < p || k > q) return INT_MIN;
    else {
        int heapsize = q - p + 1;
        
        // costruisco una min-heap a partire da H1
        buildHeap(H1, heapsize, type);

        // heapsize viene incrementato da minHeapInsert
        int heapsize2 = 0;
        Node *H2 = NULL;
        H2 = ALLOC_ARRAY(k+1, Node);

        // inizialmente H2 contiene solamente la radice di H1
        heapInsert(H2, &heapsize2, H1[0].key, H1[0].index, type);

        Node root_h2;

        // stabilisco quando deve terminare l'algoritmo
        int end;
        if(type == MinHeap){
            end = k;
        } else {
            end = heapsize - k - 1;
        }
        
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
}
#endif 
