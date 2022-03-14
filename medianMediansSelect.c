/**
 * @file medianMedians.c
 * @author Capone, Della Rovere, Gortani, Fior
 */
#include <stdio.h>
#include <math.h>

int MoM(int arr[], int p, int q);
void inSort(int arr[], int p, int q );
int med5(int arr[], int p, int q);
int partition(int arr[], int k, int p, int q, int posPerno);

/**
 * @brief scambia due elementi dell'array
 * 
 * @param A array
 * @param i primo indice
 * @param j secondo indice
 */
void swap( int A[], int i, int j ){
  int key = A[i];
  A[i] = A[j];
  A[j] = key;
}

/**
 * @brief 
 * 
 * @param arr 
 * @param k 
 * @param p 
 * @param q 
 * @return int 
 */
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
//alla fine restituisce        --- QUESTO VA NEL CONTRATTO !!!
/**
 * @brief 
 * 
 * @param arr 
 * @param k 
 * @param p 
 * @param q 
 * @param posPerno 
 * @return int 
 */
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

/**
 * @brief 
 * 
 * @param arr 
 * @param p 
 * @param q 
 * @return int 
 */
int MoM(int arr[], int p, int q){

  int quinti = ceil((q-p+1)/5); //quinti è il numero di blocchetti di dimensione <= 5 che compongono arr
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

/**
 * @brief 
 * 
 * @param arr 
 * @param p 
 * @param q 
 * @return int 
 */
int med5(int arr[], int p, int q){ //dato blocco, ritorno il mediano

  inSort(arr, p, q);
  return ceil( (p+q)/2 );
}


/**
 * @brief 
 * 
 * @param arr 
 * @param p 
 * @param q 
 */
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
  /** TODO
  - fare tutti i contratti delle funzioni
  - non serve fare i prototipi all'inizio del codice per le funzioni se le funzioni le mettete prima del main
  - importare la funzione scanArray del prof anche qua (la trovate anche nei nostri codici)
  - pulizia del codice
  - inSort cos'è? Una parola losca napoletana? chiamatela insertionSort
  - versione in place di medianMediansSelect
   */
  return 0;
}