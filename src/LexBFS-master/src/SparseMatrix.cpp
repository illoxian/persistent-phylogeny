/*
 * SparseMatrix.cpp
 *
 *  Created on: 10 May 2015
 *      Author: Hackob
 */

#include "SparseMatrix.h"

//assumption: vertex numbers appear in an ascending order
SparseMatrix::SparseMatrix(vector <Clique> cliques) { //the passed parameter is an array of rows
	numofCliques = cliques.size(); //save the number of cliques
	numofVertices = 0; //initial value before finding the biggest number of vertices occurring in cliques
	for (unsigned int i = 0; i < numofCliques; ++i) { //run over all the cliques
		if (numofVertices < cliques[i].getVertices().back() + 1) { //check for bigger value for number of vertices
			numofVertices = cliques[i].getVertices().back() + 1; //update the value
		}
	}
	sparseCliques = cliques; //copy the cliques vector
}

SparseMatrix::SparseMatrix(bool* binaryMatrix, int rows, int cols) {
	numofCliques = cols;
	numofVertices = rows;
	sparseCliques.resize(numofCliques);
	for (int i = 0; i < cols; ++i) {
		for (int j = 0; j < rows; ++j) {
			if (binaryMatrix[j*cols+i]) {
				sparseCliques[i].getVertices().push_back(j);
			}
		}
	}
}

SparseMatrix::~SparseMatrix() {
}

void SparseMatrix::printMatrix() {
	for (unsigned int j = 0; j < numofVertices; ++j) {
		for (unsigned int i = 0; i < numofCliques; ++i) {
			printf("%d  ", sparseCliques[i].containsXasOneEntry(j) ? 1 : 0);
		}
		printf("\n");
	}
}

//implementation of the algorithm 8 (using algorithm 10) after appending the identity matrix below
bool SparseMatrix::hasConsecutiveOnesProperty() {
	appendIdentityMatrix();		//append the identity matrix below the actual matrix (to make sure the cliques are maximal)
	//printf("Extended matrix M~:\n");
	//printMatrix();

	//1. Start with algorithm 10 instead of algorithm 4 to find the LexBFS order and the clique-tree
	//==============================================================================================
	//mark all the cliques unnumbered and without any labels
	for (unsigned int c=0; c<numofCliques; ++c) {
		sparseCliques[c].id = c;	//assign id to each clique as well
		sparseCliques[c].label.clear();	//clear all its labels
		sparseCliques[c].number = -1;	//assign number -1, which means unnumbered
	}
	
	unsigned int i = numofVertices;		// i <- n
	
//	vector<int> sigma;				//LexBFS order of matrix rows (i.e. vertices)
//	sigma.resize(numofVertices+1);
	vector<int> alpha;		//LexBFS order of matrix columns (i.e. cliques), so this will contain the sequence of clique IDs (e.g. 4,2,3,1)
	vector<int> Y;
	vector<int> last;
	vector<int> numberedVertices;	//container for keeping track of numbered vertices
	vector<int> treeParentArray;	//an array container for keeping the parent clique IDs instead of constructing the tree
	treeParentArray.resize(numofCliques);	//for one parent per each clique
	alpha.resize(numofCliques+1);
	Y.resize(numofVertices);
	last.resize(numofCliques);
	for (unsigned int index = 0; index < numofCliques; ++index) {	//initialise all to -1 as invalid values
		treeParentArray[index] = -1;
		alpha[index] = -1;
		last[index] = -1;
	}
	alpha[numofCliques] = -1;	//one additional item (specifics of the algorithm)
	for (unsigned int index = 0; index < numofVertices; ++index) {	//initialise all to -1 as invalid values
		Y[index] = -1;
	}
	for (unsigned int j = 1; j<numofCliques+1; ++j) {				// for j=1 to k (k was a mistake in the original algorithm in the paper!)
		//pick an unnumbered column C with the largest label
		int candidateCliqueIndex = -1;
		for (unsigned int ind = 0; ind < numofCliques; ++ind) {
			if (sparseCliques[ind].number != -1) {	//skip numbered cliques
				continue;
			}
			
			//find the one with the largest label
			if (isLabelGreaterThan(sparseCliques[ind].label, (candidateCliqueIndex == -1 ? sparseCliques[ind].label : sparseCliques[candidateCliqueIndex].label))) {
				candidateCliqueIndex = ind;
			}
		}
		
		sparseCliques[candidateCliqueIndex].number = j;	//number this clique in its special container aside from alpha set
		
		alpha[j] = candidateCliqueIndex;	//alpha(j) <- C		=> this meant to be numbering the picked clique!
		
		//connect C to Y(last(C))   => instead of the tree construction, we'll keep child-parent relationship like in a parent array for chordality test function
		if (last[candidateCliqueIndex] != -1) {
			treeParentArray[candidateCliqueIndex] = Y[last[candidateCliqueIndex]];
		}

		int x = -1;	//initialise to an invalid value
		while (sparseCliques[candidateCliqueIndex].containsUnnumberedOneEntry(numberedVertices, x)) {	//note: x is passed by reference!
			numberedVertices.push_back(x);	//number the vertex (by pushing it to a special container)
//			sigma[i] = x;
			Y[x] = j;
			for (unsigned int index=0; index<sparseCliques.size(); ++index) {	//for each unnumbered column C' that contains x as one entry
				if (sparseCliques[index].number != -1) {	//skip already numbered cliques
					continue;
				}
				if (sparseCliques[index].containsXasOneEntry(x)) {
					sparseCliques[index].label.push_back(i);	//Add i to label(C')
					last[index] = x;			//last(C') <- x
				}
			}
			--i;
		}	//end of while() loop
	}	//end of for() loop
	

	//Check for chordality (note: it will cost us more than linear time since we need the adjacency matrix, even with the sigma[] sequence acquired above)
	//====================================================================================================================================================
	//find the edges of the graph out of the cliques
//	vector<Edge> edges;
//	for (unsigned int i=0; i<numofCliques; ++i) {
//		for (unsigned v=0; v<sparseCliques[i].getVertices().size(); ++v) {
//			for (unsigned u=v+1; u<sparseCliques[i].getVertices().size(); ++u) {
//				Edge e = Edge(sparseCliques[i].getVertices()[v], sparseCliques[i].getVertices()[u]);
//				edges.push_back(e);
//			}
//		}
//	}
//	
//	//build the graph representing the matrix M~ and call its isChordal() method
//	Graph G(numofVertices, edges.size());
//	for (unsigned int i = 0; i<edges.size(); ++i) {
//		G.insertEdge(edges[i].u, edges[i].v);
//	}
//	
//	if (!G.isChordal()) {	//check for chordality
//		printf("Not a chordal graph!\n");
//		rollbackOriginalMatrix();		//roll-back to the original matrix before returning from function
//		return false;	//the matrix failed to pass the test for Consecutive-Ones!
//	}
	

	//2. Continue as in algorithm 8
	//=============================
	vector< vector<Clique*> > L;		//let L be the ordered list (~L~)	=> this means that initially there's only one class of cliques: the original list of all cliques
	vector<Clique*> cliqueClass;			//so create the class of all the cliques (using pointers for referring to a particular clique but clique IDs also could be used)
	for (unsigned int index=0; index<numofCliques; ++index) {
		cliqueClass.push_back(&sparseCliques[index]);
	}
	L.push_back(cliqueClass);	//and push it into the L
	
	stack<unsigned int> pivots;		//pivots is an empty stack
	
	vector<bool> processedVertices;	//will be used to check/keep track of processed vertices 
	for (unsigned int index=0; index<numofVertices; ++index) {
		processedVertices.push_back(false);
	}
	
	Clique* Cl;
	vector<Clique*> C;
	while(L.size() < numofCliques) {		//while there exists a non-singleton class Lc in L
		if (pivots.empty()) {	//pivots can be empty several times, not just only on the first run!
			//first let's find a non-singleton class (and refer to it as Lc)
			int indexOfLcInL = -1;
			for (unsigned int i = 0; i<L.size(); ++i) {	//run over all the elements of the L container
				if (L[i].size() > 1) {	//if found a non-singleton class
					indexOfLcInL = i;	//remember the index in the actual container L for later references (i.e. Lc is L[indexOfLcInL] from now on)
					break;
				}
			}
			
			//Let Cl be the last clique in Lc discovered by LexBFS (the clique with the greatest number)
			int indexOfCliqueWithTheGreatestNumber = 0;
			for (unsigned int i=1; i<L[indexOfLcInL].size(); ++i) {
				if (L[indexOfLcInL][i]->number > L[indexOfLcInL][indexOfCliqueWithTheGreatestNumber]->number) {
					indexOfCliqueWithTheGreatestNumber = i;
				}
			}
			Cl = L[indexOfLcInL][indexOfCliqueWithTheGreatestNumber];

			//Replace Lc by (Lc\{Cl}, {Cl}) in L	=> i.e. separate the Cl from Lc and put it at right of Lc in the L container
			L[indexOfLcInL].erase(L[indexOfLcInL].begin()+indexOfCliqueWithTheGreatestNumber);
			C.clear();
			C.push_back(Cl);	//C = {Cl}
			L.insert(L.begin() + indexOfLcInL+1, C);
			
			//check for the container L to remove any empty class
			for (unsigned int i = 0; i<L.size(); ++i) {	//run over all the classes of cliques
				if (L[i].size() == 0) {		//if empty
					L.erase(L.begin()+i);	//remove it
					--i;	//decrease the index not to miss the next item
				}
			}
		}
		else {
			while(pivots.size() > 0) {
				//pick an unprocessed vertex x in pivots (throw the processed ones)
				while (pivots.size() > 0 && processedVertices[pivots.top()]) {
					pivots.pop();	//throwing until we get an unprocessed vertex
				}
				if (pivots.size() == 0) {
					break;
				}
				processedVertices[pivots.top()] = true;	//marking it as processed to avoid later use
				
				//let C be the set of all the cliques containing x		=> note: we don't create a new variable x, but instead will be using pivots.top() for a reference
				C.clear();
				vector<bool> consecutivenessCheckBooleanContainer;	//array for checking the consecutiveness of the clique-containing classes
				consecutivenessCheckBooleanContainer.resize(L.size());
				for (unsigned int i = 0; i < L.size(); ++i) {
					consecutivenessCheckBooleanContainer[i] = false;
				}
				for (unsigned int index=0; index<numofCliques; ++index) {	//run over all the cliques
					if (sparseCliques[index].containsXasOneEntry(pivots.top())) {		//if a clique contains vertex x (i.e. pivots.top())
						C.push_back(&sparseCliques[index]);				//push it into C
						
						//after pushing the clique into the C container, find its container class and mark appropriately in the consecutivenessCheckBooleanContainer for later use
						for (unsigned int classIndex = 0; classIndex<L.size(); ++classIndex) {	//run over all the classes in container L
							if (doesClassContainClique(L[classIndex], sparseCliques[index].id)) {
								consecutivenessCheckBooleanContainer[classIndex] = true;
							}
						}
					}
				}
				
				//check for consecutiveness in consecutivenessCheckBooleanContainer array
				int numOfChanges = 0;	//if this value will increase more than 2 times then there were non-consecutive true values in the array
				bool lastValue = false;
				for (unsigned int i = 0; i < L.size(); ++i) {
					if (consecutivenessCheckBooleanContainer[i] != lastValue) {
						++numOfChanges;
						lastValue = !lastValue;
					}
				}
				
				int La = -1;
				int Lb = -1;
				if (numOfChanges < 3) {		//if all the cliques in C occur in consecutive classes
					//let La be the first class containing such a clique
					for (unsigned int i = 0; i < L.size(); ++i) {
						if (consecutivenessCheckBooleanContainer[i] == true) {
							La = i;
							break;
						}
					}
					
					//let Lb be the last class containing such a clique
					for (unsigned int i = La+1; i < L.size(); ++i) {
						if (consecutivenessCheckBooleanContainer[i] == false) {
							Lb = i-1;
							break;
						}
					}
					if (Lb == -1) {	//check if the true values are continuing till the end of the array
						Lb = L.size()-1;	//assign Lb to point to the last class in the L
					}
				}
				else {
					rollbackOriginalMatrix();		//roll-back to the original matrix before returning from function
					return false;	//the matrix failed to pass the test for Consecutive-Ones!
				}
				
				//if a class strictly between La and Lb contains a clique not in C
				for (int i = La+1; i < Lb; ++i) {
					for (unsigned int j = 0; j<L[i].size(); ++j) {	//run over all the cliques of the class L[i]
						if (!doesClassContainClique(C, L[i][j]->id)) {
							rollbackOriginalMatrix();		//roll-back to the original matrix before returning from function
							return false;	//the matrix failed to pass the test for Consecutive-Ones!
						}
					}
				}
				
				//replace La by (La\C, La intersected with C) and Lb by (Lb intersected with C, Lb\C)
				vector<Clique*> LaIntersectionWithC;
				for (int i = L[La].size()-1; i >= 0; --i) {	//run over all the cliques of the La from back to front
					if (doesClassContainClique(C, L[La][i]->id)) {	//if it belongs to the set of C
						LaIntersectionWithC.push_back(L[La][i]);	//push it into a new class of cliques
						L[La].erase(L[La].begin() + i);				//remove it from La
					}
				}
				vector<Clique*> LbIntersectionWithC;
				for (int i = L[Lb].size()-1; i >= 0; --i) {	//run over all the cliques of the Lb from back to front
					if (doesClassContainClique(C, L[Lb][i]->id)) {	//if it belongs to the set of C
						LbIntersectionWithC.push_back(L[Lb][i]);	//push it into a new class of cliques
						L[Lb].erase(L[Lb].begin() + i);				//remove it from Lb
					}
				}
				L.insert(L.begin() + Lb, LbIntersectionWithC);	//insert the new class of cliques removed from Lb right before Lb (do this first so La index will not change)
				L.insert(L.begin() + La+1, LaIntersectionWithC);	//insert the new class of cliques removed from La right after La (do this after Lb so indices will not change)
				
				//check for the container L to remove L[La] and L[Lb] in case they become empty after erase() function calls in the above loops
				for (unsigned int i = 0; i<L.size(); ++i) {	//run over all the classes of cliques
					if (L[i].size() == 0) {		//if empty
						L.erase(L.begin()+i);	//remove it
						--i;	//decrease the index not to miss the next item
					}
				}
				
			}
		}
		
		//for each remaining tree edge CiCj connecting a clique Ci (belonging to C) to a clique Cj (not belonging to C)
		for (unsigned int i = 0; i < treeParentArray.size(); ++i) {
			if (treeParentArray[i] == -1) {
				continue;
			}
			
			//check the condition denoting Ci as the clique with number i and Cj as the clique with number treeParentArray[i]
			if (
				(doesClassContainClique(C, i) && !doesClassContainClique(C, treeParentArray[i]))
				||
				(!doesClassContainClique(C, i) && doesClassContainClique(C, treeParentArray[i]))
			) {
					//find the intersection vertices to push into the pivots container
					for (unsigned int index = 0; index < sparseCliques[i].getVertices().size(); ++index) {	//run over all of the vertices of the clique Ci
						if (sparseCliques[treeParentArray[i]].containsXasOneEntry(sparseCliques[i].getVertices()[index])) {	//if Cj also contains this vertex 
							pivots.push(sparseCliques[i].getVertices()[index]);		//push it into the pivots container
						}
					}
					treeParentArray[i] = -1;	//remove CiCj from the clique tree
			}
		}
		
	}	//end of the while() loop, algorithm 8
	
	//print out the final sequence of the matrix columns
	/*printf("The reordered columns sequence: ");
	for (unsigned int i=0; i<L.size(); ++i) {
		printf(" %d", L[i][0]->id);
	}
	printf("\n");*/
	
	rollbackOriginalMatrix();		//roll-back to the original matrix before returning from function
	return true;	//the matrix has passed the test for Consecutive-Ones!
}

void SparseMatrix::appendIdentityMatrix() {
	for (unsigned int i = 0; i < sparseCliques.size(); ++i) {
		sparseCliques[i].getVertices().push_back(numofVertices + i);
	}
	numofVertices += sparseCliques.size();
}

void SparseMatrix::rollbackOriginalMatrix() {
	for (unsigned int i = 0; i < sparseCliques.size(); ++i) {
		sparseCliques[i].getVertices().pop_back();
	}
	numofVertices -= sparseCliques.size();
}

bool SparseMatrix::isLabelGreaterThan(vector<int> label1, vector<int> label2) {
	for (unsigned int i=0; i<label1.size(); ++i) {
		if (label2.size() <= i) {
			return true;
		}
		if (label1[i] < label2[i]) {
			return false;
		}
	}
	return true;
}

bool SparseMatrix::doesClassContainClique(vector<Clique*> cliqueClass, int cliqueID) {
	for (unsigned int i=0; i<cliqueClass.size(); ++i) {
		if (cliqueClass[i]->id == cliqueID) {
			return true;
		}
	}
	return false;
}
