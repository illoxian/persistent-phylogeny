bit vector

idea:

- tengo il bit vector originale di "differenza" implementando l'ugaglianza come *==0\==1 oppure 

- implemento t bit vector in modo da sapere con quale bit potrei correggere la wildcard per ampliare il blocco (correggerla solo per quell'iterazione), dato che comunque in un k successivo potrei aver bisogno di quella wildcard con valore differente (?) --> definire matematicamente questo problema: sono davvero necessari t bit vector, e in quest'ultimo caso, sarebbe utile per una correzione della wildcard (algoritmo greedy di correzione migliore della wildcard basandosi su match precedenti)

  

- effettuo una correzione: dato un valore standard, lo metto nel bitvector.
- data una scelta di match (0/1/2/... con * ) scelgo di cambiare il valore successivo nell'array importato con la correzione (in modo da non dover ordinare piu occorrenze)... metodo greedy ... e' la soluzione ottima? potrei avere un blocco che sfrutta quella wildcard? essendoci un match prima, dovrei avere la wildcard presente in piu blocchi. a questo punto arriverei ad avere in k+1 di dover correggere la stessa sequenza piu volte (per ogni wildcard espansa riferita ad una singola posizione). La correzione in questo modo **non e' attuabile**.
  - Posso correggere a monte sapendo che non ho wildcard tra due posizioni, a quel punto dovrei avere gap distanti almeno 1 posizione. posso a quel punto pensare di corregere la wildcard una volta collassata, oppure scegliendo il blocco maggiore in fieri
  -  alla fine dell'algoritmo ho comunque in out put dei range. a questo punto decido di leggere la prima sequenza del blocco, e sostituire nelle posizione con gap altre sequenze che per quella data posizione non presentano gap, in modo da avere un "Sample" corretto per un x blocco. Potrei avere blocchi formati da posizioni solo con gap? si. a quel punto in output avro' solamente le posizioni con gap oppure permettere un flag che salti tali blocchi.

---

prima implementazione di bitvector

scelgo la versione mono bit? potrebbe essere la soluzione migliore, rapportata alla soluzione di bitvector gia presente:

il bit vector su pbwt classifca funziona in modo da prendere l'ordinamento in colonna k 