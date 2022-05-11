#ifndef MEDIANSELECT
#define MEDIANSELECT
#include <math.h> 
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>

#define MAX_LINE_SIZE 5000   // maximum size of a line of input

//Dichiarazione funzioni utilizzate
int MoMSelect(int arr[], int p, int q, int k);
int MoM(int arr[], int p, int q);
int MoMPlace(int arr[], int p, int q);
int MoMPartition(int arr[], int p, int q, int k, int posPerno);
int med(int arr[], int p, int q);
void insertionSort(int arr[], int p, int q );

/** 
 * @brief: trova l'indice di k nell'array se l'array fosse ordinato  
 *  
 * @param arr: array/vettore
 * @param k: posizione data in input
 * @param p: prima posizione della parte di vettore considerata
 * @param q: ultima posizione della parte di vettore considerata
 * @return int: posizione di k nell'array se questo fosse ordinato
 */ 
int MoMSelect(int arr[], int p, int q, int k){

  //controllo k sensato
  if( k < p || k > q) return INT_MIN;

  //caso base della ricorsione: 1 elemento 
  if(p == q) return p; 
  
  //calcolo della posizione del perno 
  int posPerno;
  posPerno = MoMPlace(arr, p, q);

  //partizione della porzione di array da p a q, in base a k e posPerno
  posPerno = MoMPartition(arr, p, q, k, posPerno); 
 
  /*
  posPerno a questo punto può essere:
    -uguale a k: ho finito, è la posizione di k cercata
    -minore/maggiore di k: ricorsione nella parte di array contentente k
  *                        escludendo la posizione del perno
  */ 
 
  if(k == posPerno) 
    return k; 
  else if(k < posPerno) { 
    q = posPerno - 1; 
    return MoMSelect(arr, p, q, k); 
  } else { 
    p = posPerno + 1; 
    return MoMSelect(arr, p, q, k); 
  } 
} 

/** 
 * @brief: trova la posizione del mediano dei mediani (versione "in place")
 *  
 * @param arr: array  
 * @param p: prima posizione della parte di vettore considerata
 * @param q: ultima posizione della parte di vettore considerata
 * @return int: posizione (indice) del mediano dei mediani
 */ 
int MoMPlace(int arr[], int p, int q){

  //caso base della ricorsione: trovo il mediano in una porzione di array di dimensione 5
  if( q-p < 5) return med(arr, p, q);
  
  //mettiamo gli n/5 mediani dei blocchi da 5 nelle prime n/5 posizioni dello stesso array

  int sLimit;                       //section limit: indice che definisce il blocco da 5 elementi
  for(int i = p; i <= q; i = i+5){ 
    sLimit = i+4;  

    //controllo di non andare oltre l'ultima posizione del vettore
    if(sLimit > q) sLimit = q;
    int mediano = med(arr, i, sLimit);
    swap(arr, mediano, p+floor( (i-p)/5) );
  }

  int tempQ = p + floor( (q-p)/5 );
  int mid = (tempQ-p+1)/2;

  //chiamata ricorsiva per trovare il mediano dei mediani, ossia il mediano dei primi n/5 elementi
  return MoMSelect(arr, p, tempQ, p+mid-1 );
}

/** 
 * @brief: trova la posizione del mediano dei mediani (versione con array di supporto)
 *  
 * @param arr: array  
 * @param p: prima posizione della parte di vettore considerata
 * @param q: ultima posizione della parte di vettore considerata
 * @return int: posizione (indice) del mediano dei mediani
 */
int MoM(int arr[], int p, int q){

  //caso base della ricorsione: trovo il mediano in una porzione di array di dimensione 5
  if( q-p < 5) return med(arr, p, q);
  
  int j=0;
  int sLimit;
  int dim = ceil( (q-p+1)/5.0 );
  int B[dim];
  
  //calcolo dei mediani dei blocchi da 5 e salvataggio di questi in un array di supporto B
  for(int i = p; i <= q; i = i+5) { 
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
 *         quella con elementi minori dell'elemento che finirebbe in posizione k,
 *         quella con elementi uguali dell'elemento che finirebbe in posizione k
 *         quella con elementi maggiori dell'elemento che finirebbe in posizione k 
 *  
 * @param arr: array  
 * @param k: posizione data in input
 * @param p: prima posizione della parte di vettore considerata
 * @param q: ultima posizione della parte di vettore considerata  
 * @param posPerno: posizione del perno calcolato con MoMPlace
 * @return int: posizione del perno (calcolato con MoMPlace) rispetto a k
 *              
 */ 
int MoMPartition(int arr[], int p, int q, int k, int posPerno){ 
  
  int perno = arr[posPerno];
  swap(arr, posPerno, q);      //sposto il perno in ultima posizione 
  int indice = p;
 
  //sezione di elementi minori del perno
  for(int i = p; i <= q-1; i++) { 
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
 
  if(k < indice) { 
    return indice; 
  } 
  if(k <= indiciEq){ 
    return k; 
  } 
  return indiciEq; 
} 

/** 
 * @brief: ordina l'array tra la posizione p e la posizione q comprese
 *  
 * @param arr: array
 * @param p: prima posizione della parte di vettore considerata 
 * @param q: ultima posizione della parte di vettore considerata 
 */ 
void insertionSort(int A[], int p, int q ){
   
    int key;
    int i;
    for (int j = p+1; j < q; j++) {
        key = A[j];
        i = j-1;
        
        while ((i >= p) && (A[i] > key)) {
            A[i+1] = A[i];
            i = i-1;
        }
    
        A[i+1] = key;
    }
}

/** 
 * @brief: trova il mediano di una porzione di array
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
#endif 

