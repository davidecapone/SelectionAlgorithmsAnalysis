#ifndef MEDIANSELECT2
#define MEDIANSELECT2
#include <math.h> 
#include <limits.h>
#include <stdlib.h>
#include <stdio.h>
#include "swap.h"
#define MAX_LINE_SIZE 5000   // maximum size of a line of input


int momPartition(int arr[], int p, int q, int posPerno){ 
  
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

  //sistemo il perno in  modo da avere stabilità 
  swap(arr, q, indice); 
 
  return indice; 
} 

void insertionSort(int A[], int p, int q ){
   
    int key;
    int i;
    for (int j = p+1; j <= q; j++) {
        key = A[j];
        i = j-1;
        
        while ((i >= p) && (A[i] > key)) {
            A[i+1] = A[i];
            i = i-1;
        }
    
        A[i+1] = key;
    }

}

int med(int arr[], int p, int q){ 
 
  insertionSort(arr, p, q); 
  return (p+q)/2.0; 
}


// ritorna l'indice del mediano dei mediani
int MoMPlace(int A[], int p, int q){
  
  // 5 elementi (caso base)
  if (q-p+1 <= 5) {
    return med(A, p, q);
  }

  int sLimit = 0;            //section limit: indice che definisce il blocco da 5 elementi
  int numeroMediani = 0;

  for(int i = p; i <= q; i = i+5){ 
    sLimit = i+4;  
  
    //controllo di non andare oltre l'ultima posizione del vettore
    if(sLimit > q) sLimit = q;
    int mediano = med(A, i, sLimit);
  //  printf("mediano = %d\n", mediano);
    swap(A, mediano, p+numeroMediani );
    numeroMediani++;
  }
  //int tempQ = p + ceil( (q-p+1) / 5);

  return MoMPlace(A, p, p+numeroMediani-1);

}

int MoMSelect(int A[], int p, int q, int k) {

  if (k < p || k > q) {
    return INT_MIN;
  }
 

  int perno = MoMPlace(A, p, q);

  // partiziono intorno al perno:
  perno = momPartition(A, p, q, perno);
  if (k == perno) {
    return k;
  }else if (k < perno) {
    return MoMSelect(A, p, perno-1, k);
  }else{
    return MoMSelect(A, perno+1, q, k);
  }

}


#endif