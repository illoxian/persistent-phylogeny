/*
 * Edge.h
 *
 *  Created on: 6 Jun 2015
 *      Author: Hackob
 */

#ifndef EDGE_H_
#define EDGE_H_

class Edge {
public:
	int u, v;
	Edge(int u = 0, int v = 0)
			: u(u), v(v) {
	}
	int otherEnd(int a = 0) {
		return (u == a) ? v : u;
	}
};

#endif /* EDGE_H_ */
