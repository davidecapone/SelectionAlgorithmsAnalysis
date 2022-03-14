/**
 * @file quickSelect.c
 * @author Capone, Della Rovere, Gortani, Fior
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
 * @brief partizionamento intorno al perno (l'ultimo elemento di A nell'intervallo [p, q])
 *
 * 
 * @param A il vettore
 * @param p posizione iniziale
 * @param q posizione finale
 * @return int: posizione in cui è finito il perno
 */
int partition( int A[], int p, int q ) {
  
  int i = p;
  int perno = A[ q ]; // perno (ultima posizione nell'intervallo [p, q])
  
  for( int j = p; j < q; j++ ) {  // procedura di partizionamento del vettore intorno al perno
    if( A[ j ] < perno ) {
      swap( A, j, i );
      i++;
    }
  }

  swap( A, i, q );  // il perno si trova ancora in posizione q, viene spostato nella posizione in cui dovrebbe stare se il vettore fosse ordinato
  
  return i;
}

/**
 * @brief Selezione del k-esimo elemento
 * 
 * @param A porzione non necessariamente ordinata di interi
 * @param i indice sx
 * @param j indice dx
 * @param k posizione da determinare
 * @return int : k-esimo elemento in un vettore ordinato, -1 se non k non ha senso
 */
int quickSelect( int A[], int i, int j, int k ) {
  /* verifico se k ha senso nella porzione [i, j] del vettore */
  if (( k >= i ) && ( k <= j )) {

    /* vettore di 1 elemento */
    if ( i == j ) return A[ i ];

    /* partizionamento intorno all'ultimo elemento in A, ottengo la posizione in cui è finito */
    int perno = partition(A, i, j);

    if ( k == perno ) return A[ k ];
    else if ( k < perno ) return quickSelect( A, i, perno-1, k );
    else return quickSelect( A, perno+1, j, k );
  } 
  else {
    return -1;
  }
}

int main () {
  // scan sullo standard input per definire il vettore
  int A[ MAX_LINE_SIZE ];
  int *p = &A[ 0 ];
  int len = scanArray( p );

  int kSmallest;
  scanf( "%d", &kSmallest );

  printf( "%d\n", quickSelect( A, 0, len-1, kSmallest ));

  return 0;
}