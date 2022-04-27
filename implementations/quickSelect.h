#ifndef QUICKSELECT
#define QUICKSELECT
#include <limits.h>
#include "scanArray.h"
#include "partition.h"
#include "swap.h"

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
    
    return INT_MIN;
  }
}
#endif 