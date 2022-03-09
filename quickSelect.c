/**
 * @file quickSelect.c
 * @author Capone, Della Rovere, Gortani, Fior
 * @brief 
    Si tratta di una variante dell'algoritmo di ordinamento "quick sort", 
    in cui ogni chiamata ricorsiva su un intervallo [𝑖,𝑗] del vettore fornito in input termina in tempo costante 
    ogniqualvolta il parametro 𝑘 non sia contenuto nell'intervallo [𝑖,𝑗]. 
    L'algoritmo deve avere complessità temporale asintotica Θ(𝑛2) nel caso pessimo e 𝑂(𝑛) nel caso medio, 
    dove 𝑛 è il numero di elementi del vettore.
 * 
 */
#include <stdio.h>

/**
 * @brief Selezione del k-esimo elemento
 * 
 * @param A porzione non necessariamente ordinata di interi
 * @param i partenza
 * @param j fine
 * @param k posizione da determinare
 * @return int : k-esimo elemento in un vettore ordinato 
 */
int quickSelect(int A[], int i, int j, int k) {
  /* verifico se k ha senso nella porzione [i, j] del vettore */
  if (( k >= i ) && ( k <= j )) {

    /* vettore di 1 elemento */
    if ( i == j ) {
      return A[i];
    }

    /* chiamata Select determinare mediano dei mediani M  */
    int M = Select();

    /* partizionamento intorno ad M */
    int perno = Partition(A, i, j, M);

    if ( k == perno ) return A[k];
    else if ( k < perno ) return quickSelect(A, i, perno-1);
    else return quickSelect(A, perno+1, j);
  }
}

int main () {

  return 0;
}