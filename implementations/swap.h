#ifndef SWAP_H
#define SWAP_H

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

#endif