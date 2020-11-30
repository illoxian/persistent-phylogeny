/*
 * Graph.cpp
 *
 *  Created on: 14 Apr 2015
 *      Author: Hackob
 */

#include "Graph.h"

//N=number of nodes, M=number of edges
Graph::Graph(int n, int m)
		: Adj(n), E(m), n(n), m(m) {
	//initialise the binary array A[n][n] with false values
	A.resize(n);
	for (int i = 0; i < n; i++) {
		A[i].resize(n);
	}
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			A[i][j] = false;
		}
	}
}

//Uses the fact that for chordal graphs to sort vertices directed by Lex-BFS is a perfect elimination scheme for the graph if G is chordal.
//Algorithm 3 at page 6 of the paper by Habib, McConnell, Paul, Viennot.
bool Graph::isChordal() {
	int x, v;
	unsigned int i, j;
	vector < vector < int > > RN(n + 1);	//values from 0 to n, not (n-1) because we need to compare with the root of the tree of parent pointers which will have the value n
	int *parent = new int[n];
	int *pi = Lex_BFS();

	//Build RN(x) and parent(x), such that RN(x) contains all the neighbours of x that are at right of x in the sequence pi, and parent(x) is the leftmost of them
	for (x = 0; x < n; x++) {
		parent[x] = n;	//make it as root initially
		for (i = 0; i < degree(x); i++) { //for all neighbours of x
			v = getIthNeighbour(x, i);
			if (pi[v] > pi[x]) { //if v is at the right of x
				RN[x].push_back(v); //push it to the respective RN vector for x
				if (pi[v] < pi[parent[x]]) { //if v is at the left of previously encountered leftmost member
					parent[x] = v; //update the leftmost member
				}
			}
		}
	}
	
	//Check whether RN(x)\parent[x] is contained in RN(parent[x])
	bool isContained;
	for (x = 0; x < n; x++) {
		for (i = 0; i < RN[x].size(); i++) {
			if (RN[x][i] == parent[x]) {	//skip the node parent[x]
				continue;
			}
			
			isContained = false;
			for (j = 0; j < RN[parent[x]].size(); j++) {
				if (RN[x][i] == RN[parent[x]][j]) {
					isContained = true;
					break;
				}
			}
			
			if (!isContained) {
				//cleanup pointers before returning from the function
				delete pi;
				delete parent;
				
				return false;	//chordality test fails!
			}
		}
	}
	
	//cleanup pointers before returning from the function
	delete pi;
	delete parent;

	return true;	//chordality test passes!
}

//implemented according to the LexBFS algorithm described in page 5 of the paper by Habib, McConnell, Paul, Veinnot.
int* Graph::Lex_BFS() {
	int* pi = new int[n + 1];
	List::iterator it;
	vector < iListS > pos;
	int u, x, i;
	ListS L;
	List *S, *Y;
	iListS X;

	//init S with all the vertices
	S = new List();
	for (u = 0; u < n; u++) {
		S->push_back(u);
	}

	//L=(V); //L receives an ordered set of vertices (V)
	L.push_back(S);

	pi[n] = n;
	i = n - 1;

	while (!L.empty()) {
		S = L.front();
		x = S->front();
		S->pop_front();
		if (S->empty()) { //if S becomes empty
			L.pop_front(); //remove it from the container
			delete S;
		}

		//x receives the first element of the last class composed of unvisited vertices Xa
		pi[x] = i;
		i--;

		//refinement of each partition X in L
		for (X = L.begin(); X != L.end();) { //for each class Xb with b<=a do:
			Y = new List(); //Y is the members of Xb that are adjacent to x
			S = *X;

			//Find members in Xb that are adjacent to x and add them into Y
			for (it = S->begin(); it != S->end();) {
				if (A[x][*it]) {
					Y->push_back(*it);
					it = S->erase(it);
				}
				else {
					it++;
				}
			}

			//If Y is non-empty, insert before Xb (Y will be containing neighbouring vertices)
			if (Y->size() > 0) {
				L.insert(X, Y);
			}
			else {
				delete Y;
			}

			//check if S became empty, remove Xb from the list L
			if (S->size() == 0) {
				X = L.erase(X);
			}
			else {
				X++;
			}
		}
	}

	return pi;
}

void Graph::print() {
	for (int u = 0; u < n; u++) {
		printf("%d:\n", u);
		for (unsigned int i = 0; i < Adj[u].size(); i++) {
			printf(" %d", getIthNeighbour(u, i));
		}
		printf("\n");
	}
}
