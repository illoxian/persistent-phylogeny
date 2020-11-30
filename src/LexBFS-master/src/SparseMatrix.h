/*
 * SparseMatrix.h
 *
 *  Created on: 10 May 2015
 *      Author: Hackob
 */

#ifndef SPARSEMATRIX_H_
#define SPARSEMATRIX_H_

#include "Clique.h"
#include "SparseMatrix.h"
#include "Edge.h"
#include "Graph.h"
#include <cstdio>
#include <vector>
#include <list>
using namespace std;

class SparseMatrix {
private:
	vector <Clique> sparseCliques;
	unsigned int numofCliques;
	unsigned int numofVertices;

	void appendIdentityMatrix();
	void rollbackOriginalMatrix();	//must be called after appendIdentityMatrix()
	bool isLabelGreaterThan(vector<int> label1, vector<int> label2);
	bool doesClassContainClique(vector<Clique*> cliqueClass, int cliqueID);
public:
	SparseMatrix(vector <Clique> cliques);
	SparseMatrix(bool* binaryMatrix, int rows, int cols);
	virtual ~SparseMatrix();
	
	void printMatrix();
	bool hasConsecutiveOnesProperty();
};

#endif /* SPARSEMATRIX_H_ */
