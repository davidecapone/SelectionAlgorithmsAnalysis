/**
 * @file medianMedians.c
 * @author Capone, Della Rovere, Gortani, Fior
 * @brief 
  L'algoritmo è basato sulla suddivisione del vettore fornito in input in blocchi di dimensione limitata 
  e sul calcolo della mediana delle mediane. Più precisamente, l'algoritmo esegue le seguenti operazioni:
  - divisione dell'array in blocchi di 5 elementi, escluso eventualmente l'ultimo blocco che potrà contenere meno di 5 elementi,
  - ordinamento e calcolo della mediana di ciascun blocco,
  - calcolo della mediana 𝑀 delle mediane dei blocchi, attraverso chiamata ricorsiva allo stesso algoritmo
  - partizionamento dell'intero array attorno alla mediana 𝑀, attraverso una variante della procedura "partition" dell'algoritmo "quick sort"
  - chiamata ricorsiva nella parte di array che sta a sinistra o a destra della mediana 𝑀, in funzione del valore 𝑘 fornito in input.
  Il modo più semplice per implementare quest'algoritmo consiste nell'allocare, ad ogni chiamata ricorsiva, un nuovo vettore per memorizzare le mediane dei blocchi. Esiste tuttavia un approccio più efficiente e "in place" che riutilizza lo spazio allocato per il vettore originariamente fornito in input. La valutazione del progetto terrà conto della variante implementata (quella "in place", essendo più complicata ma anche più efficiente, sarà valutata con un punteggio più alto).
  Indipendentemente dalla variante implementata, nel caso pessimo l'algoritmo dovrà avere complessità, sia temporale che spaziale, pari a O(𝑛).
  codice: https://en.wikipedia.org/wiki/Median_of_medians
 * 
 */
#include <stdio.h>
#include <math.h>

void swap(int A[], int i, int j){
  
  int key = A[i];
  
  A[i] = A[j];
  A[j] = key;
}

int MoMSelect(int arr[], int k, int p, int q){

  //caso base: 1 elemento
  if(p == q){
    return p;
  }

  //calcola con median of medians la posizione del perno
  int posPerno = MoM(arr, p, q);
  posPerno = partition(arr, k, p, q, posPerno);


  if(k == posPerno){
    return k;
  } else if(k < posPerno){
    q = posPerno - 1;
    return MoMSelect(arr, k, p, q);
  } else {
    p = posPerno + 1;
    return MoMSelect(arr, k, p, q);
  }
}


//divide l'array in 3 sezioni: quello minore di k, quello uguale a k e quello maggiore di k
//alla fine restituisce 
int partition(int arr[], int k, int p, int q, int posPerno){

  int perno = arr[posPerno];   //pivotValue, valore del perno in posizione posPerno
  swap(arr, posPerno, q);      //sposto il perno in ultima posizione
  int indice = p;              //storeIndex, prima posizione nell'array

  //
  for(int i = p; i < q-1; i++){
    if(arr[i] < perno){
      swap(arr, indice, i);
      indice++;
    }
  }

  int indiciEq = indice; //storeIndexEq, indice dell'ultimo elemento uguale al perno

  //Gruppo "in mezzo" di elementi uguali al perno
  for(int i = indice; i < q-1; i++){
    if(arr[i] == perno){
      swap(arr, indiciEq, i);
      indiciEq++;
    }
  }
  
  //sistemo il perno in modo da avere stabilità
  swap(arr, q, indiciEq);

  //restituisco indice del perno
  if(k < indice){
    return indice;
  }

  if(k <= indiciEq){
    return k;
  }

  return indiciEq;
}

int MoM(int arr[], int p, int q){

  int quinti = ceiling((q-p+1)/5); //quinti è il numero di blocchetti di dimensione <= 5 che compongono arr
  int B[quinti];
  int j=0;

  //suddividiamo l' array in blocchi da 5 per poi salvare sul vettore B la mediana di ogni blocco
  for(int i = 0; i <= q; i+=5){
    
    if(4+i>q){
      B[j] = med5(arr, i, q);
    } else {
    //B[i] = select(arr, k, 5*i, 4+5*i);
    B[j] = med5(arr, i, 4+i);
    }
    j++;
  }
  return med5(B, 0, quinti);
}

int med5(int arr[], int p, int q){ //dato blocco, ritorno il mediano

  inSort(arr, p, q);
  return ceiling( (p+q)/2 );
}

void inSort(int arr[], int p, int q ){  //ordino blocco con insertion sort
  
  int i = p+1;

  while(i <= q){
    int j = i;
    while (j > p && arr[j-1] > arr[j] ){
      swap(arr, j-1, j);
      j = j-1;
      i = i+1;
    }    
  }
}


int main () {

  return 0;
}