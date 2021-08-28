// Implementation of Graph ADT for COMP2521 assignment 2
// by Samuel Thorley (z5257239) Trimester 1 2020

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"

// check validity of Vertex (modified from Lab 6)
int validV (Graph g, Vertex v)
{
	return (g != NULL && v >= 0 && v < g->nV);
}


// remove an Edge (modified from Lab 6)
void removeEdge (Graph g, Vertex v, Vertex w)
{
	assert (g != NULL && validV (g, v) && validV (g, w));
	if (g->edges[v][w] == 0) {
		// an edge doesn't exist; do nothing.
		return;
	}

	g->edges[v][w] = 0;
	g->nE--;
}

// insert a one directional Edge (modified from Lab 6)
void insertEdge (Graph g, Vertex v, Vertex w, double wt)
{
	assert (g != NULL && validV (g, v) && validV (g, w));

	if (g->edges[v][w] != 0) {
		// an edge already exists; do nothing.
		return;
	}

	g->edges[v][w] = g->edges[v][w] + wt;
	g->nE++;
}

// create an empty graph (modified from Lab 6)
Graph newGraph (int nV)
{
	assert (nV > 0);

	GraphRep *new = malloc (sizeof *new);
	assert (new != NULL);
	*new = (GraphRep){ .nV = nV, .nE = 0 };

	new->edges = calloc ((size_t) nV, sizeof (int *));
	assert (new->edges != NULL);
	for (int v = 0; v < nV; v++) {
		new->edges[v] = calloc ((size_t) nV, sizeof (int));
		assert (new->edges[v] != 0);
	}

	return new;
}

// free memory associated with graph (modified from Lab 6)
void dropGraph (Graph g)
{
	assert (g != NULL);
	for (int v = 0; v < g->nV; v++)
		free (g->edges[v]);
	free (g->edges);
	free (g);
}










