/*
 * Clique.cpp
 *
 *  Created on: 28 May 2015
 *      Author: Hackob
 */

#include "Clique.h"

Clique::Clique() {
	//initialise member variables to a negative number to avoid uninitialised warning
	id = -1;
	number = -1;
	
	this->vertices.resize(0);
}

Clique::Clique(vector <unsigned int> _vertices) {
	//initialise member variables to a negative number to avoid uninitialised warning
	id = -1;
	number = -1;
	
	for (unsigned int i = 0; i < _vertices.size(); ++i) {
		this->vertices.resize(_vertices.size());
		this->vertices[i] = _vertices[i];
	}
}

Clique::~Clique() {
}

bool Clique::containsUnnumberedOneEntry(vector<int> numberedVerticesOfAClique, int& x) {
	bool isFound;
	for (unsigned int index = 0; index < vertices.size(); ++index) {	//run over all the vertices of this Clique
		isFound = false;
		for (unsigned int j = 0; j < numberedVerticesOfAClique.size(); ++j) {	//try to find a matching vertex in the numberedVerticesOfAClique
			if (vertices[index] - numberedVerticesOfAClique[j] == 0) {		//vertices[index] contains 1 entries, and -1 means unnumbered
				isFound = true;
				break;
				
			}
		}
		if (!isFound) {
			x = vertices[index];
			return true;
		}
	}
	return false;
}

bool Clique::containsXasOneEntry(unsigned int x) {
	for (unsigned int i = 0; i < vertices.size(); ++i) {
		if (vertices[i] == x) {
			return true;
		}
	}
	return false;
}

vector <unsigned int>& Clique::getVertices() {
	return vertices;
}
