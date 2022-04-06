/** 
 * @file medianMedians.c 
 * @author Capone, Della Rovere, Gortani, Fior 
 */ 
#include <stdio.h>
#include <math.h> 
#include <limits.h>
#include "scanArray.h"
#include "swap.h"

int MoMSelect(int arr[], int k, int p, int q);
int MoM(int arr[], int p, int q);
int MoMPlace(int arr[], int p, int q);
int MoMPartition(int arr[], int k, int p, int q, int posPerno);
int med(int arr[], int p, int q);
void insertionSort(int arr[], int p, int q );


/** 
 * @brief: trova l'indice di k in arr se arr fosse ordinato  
 *  
 * @param arr: array
 * @param k: posizione data in input
 * @param p: prima posizione della parte di vettore considerata
 * @param q: ultima posizione della parte di vettore considerata
 * @return int: posizione di k se arr fosse ordinato
 */ 
int MoMSelect(int arr[], int k, int p, int q){ 
 
  if( k < p || k > q){
    return INT_MIN;
  }

  //caso base della ricorsione: 1 elemento 
  if(p == q){ 
    return arr[p]; 
  } 
  
  //calcolo la posizione del perno (MoM) e partiziono l'array
  int posPerno = MoMPlace(arr, p, q);
  posPerno = MoMPartition(arr, k, p, q, posPerno); 
 
  /*posPerno a questo punto può essere:
  * -uguale a k: ho finito
  * -minore/maggiore di k: ricorsione nella parte di array contentente k
  *                        escludendo la posizione del perno
  */ 
 
  if(k == posPerno){ 
    return arr[k]; 
  } else if(k < posPerno){ 
    q = posPerno - 1; 
    return MoMSelect(arr, k, p, q); 
  } else { 
    p = posPerno + 1; 
    return MoMSelect(arr, k, p, q); 
  } 
} 

/** 
 * @brief: trova la posizione del mediano dei mediani (versione in place)
 *  
 * @param arr: array  
 * @param p: prima posizione della parte di vettore considerata
 * @param q: ultima posizione della parte di vettore considerata
 * @return int: posizione (indice) del mediano dei mediani
 */ 
int MoMPlace(int arr[], int p, int q){

  if( q-p < 5){
    return med(arr, p, q);   
  } 

  //mettiamo i mediani dei blocchi da 5 nelle prime n/5 posizioni dello stesso array
  int sLimit; //section limit: indice che definisce il blocco da 5 elementi

  for(int i = p; i <= q; i = i+5){ 
    sLimit = i+4;  
    //controllo di non andare oltre l'ultima posizione del vettore
    if(sLimit > q){  
      sLimit = q;
    } 
    
    int mediano = med(arr, i, sLimit);
    swap(arr, mediano, p + (i-p)/5 );
  }

  int dim = q-p+1;
  return med(arr, 0, ceil(dim/5.0)-1 );

}

int MoM(int arr[], int p, int q){

  if( q-p < 5){
      return med(arr, p, q);
  }

  int j=0;
  int sLimit;
  int dim = ceil( (q-p+1)/5.0 );
  int B[dim];
  
  for(int i = p; i <= q; i = i+5){ 
    sLimit = i+4;
    if(sLimit > q){  
      sLimit = q;
    }

    B[j] = MoM(arr, i, sLimit);
    j++;
  }

  return B[med(B, 0, dim-1)];

}

/** 
 * @brief: divide l'array in 3 sezioni: 
 *         quella con elementi minori di k,
 *         quella con elementi uguali a k
 *         quella con elementi maggiori di k 
 *  
 * @param arr: array  
 * @param k: posizione data in input
 * @param p: prima posizione della parte di vettore considerata
 * @param q: ultima posizione della parte di vettore considerata  
 * @param posPerno: posizione del perno calcolato con MoM 
 * @return int: posizione del perno (calcolato con MoM) rispetto a k
 *              
 */ 
int MoMPartition(int arr[], int k, int p, int q, int posPerno){ 
 
  int perno = arr[posPerno];
  swap(arr, posPerno, q);      //sposto il perno in ultima posizione 
  int indice = p;
 
  //sezione di elementi minori del perno
  for(int i = p; i <= q-1; i++){ 
    if(arr[i] < perno){ 
      swap(arr, indice, i); 
      indice++; 
    } 
  } 

  int indiciEq = indice; //indice dell'ultimo elemento uguale al perno 
 
  //sezione di elementi uguali al perno
  for(int i = indice; i <= q-1; i++){ 
    if(arr[i] == perno){ 
      swap(arr, indiciEq, i); 
      indiciEq++; 
    } 
  } 

  //sistemo il perno in  modo da avere stabilità 
  swap(arr, q, indiciEq); 
 
  if(k < indice){ 
    return indice; 
  } 
  if(k <= indiciEq){ 
    return k; 
  } 
  return indiciEq; 
} 

/** 
 * @brief: ordina l'array da p a q 
 *  
 * @param arr: array
 * @param p: prima posizione della parte di vettore considerata 
 * @param q: ultima posizione della parte di vettore considerata 
 */ 
void insertionSort(int arr[], int p, int q ){
   
  int i = p+1; 
  while(i <= q){ 
    int j = i; 
    while (j > p && arr[j-1] > arr[j] ){ 
      swap(arr, j-1, j); 
      j = j-1; 
    }
    i = i+1;     
  } 
} 

/** 
 * @brief: trova il mediano di un vettore
 *  
 * @param arr: array
 * @param p: prima posizione della parte di vettore considerata 
 * @param q: ultima posizione della parte di vettore considerata
 * @return int: posizione del mediano del vettore rispetto a p 
 */ 
int med(int arr[], int p, int q){ 
 
  insertionSort(arr, p, q); 
  return ceil( (p+q)/2.0 ); 
}