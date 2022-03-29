/** 
 * @file medianMedians.c 
 * @author Capone, Della Rovere, Gortani, Fior 
 */ 
#include <math.h> 
#include <limits.h>
#include "scanArray.h"
#include "swap.h"
 
/** 
 * @brief: ordinare una porzione di array compresa tra p ... q
 *  
 * @param A: array da ordinare
 * @param p: posizione di partenza
 * @param q: posizione finale
 */ 
void insertionSort(int A[], int p, int q ){
   
  int i = p+1; 
  while( i <= q ){ 
    int j = i; 
    
    while (j > p && A[j-1] > A[j] ){ 
      swap(A, j-1, j); 
      j = j-1; 
    }
    
    i = i+1;     
  } 
} 

/** 
 * @brief: posizione del mediano in una porzione di 5 elementi
 *  - ordino utilizzando insertionSort
 *  - restituisco la posizione centrale
 *
 * @param A: array
 * @param p: posizione partenza
 * @param q: posizione finale
 * @return int: posizione del mediano del vettore  
 */ 
int medianOfFiveElement(int A[], int p, int q){ 
 
  insertionSort(A, p, q); 
  return (p+q)/2; 
} 

/** 
 * @brief: divide l'array in 3 sezioni: 
 *         quella con elementi minori di k,
 *         quella con elementi uguali a k
 *         quella con elementi maggiori di k 
 *  
 * @param A: array  
 * @param k: posizione data in input
 * @param p: prima posizione della parte di vettore considerata
 * @param q: ultima posizione della parte di vettore considerata  
 * @param posPerno: posizione del perno calcolato con MoM 
 * @return int: posizione del perno (calcolato con MoM) rispetto a k
 *              
 */ 
int momPartition(int A[], int k, int p, int q, int posPerno){ 
 
  int perno = A[posPerno];
  swap(A, posPerno, q);      //sposto il perno in ultima posizione 
  int indice = p;
 
  //sezione di elementi minori del perno
  for(int i = p; i < q-1; i++){ 
    if(A[i] < perno){ 
      swap(A, indice, i); 
      indice++; 
    } 
  } 
 
  int indiciEq = indice; //indice dell'ultimo elemento uguale al perno 
 
  //sezione di elementi uguali al perno
  for(int i = indice; i < q-1; i++){ 
    if(A[i] == perno){ 
      swap(A, indiciEq, i); 
      indiciEq++; 
    } 
  } 
   
  //sistemo il perno in  modo da avere stabilità 
  swap(A, q, indiciEq); 
 
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
 * @param A: array  
 * @param p: prima posizione della parte di vettore considerata
 * @param q: ultima posizione della parte di vettore considerata
 * @return int: posizione del mediano dei mediani
 */ 
int mom_noninplace(int A[], int p, int q){ 
 
  int quinti = ceil((q-p+1)/5); //numero di blocchetti di dimensione <= 5 che compongono A 
  int B[quinti]; 
  int j=0; 
 
  //suddividiamo l' array in blocchi da 5 per poi salvare sul vettore B la mediana di ogni blocco 
  for(int i = 0; i <= q; i = i+5){ 
     
    if( i+4 > q){ 
      B[j] = medianOfFiveElement(A, i, q); 
    } else { 
      B[j] = medianOfFiveElement(A, i, i+4); 
    } 
    j++; 
  } 

  int median = medianOfFiveElement(B, 0, quinti);
  return median;
} 

/** 
 * @brief: trova la posizione del mediano dei mediani (versione in place)
 *  
 * @param A: array  
 * @param p: prima posizione della parte di vettore considerata
 * @param q: ultima posizione della parte di vettore considerata
 * @return int: posizione del mediano dei mediani
 */ 
int mom(int A[], int p, int q){ 
 
  //se il blocco è da meno di 5 elementi
  if( q-p < 5) return medianOfFiveElement(A, p, q);   

  int quinti = ceil((q-p+1)/5); //numero di blocchetti di dimensione <= 5 che compongono A 

  //mettiamo i mediani delle sezioni nelle prime n/5 posizioni dello stesso array
  for(int i = p; i <= q; i+=5){ 
    
    int sLimit = i+4;  //section limit: indice che definisce la sezione da 5 elementi
    
    //controllo di non andare oltre l'ultima posizione del vettore
    if(sLimit > q){  
      sLimit = q;
    } 
    
    int mediano = medianOfFiveElement(A, i, sLimit);
    swap(A, mediano, p+floor((i-p)/5) );
  }
  
  return medianOfFiveElement(A, 0, quinti); 
}  

/** 
 * @brief: trova A[k] se A fosse ordinato  
 *  
 * @param A: array
 * @param k: posizione data in input
 * @param p: prima posizione della parte di vettore considerata
 * @param q: ultima posizione della parte di vettore considerata
 * @return int: posizione di k se A fosse ordinato
 */ 
int momSelect_noninplace(int A[], int k, int p, int q) { 
 
  // controllo se k appartiene all'intervallo [p, ..., q]
  if( k < p || k > q) return INT_MIN;

  // caso base
  if(p == q) return A[p]; 
 
  // partizionamento intorno al mediano
  int posPerno = mom(A, p, q);
  posPerno = momPartition(A, k, p, q, posPerno); 

  if(k == posPerno) 
    return A[k]; 
  
  else if (k < posPerno) { 
    q = posPerno - 1; 
    return momSelect_noninplace(A, k, p, q); 
  } else { 
    p = posPerno + 1; 
    return momSelect_noninplace(A, k, p, q); 
  } 
} 


int main () { 
 
  int A[ MAX_LINE_SIZE ];
  int *p = &A[0];
  int k;

  int size = scanArray( p );

  scanf("%d", &k);

  int kSmallest = momSelect_noninplace(A, k, 0, size-1);
  
  printf("elemento: %d\n", kSmallest);
  
  return 0; 
}
