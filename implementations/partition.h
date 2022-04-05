#ifndef PARTITION_H
#define PARTITION_H

#include "swap.h"
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

#endif
