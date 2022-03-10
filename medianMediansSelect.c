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

void swap(int A[], int i, int j){
  
  int key = A[i];
  
  A[i] = A[j];
  A[j] = key;
}

int medianOfMedians(int arr[], int k, int p, int q){

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