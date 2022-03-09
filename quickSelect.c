/**
 * @file quickSelect.c
 * @author Capone, Della Rovere, Gortani Fior
 * @brief 
    Si tratta di una variante dell'algoritmo di ordinamento "quick sort", 
    in cui ogni chiamata ricorsiva su un intervallo [𝑖,𝑗] del vettore fornito in input termina in tempo costante 
    ogniqualvolta il parametro 𝑘 non sia contenuto nell'intervallo [𝑖,𝑗]. 
    L'algoritmo deve avere complessità temporale asintotica Θ(𝑛2) nel caso pessimo e 𝑂(𝑛) nel caso medio, 
    dove 𝑛 è il numero di elementi del vettore.
 * 
 */
#include <stdio.h>
#include <stdlib.h>

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
 * @brief scambia 2 elementi in un vettore
 * 
 * @param A il vettore
 * @param i elemento da scambiare
 * @param j elemento da scambiare
 */
void swap(int A[], int i, int j){
  
  int key = A[i];
  
  A[i] = A[j];
  A[j] = key;
}

/**
 * @brief partizionamento intorno al perno (l'ultimo elemento di A nell'intervallo [p, q])
 *
 * 
 * @param A il vettore
 * @param p posizione iniziale
 * @param q posizione finale
 * @return int: posizione in cui è finito il perno
 */
int partition(int A[], int p, int q){
  
  int i = p-1;
  int x = A[q]; // perno (ultima posizione nell'intervallo [p, q])
  
  for(int j=p; j<=q; j++){  // procedura di partizionamento del vettore intorno al perno
    if(A[j] <= x){
      swap(A, j, i);
      i++;
    }
  }
  return i;
}

int main () {
  // scan sullo standard input per definire il vettore
  int A[MAX_LINE_SIZE];
  int *p = &A[0];
  int l = scanArray(p);

  return 0;
}