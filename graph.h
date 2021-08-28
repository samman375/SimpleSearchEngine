// Interface to graph ADT for COMP2521 assignment 2 (modified from Lab 6)
// by Samuel Thorley (z5257239) Trimester 1 2020

#include <stdio.h>

// graph representation is hidden
typedef struct GraphRep *Graph;

// graph representation (adjacency matrix)
typedef struct GraphRep {
	int nV;		 // #vertices
	int nE;		 // #edges
	int **edges; // matrix of weights (0 == no link)
} GraphRep;

// vertices denoted by integers 0..N-1
typedef int Vertex;
int validV (Graph, Vertex); // validity check

// link between two vertices
typedef struct {
	Vertex v;
	Vertex w;
} Edge;

// Link operations
void insertEdge (Graph, Vertex, Vertex, double);
void removeEdge (Graph, Vertex, Vertex);

// operations on graphs
Graph newGraph (int nV);
void dropGraph (Graph);


