/** 
 * @file medianMedians.c 
 * @author Capone, Della Rovere, Gortani, Fior 
 */ 
#include <math.h> 
#include <limits.h>
#include "scanArray.h"
#include "swap.h"

int MoM(int arr[], int p, int q); 
void insertionSort(int arr[], int p, int q ); 
int med5(int arr[], int p, int q); 
int MoMpartition(int arr[], int k, int p, int q, int posPerno); 
int MoMPlace(int arr[], int p, int q);
 
/** 
 * @brief: trova arr[k] se arr fosse ordinato  
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
    return p; 
  } 
 
  int posPerno = MoMPlace(arr, p, q);
  posPerno = MoMpartition(arr, k, p, q, posPerno); 
 
  /*posPerno a questo punto può essere:
  * uguale a k: ho finito
  * minore/maggiore di k: ricorsione nella parte di array contentente k
  *                       escludendo la posizione del perno
  */ 
 
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
int MoMpartition(int arr[], int k, int p, int q, int posPerno){ 
 
  int perno = arr[posPerno];
  swap(arr, posPerno, q);      //sposto il perno in ultima posizione 
  int indice = p;
 
  //sezione di elementi minori del perno
  for(int i = p; i < q-1; i++){ 
    if(arr[i] < perno){ 
      swap(arr, indice, i); 
      indice++; 
    } 
  } 
 
  int indiciEq = indice; //indice dell'ultimo elemento uguale al perno 
 
  //sezione di elementi uguali al perno
  for(int i = indice; i < q-1; i++){ 
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
 * @brief: trova la posizione del mediano dei mediani (versione NON in place)
 *  
 * @param arr: array  
 * @param p: prima posizione della parte di vettore considerata
 * @param q: ultima posizione della parte di vettore considerata
 * @return int: posizione del mediano dei mediani
 */ 
int MoM(int arr[], int p, int q){ 
 
  int quinti = ceil((q-p+1)/5); //numero di blocchetti di dimensione <= 5 che compongono arr 
  int B[quinti]; 
  int j=0; 
 
  //suddividiamo l' array in blocchi da 5 per poi salvare sul vettore B la mediana di ogni blocco 
  for(int i = 0; i <= q; i = i+5){ 
     
    if( i+4 > q){ 
      B[j] = med5(arr, i, q); 
    } else { 
      B[j] = med5(arr, i, i+4); 
    } 
    j++; 
  } 
  return med5(B, 0, quinti); 
} 

/** 
 * @brief: trova la posizione del mediano dei mediani (versione in place)
 *  
 * @param arr: array  
 * @param p: prima posizione della parte di vettore considerata
 * @param q: ultima posizione della parte di vettore considerata
 * @return int: posizione del mediano dei mediani
 */ 
int MoMPlace(int arr[], int p, int q){ 
 
  //se il blocco è da meno di 5 elementi
  if( q-p < 5){
    return med5(arr, p, q);   
  }
  
  int quinti = ceil((q-p+1)/5); //numero di blocchetti di dimensione <= 5 che compongono arr 

  //mettiamo i mediani delle sezioni nelle prime n/5 posizioni dello stesso array
  
  for(int i = p; i <= q; i+=5){ 
    
    int sLimit = i+4;  //section limit: indice che definisce la sezione da 5 elementi
    
    //controllo di non andare oltre l'ultima posizione del vettore
    if(sLimit > q){  
      sLimit = q;
    } 
    
    int mediano = med5(arr, i, sLimit);
    swap(arr, mediano, p+floor((i-p)/5) );

  }
  
  return med5(arr, 0, quinti); 
}  

 
/** 
 * @brief: trovare il mediano di un vettore di dimensione 5 
 *  
 * @param arr: array
 * @param p: prima posizione della parte di vettore considerata 
 * @param q: ultima posizione della parte di vettore considerata
 * @return int: posizione del mediano del vettore  
 */ 
int med5(int arr[], int p, int q){ 
 
  insertionSort(arr, p, q); 
  return ceil( (p+q)/2.0 ); 
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
 

int main () { 
  /** TODO  
  - Correttezza MoM NON in place
  - pulizia del codice 
   */ 
 
  // scan sullo standard input per definire il vettore
  int arr[ MAX_LINE_SIZE ];
  int *p = &arr[0];
  int k;

  printf("Inserire gli elementi dell'array: ");
  int size = scanArray( p );


  printf("Inserire la posizione cercata: ");
  scanf("%d", &k);

  int posizione = MoMSelect(arr, k, 0, size-1);
  
  if(posizione < 0){
    printf("Valore: %d. Posizione k cercata non valida.\n", posizione);
  }else{
   printf("L'elemento in posizione %d dell'array è: %d\n", k, arr[posizione] ); 
  }
  
  return 0; 
}
// 1 2 3 4 5 6 7 8 9