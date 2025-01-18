# Selection Algortihms Analysis

> Il progetto richiede l'implementazione e l'analisi dei tempi medi di esecuzione di tre algoritmi di selezione > (calcolo del 𝑘-esimo elemento più piccolo in un vettore non ordinato di interi). I tre algoritmi di selezione, denominati rispettivamente "quick select", "heap select" e "median-of-medians select".

## Quick select

Si tratta di una variante dell'algoritmo di ordinamento "quick sort", in cui ogni chiamata ricorsiva su un intervallo [𝑖,𝑗] del vettore fornito in input termina in tempo costante ogniqualvolta il parametro 𝑘 non sia contenuto nell'intervallo [𝑖,𝑗]. L'algoritmo deve avere complessità temporale asintotica Θ(𝑛2) nel caso pessimo e 𝑂(𝑛) nel caso medio, dove 𝑛 è il numero di elementi del vettore.

## Heap select

Questo algoritmo di selezione utilizza due min-heap, denominate H1 e H2. La prima heap H1 é costruita a partire dal vettore fornito in input in tempo lineare e non viene modificata. La seconda heap H2 contiene inizialmente un solo nodo, corrispondente alla radice di H1. All'𝑖-esima iterazione, per 𝑖 che va da 1 a 𝑘−1, l'algoritmo estrae la radice di H2, che corrisponde a un nodo 𝑥𝑖 in H1, e reinserisce in H2 i nodi successori (figli sinistro e destro) di 𝑥𝑖 nella heap H1. Dopo 𝑘−1 iterazioni, la radice di H2 corrisponderà al 𝑘-esimo elemento più piccolo del vettore fornito in input.
L'algoritmo descritto ha complessità temporale 𝑂(𝑛+𝑘𝑙𝑜𝑔𝑘) sia nel caso pessimo che in quello medio. Per 𝑘 sufficientemente piccolo, quindi, l'algoritmo "heap select" sarà preferibile, almeno nel caso pessimo, all'algoritmo "quick select". È possibile implementare una variante che utilizzi opportunamente min-heap o max-heap, a seconda del valore di 𝑘.

## Median-of-medians select 

L'algoritmo è basato sulla suddivisione del vettore fornito in input in blocchi di dimensione limitata e sul calcolo della mediana delle mediane. Più precisamente, l'algoritmo esegue le seguenti operazioni:
- divisione dell'array in blocchi di 5 elementi, escluso eventualmente l'ultimo blocco che potrà contenere meno di 5 elementi,
- ordinamento e calcolo della mediana di ciascun blocco,
- calcolo della mediana 𝑀 delle mediate dei blocchi, attraverso chiamata ricorsiva allo stesso algoritmo
- partizionamento dell'intero array attorno alla mediana 𝑀, attraverso una variante della procedura "partition" dell'algoritmo "quick sort"
- chiamata ricorsiva nella parte di array che sta a sinistra o a destra della mediana 𝑀, in funzione del valore 𝑘 fornito in input.
Il modo più semplice per implementare quest'algoritmo consiste nell'allocare, ad ogni chiamata ricorsiva, un nuovo vettore per memorizzare le mediane dei blocchi. Esiste tuttavia un approccio più efficiente e "in place" che riutilizza lo spazio allocato per il vettore originariamente fornito in input. La valutazione del progetto terrà conto della variante implementata (quella "in place", essendo più complicata ma anche più efficiente, sarà valutata con un punteggio più alto).
Indipendentemente dalla variante implementata, nel caso pessimo l'algoritmo dovrà avere complessità, sia temporale che spaziale, pari a Θ(𝑛).
