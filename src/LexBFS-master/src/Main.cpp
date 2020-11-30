/*
 * Graph.h
 *
 *  Created on: 14 Apr 2015
 *      Author: Hackob
 */

#include "Graph.h"
#include "Clique.h"
#include "SparseMatrix.h"

void chordalityTest() {
  Graph G(7, 11);
  G.insertEdge(0, 1);
  G.insertEdge(1, 2);
  G.insertEdge(2, 3);
  G.insertEdge(3, 0);
  G.insertEdge(0, 2);

  G.insertEdge(1, 5);
  G.insertEdge(1, 4);
  G.insertEdge(5, 4);
  G.insertEdge(2, 4);

  G.insertEdge(6, 3);
  G.insertEdge(6, 2);

  //G.print();

  if (G.isChordal()) {
    printf("The graph is chordal!\n");
  }
  else {
    printf("The graph is NOT chordal!\n");
  }
}

void matrixTest2() {
  bool binaryMatrix[9] = {
		  0, 1, 0,
		  1, 0, 1,
		  0, 1, 0
  };

  SparseMatrix M(binaryMatrix, 3, 3);

  printf("Original matrix M:\n");
  M.printMatrix();

  if (M.hasConsecutiveOnesProperty()) {
    printf("The matrix has Consecutive-Ones property!\n");
  }
  else {
    printf("The matrix does NOT have Consecutive-Ones property!\n");
  }
}

void matrixTest3() {
  bool binaryMatrix[9] = {
		  1, 0, 1,
		  1, 1, 0,
		  0, 1, 1 };

  SparseMatrix M(binaryMatrix, 3, 3);

  printf("Original matrix M:\n");
  M.printMatrix();

  if (M.hasConsecutiveOnesProperty()) {
    printf("The matrix has Consecutive-Ones property!\n");
  }
  else {
    printf("The matrix does NOT have Consecutive-Ones property!\n");
  }
}

void matrixTest4() {
  bool binaryMatrix[12] = {
		  1, 0, 1, 0,
		  1, 1, 0, 1,
		  0, 1, 1, 1
  };

  SparseMatrix M(binaryMatrix, 3, 4);

  printf("Original matrix M:\n");
  M.printMatrix();

  if (M.hasConsecutiveOnesProperty()) {
    printf("The matrix has Consecutive-Ones property!\n");
  }
  else {
    printf("The matrix does NOT have Consecutive-Ones property!\n");
  }
}

void matrixTest4_withNonDuplicateColumns() {
  bool binaryMatrix[28] = {
		  1, 0, 1, 0,
		  1, 1, 0, 1,
		  0, 1, 1, 1,
		  1, 0, 0, 0,
		  0, 1, 0, 0,
		  0, 0, 1, 0,
		  0, 0, 0, 1
  };

  SparseMatrix M(binaryMatrix, 7, 4);

  printf("Original matrix M:\n");
  M.printMatrix();

  if (M.hasConsecutiveOnesProperty()) {
    printf("The matrix has Consecutive-Ones property!\n");
  }
  else {
    printf("The matrix does NOT have Consecutive-Ones property!\n");
  }
}

void matrixTest5() {
  bool binaryMatrix[48] = {
		  0, 0, 0, 0, 1, 0,
		  0, 0, 1, 0, 0, 0,
		  0, 0, 0, 1, 0, 0,
		  0, 0, 0, 0, 0, 1,
		  0, 1, 0, 0, 0, 0,
		  1, 0, 0, 1, 1, 0,
		  1, 1, 0, 0, 0, 1,
		  1, 1, 1, 1, 0, 1
  };

  SparseMatrix M(binaryMatrix, 8, 6);

  printf("Original matrix M:\n");
  M.printMatrix();

  if (M.hasConsecutiveOnesProperty()) {
    printf("The matrix has Consecutive-Ones property!\n");
  }
  else {
    printf("The matrix does NOT have Consecutive-Ones property!\n");
  }
}

int main(int argc, char* argv[]) {

//	chordalityTest();

	matrixTest4_withNonDuplicateColumns();

  return 0;
}
