/*
 * Graph.h
 *
 *  Created on: 14 Apr 2015
 *      Author: Hackob
 */

#ifndef GRAPH_H_
#define GRAPH_H_

#include <cstdio>
#include <list>
#include <vector>	//for vectors
using namespace std;

#include "Edge.h"

typedef vector < int > Ladj;
typedef list < int > List;
typedef list < List* > ListS;
typedef ListS::iterator iListS;

struct Graph {

private:
	vector < Ladj > Adj;
	vector < Edge > E;
	vector < vector < bool > > A; //being used in Lex_BFS() function for quickly identifying whether vertices are adjacent or no
	int n, m;

public:

	Graph(int n = 0, int m = 0); //constructor

	//insert edge
	inline
	void insertEdge(int u, int v) {
		int i;
		i = E.size();
		E.push_back(Edge(u, v));
		Adj[u].push_back(i);
		Adj[v].push_back(i); //if G is an undirected graph
		A[u][v] = A[v][u] = true;
	}

	//degree of vertex u
	inline
	unsigned int degree(int u) {
		return Adj[u].size();
	}

	//get i-th vertex adjacent to u
	inline
	int getIthNeighbour(int u, int i) {
		return E[Adj[u][i]].otherEnd(u);
	}

	int* Lex_BFS(); //execute Lex-BFS in graph
	bool isChordal(); //check if graph is chordal
	void print(); //print the graph
};

#endif /* GRAPH_H_ */
