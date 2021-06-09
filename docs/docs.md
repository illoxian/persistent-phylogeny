# Docs

## g-skeleton

1. prendo i max CHARS: A_1 A_2 A_3 ... A_n
2. prendo i min chars: a_1 a_2 a_3 ... a_n
seguo gli overlap ovvero:
    - confronto ogni min char con tutti i max char, sono in overlap se a_j overlappa con almeno un MAX CHAR A_k
    - confronto ogni min  char con gli altri min char, se sono in overlap gli aggiungo al set k

-> grado in minimal form = set K + set dei char MAX
-> dal minimal form, prendo il maximal reducible graph -> ottengo il **g-skeleton**.

---

## Funzioni
### enum State

- gain
- lose

states gain of loses of a character in the PP tree

### enum Signed Character 

- character name
- character name--> init GAIN

### operator << for state

- print gain + , lose -

### operator << for SignedCharacter

- print character + sign state

### operator == for 2 signedCharacter

- se sono lo stesso carattere e hanno lo stesso stato (gain-gain lose-lose)

### REALIZE THE CHARACTER C in the Graph G (+ o -)

- ritorna la lista di caratteri realizzati and bool=True if the realization of SC is feasible of G
- ritorna LISTA VUOTA e bool=false se non e' feasible

SC e\' un signed char del grafo G
G Red-Black Graph

### REALIZE SPECIES
realizza i caratteri inattivi di una specie V (vertex) in G graph

- ritorna la lista di caratteri realizzati e bool = true se V is feasible for the Graph G
- realizzazione di S (vertex v) e' la realizzazione dei caratteri di S in qualsiasi ordine. un carattere attivo C che e' connesso a tutte le specie di RGB da RED EDGES e' chiamato LIBERO, ed e' quindi cancellato dall'RGB

V vertex (specie)
G Grafo Red-Black

### REALIZE
ritorna la lista di caratteri realizzati and bool = true se la realizzazione di LSC e' possibile in Graph G.

LSC lista di cratteri segnati
G redblack Graph

### is_complete 
COSA FA??

### Sort by degree
sorta le specie (vertici) dato il concetto di Vertex Degree

list_to_order 
G red-black-graph

### GET ALL MINIMAL P ACTIVE SPECIES
g redblack graph

RETURN The list of minimal p-active species in g Graph
if All bool=false , la lista conterra solo the first minimal p-active species incontrate

### GET quasi active species

Ritorna una quasi-active species in G se esiste
Un vertes S e' quasi-attivo in un RBGraph se ha un red_incoming edge, ma puo essere rimosso da G senza produrre un RED Sigma Graph

### ppp_maximal_reducible_graph
esegui l'aloritmo sul massimo grafo riducibile G
Ritorna la sequenza di caratteri realizzati

### Realize red Universal and universal characters

Realizza caratteri in G che sono RED Universal o Universal
Ritorn la lista di caratteri segnati realizzati

### Get Extension 

S a rbvertex
gmax red-black graph
gmin red-black graph

l'estesione di Specie

RETURN l'estensione di S

### Get Soruces

return the sources del Maximal Reducible Graph
gm redblack graph *maximal reducible graph

### is_2_solvable

sources- list of sources
gm red-black graph *maximal reducible graph

### closure
in v species
in g redblack graph

returns la chiusura CL di V(specie) in particolare a appertiene alla chiusura di Vertex se A e' minimale in G e e' incluso in tutti i maximal chracter di V (specie)

### PPR GENERAL
in RBGraph
A set di caratteri

iterates the graph se VALID, procedura generale per l'esecuzione dell'algoritmo 3.
General algorithm per grafi 2-solvable.

