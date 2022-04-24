/**
 * @file heapSelect.c
 * @author Capone, Della Rovere, Gortani, Fior
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define MAX_LINE_SIZE 5000 // maximum size of a line of input

/**
 * @brief nodo della heap, contiene attributi chiave e indice
 * 
 */
typedef struct {
    int index;
    int key;
} Node;

/**
 * @brief inserisce in un array gli elementi forniti sullo standard input
 * 
 * @param A vettore
 * @return int lunghezza del vettore costruito
 */
int scanArrayNodes(Node A[])
{
    // scan line of text
    char line[MAX_LINE_SIZE];
    scanf("%[^\n]", line);

    // convert text into array
    int size = 0, offset = 0, numFilled, n;
    do
    {
        numFilled = sscanf(line + offset, "%d%n", &(A[size].key), &n);
        if (numFilled > 0)
        {
            size++;
            offset += n;
        }
    } while (numFilled > 0);

    return size;
}

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
 * @brief preservare le propietà della min-heap A
 *
 * @param A min-heap
 * @param heapsize dimensione della heap
 * @param i posizione ove stiamo analizzando l'errore nell'albero
 */
void heapify(Node A[], int heapsize, int i) {
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

/**
 * @brief costruire una min-heap a partire da un vettore A
 *
 * @param A vettore
 * @param dim dimensione del vettore A
 */
void buildMinHeap(Node A[], int len) {
    for (int i = len / 2; i >= 0; i--) {
        heapify(A, len, i);
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
 * @return radice di H2 estratta
 */
Node extractMinHeap(Node A[], int *heapsize)
{
    Node root = A[0];
    swapNodes(A, 0, *heapsize - 1);
    *heapsize = *heapsize - 1;
    heapify(A, *heapsize, 0);
    return root;
}

/**
 * @brief inserisce una chiave k nel vettore min heap
 *
 * @param A min heap
 * @param k chiave da inserire
 * @param heapsize dimensione dell'heap (il valore viene modificato, quindi lo passo come riferimento)
 */
void minHeapInsert(Node A[], int *heapsize, int key, int index) {
    // aumento la dimensione della heap
    *heapsize = (*heapsize + 1);
    // i: ultimo indice della heap
    int i = (*heapsize - 1);
    A[i].key = key;
    A[i].index = index;

    while (i > 0 && A[i].key < A[ parent(i) ].key) {
        swapNodes(A, i, parent(i));
        i = parent(i);
    }
}

void toNode(int H1[], int size, Node H1_node[]) {
    for (int i = 0; i < size; i++) {
        H1_node[i].key = H1[i];
        H1_node[i]. index = i;
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
int heapSelect(int H1[], int p, int q, int k)
{
    if (k < p || k > q) return INT_MIN;
    else {
        int heapsize = q - p + 1;
        Node H1_node[ heapsize ];

        // conversione dell'array di interi in input in array di Node
        toNode(H1, heapsize, H1_node);
        
        // costruisco una min-heap a partire da H1
        buildMinHeap(H1_node, heapsize);

        // heapsize viene incrementato da minHeapInsert
        int heapsize2 = 0;
        Node H2[k+1];
        
        // inizialmente H2 contiene solamente la radice di H1
        minHeapInsert(H2, &heapsize2, H1_node[0].key, H1_node[0].index);

        Node root_h2;
        
        for (int i = 1; i <= k; i++) {
            // estrazione radice H2
            root_h2 = extractMinHeap(H2, &heapsize2);
            // indici dei figli sinistro e destro
            int leftSon = left(root_h2.index);
            int rightSon = right(root_h2.index);

            // inserimento in H2 dei figli di i a partire da H1, se esistono
            if (rightSon < heapsize) {
                minHeapInsert( H2, &heapsize2, H1_node[leftSon].key, H1_node[leftSon].index );
                minHeapInsert( H2, &heapsize2, H1_node[rightSon].key, H1_node[rightSon].index );
            }
            else if (leftSon < heapsize) 
                minHeapInsert( H2, &heapsize2, H1_node[leftSon].key, H1_node[leftSon].index );
        }

        // k-esimo elemento
        return H2[0].key;
    }
}
