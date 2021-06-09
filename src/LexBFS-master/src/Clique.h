/*
 * Clique.h
 *
 *  Created on: 28 May 2015
 *      Author: Hackob
 */

#ifndef CLIQUE_H_
#define CLIQUE_H_

#include <vector>
#include <stack>
using namespace std;

class Clique {
private:
	vector <unsigned int> vertices;
public:
	Clique();
	Clique(vector <unsigned int> _vertices);
	virtual ~Clique();
	
	int id;
	int number;
	vector<int> label;
	
	vector <unsigned int>& getVertices();
	bool containsUnnumberedOneEntry(vector<int> numberedVerticesOfAClique, int& x);
	bool containsXasOneEntry(unsigned int x);
	
};

#endif /* CLIQUE_H_ */
