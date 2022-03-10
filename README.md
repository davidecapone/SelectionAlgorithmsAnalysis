# Progetto di ASD Laboratorio 🐒

> Il progetto richiede l'implementazione e l'analisi dei tempi medi di esecuzione di tre algoritmi di selezione > (calcolo del 𝑘-esimo elemento più piccolo in un vettore non ordinato di interi). I tre algoritmi di selezione, denominati rispettivamente "quick select", "heap select" e "median-of-medians select", dovranno avere le seguenti caratteristiche:

## Quick select 🐸

Si tratta di una variante dell'algoritmo di ordinamento "quick sort", in cui ogni chiamata ricorsiva su un intervallo [𝑖,𝑗] del vettore fornito in input termina in tempo costante ogniqualvolta il parametro 𝑘 non sia contenuto nell'intervallo [𝑖,𝑗]. L'algoritmo deve avere complessità temporale asintotica Θ(𝑛2) nel caso pessimo e 𝑂(𝑛) nel caso medio, dove 𝑛 è il numero di elementi del vettore.

## Heap select 🦄

Questo algoritmo di selezione utilizza due min-heap, denominate H1 e H2. La prima heap H1 é costruita a partire dal vettore fornito in input in tempo lineare e non viene modificata. La seconda heap H2 contiene inizialmente un solo nodo, corrispondente alla radice di H1. All'𝑖-esima iterazione, per 𝑖 che va da 1 a 𝑘−1, l'algoritmo estrae la radice di H2, che corrisponde a un nodo 𝑥𝑖 in H1, e reinserisce in H2 i nodi successori (figli sinistro e destro) di 𝑥𝑖 nella heap H1. Dopo 𝑘−1 iterazioni, la radice di H2 corrisponderà al 𝑘-esimo elemento più piccolo del vettore fornito in input.
L'algoritmo descritto ha complessità temporale 𝑂(𝑛+𝑘𝑙𝑜𝑔𝑘) sia nel caso pessimo che in quello medio. Per 𝑘 sufficientemente piccolo, quindi, l'algoritmo "heap select" sarà preferibile, almeno nel caso pessimo, all'algoritmo "quick select". È possibile implementare una variante che utilizzi opportunamente min-heap o max-heap, a seconda del valore di 𝑘.

## Median-of-medians select 🐶

L'algoritmo è basato sulla suddivisione del vettore fornito in input in blocchi di dimensione limitata e sul calcolo della mediana delle mediane. Più precisamente, l'algoritmo esegue le seguenti operazioni:
- divisione dell'array in blocchi di 5 elementi, escluso eventualmente l'ultimo blocco che potrà contenere meno di 5 elementi,
- ordinamento e calcolo della mediana di ciascun blocco,
- calcolo della mediana 𝑀 delle mediate dei blocchi, attraverso chiamata ricorsiva allo stesso algoritmo
- partizionamento dell'intero array attorno alla mediana 𝑀, attraverso una variante della procedura "partition" dell'algoritmo "quick sort"
- chiamata ricorsiva nella parte di array che sta a sinistra o a destra della mediana 𝑀, in funzione del valore 𝑘 fornito in input.
Il modo più semplice per implementare quest'algoritmo consiste nell'allocare, ad ogni chiamata ricorsiva, un nuovo vettore per memorizzare le mediane dei blocchi. Esiste tuttavia un approccio più efficiente e "in place" che riutilizza lo spazio allocato per il vettore originariamente fornito in input. La valutazione del progetto terrà conto della variante implementata (quella "in place", essendo più complicata ma anche più efficiente, sarà valutata con un punteggio più alto).
Indipendentemente dalla variante implementata, nel caso pessimo l'algoritmo dovrà avere complessità, sia temporale che spaziale, pari a Θ(𝑛).



## Modalità di consegna 📤

Si richiede:
1. L'implementazione in un linguaggio a scelta (ad esempio, C, C++, Java) dei tre algoritmi descritti sopra, in modo che siano formalmente corretti (è possibile assumere che gli input siano ben formati, ovvero che i vettori non siano vuoti e che il parametro 𝑘 sia sempre positivo e minore o uguale alla dimensione 𝑛 del vettore). Per agevolare la verifica di correttezza da parte del docente sono stati predisposti tre moduli "Virtual Programming Laboratory" (VPL) da utilizzare per caricare il codice degli algoritmi. Una condizione necessaria alla valutazione dell'elaborato è il superamento di tutti i test previsti, per tutti e tre gli algoritmi. Nota: l'esecuzione di un programma lato server attraverso un modulo VPL garantisce uno spazio di memoria di almeno 64KB, giudicato ampiamente sufficiente per risolvere il problema assegnato con qualunque algoritmo fra quelli sopra descritti.

2. La stima dei tempi medi di esecuzione per tre algoritmi, al variare della dimensione 𝑛 del vettore ed eventualmente del parametro 𝑘 (nei casi, ovviamente, in cui si ritenga esista una correlazione fra tempo di esecuzione e parametro 𝑘). Il vettore in input deve essere generato in modo pseudo-casuale con interi eventualmente anche negativi, e il tempo di inizializzazione dell'input dev'essere opportunamente scomputato dalla stima del tempo di esecuzione. I codici sorgenti degli algoritmi valutati in questa parte dovranno essere gli stessi di quelli presentati al punto 1), fatta esclusione, ovviamente, per le parti di codice che costruiscono l'input e gestiscono la misurazione dei tempi medi di esecuzione al variare di 𝑛. I tempi di esecuzione devono essere misurati con un errore relativo massimo pari a 0.01 (1%). A tal fine si consiglia di procedere nel modo seguente:

Per tutte le misurazioni di intervalli di tempo è necessario utilizzare un clock di sistema monotono (utilizzare, ad esempio, la procedura clock_gettime(CLOCK_MONOTONIC, &timeNow) della libraria <time.h> del linguaggio C, oppure il metodo steady_clock::now() della libreria <chrono> del C++, oppure System.nanoTime() del linguaggio Java).

Il primo passo consiste nello stimare la risoluzione del clock di sistema, utilizzando un ciclo while per calcolare l'intervallo minimo di tempo misurabile. A tale scopo è possibile utilizzare uno dei seguenti frammenti di codice in linguaggio C, C++, Java:

  // linguaggio C
  #include <time.h>
  ...
  double duration(struct timespec start, struct timespec end) {
      return end.tv_sec - start.tv_sec
          + ((end.tv_nsec - start.tv_nsec ) / (double) 1000000000.0);
  }
  double getResolution(){
      struct timespec start, end;
      clock_gettime(CLOCK_MONOTONIC, &start);
      do {
          clock_gettime(CLOCK_MONOTONIC, &end);
      } while (duration(start, end) == 0.0);
      return duration(start, end);
  }

  // linguaggio C++
  #include <stdlib.h>
  #include <chrono>
  ...
  using namespace std;
  using namespace std::chrono;
  ...
  double getResolution() {
      steady_clock::time_point start = steady_clock::now();
      steady_clock::time_point end;
      do {
          end = steady_clock::now();
      } while (start == end);
      typedef duration<double, seconds::period> secs;
      return duration_cast<secs>(end - start).count();
  }

  // linguaggio Java
  import java.util.*;
  ...
  class ... {
      ...
      private static double getResolution() {
        double start = System.nanoTime();
        double end;
        do {
            end = System.nanoTime();
        } while (start == end);
        return end - start;
  }
Successivamente, in funzione della risoluzione stimata 𝑅 e dell'errore relativo massimo ammissibile (𝐸=0.001), si calcola il tempo minimo misurabile

𝑇min=𝑅⋅(1𝐸+1).

Per stimare il tempo medio di esecuzione di un algoritmo su una singola istanza dell'input di dimensione 𝑛, si utilizza un ciclo while, iterando l'esecuzione dell'algoritmo sullo stesso input (che andrà eventualmente rigenerato) e misurando un intervallo di tempo superiore a 𝑇min. La misurazione deve essere effettuata senza interrompere il clock, ovvero calcolando l'intero intervallo di tempo trascorso dall'inizio dell'iterazione fino al momento il cui il tempo misurato risulti superiore a  𝑇min. Il tempo medio di esecuzione per la singola istanza di input sarà quindi ottenuto calcolando il rapporto fra il tempo totale misurato e il numero di iterazioni dell'algoritmo eseguite.

Nel caso si utilizzi il linguaggio di programmazione Java, occorre prestare attenzione a non allocare ripetutamente grandi strutture dati (esempio, array o stringhe) in modo dinamico (ad esempio, con l'istruzione new). Tale pratica potrebbe esaurire in breve tempo la memoria RAM disponibile e attivare il garbage collector, creando picchi nei tempi di esecuzione misurati. Una considerazione simile si applica ai linguaggi C e C++, che tuttavia permettono di gestire in modo esplicito l'allocazione e liberazione della memoria.

Opzionalmente, è possibile stimare anche la deviazione standard dei tempi di esecuzione rispetto al tempo medio, in funzione del parametro 𝑛. In tal caso si procederà effettuando un certo numero di misurazioni (ad esempio, un centinaio) sui tempi di esecuzione per una lunghezza 𝑛 fissata, facendo però variare l'input di dimensione 𝑛 generato in modo pseudo-casuale. Ogni misurazione sarà effettuata seguendo il metodo sopra descritto, e il risultato di tale misurazione dovrà essere memorizzato in un opportuno array. Una volta popolato l'array con le misurazioni relative ad una particolare scelta di 𝑛, si procederà calcolando media e scarto quadratico medio.

I dati raccolti devono essere presentati e discussi in una relazione in formato PDF da caricare sul server. La valutazione della relazione e del codice sorgente contribuirà in modo significativo al voto finale del progetto di laboratorio. Non è necessario inviare una relazione con molte pagine: qualche decina di pagine è largamente sufficiente a discutere gli aspetti importanti dell'implementazione e dell'analisi dei tempi di esecuzione. Si consiglia l'uso di grafici comparativi, sia in scale lineari - 𝑛 vs 𝑡(𝑛) - che doppiamente logaritmiche - log(𝑛) vs log(𝑡(𝑛)).


NOTA BENE: Durante l'elaborazione del progetto, sarà possibile caricare diverse versioni dei programmi sui vari moduli VPL (sostituendo ovviamente i vecchi file). Il progetto viene considerato come consegnato una volta che la relazione in formato PDF viene caricata online. A partire da quel momento il docente potrà prendere visione della relazione e dei codici sorgente forniti.

Sono ammessi gruppi di massimo 4 persone per lo svolgimento del progetto. Il sito e-learning dell'università non permette la creazione autonoma di gruppi; si consiglia quindi di procedere scegliendo un "rappresentante" per ogni gruppo, che avrà la responsabilità di caricare tutti i file sul sito, inclusa la relazione finale. È importante che la relazione riporti in chiaro i nomi, i cognomi, le email, e i numeri di matricola di ciascun componente del gruppo.

Non è fissata una data precisa per la consegna degli elaborati, ma si chiede di consegnare il progetto entro la fine dell'anno accademico. I progetti consegnati verranno valutati dal docente a intervalli regolari (approssimativamente ogni 2-3 settimane). Qualora fosse necessaria una valutazione in tempi brevi, si prega di contattare il docente per e-mail (gabriele.puppis@uniud.it).