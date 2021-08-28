// Interface to part 1 functions for COMP2521 assignment 2
// by Samuel Thorley (z5257239) Trimester 1 2020

#include <stdio.h>

#include "graph.h"

#define MAX_LINKS 1000
#define MAX_LINE_LENGTH 1000
#define MAX_NAME 7 //'url' + up to 3 digits + '/0'
#define MAX_URL 11 //'url' + up to 3 digits + '.txt/0'

// (modifed from lab 4)
typedef struct node *Node;
struct node {
	char name[MAX_NAME]; 
	Node next;
	int index;
	double rank;
	int outDegree;
	int inDegree;
};

// (taken from lab 4)
typedef struct list *List;
struct list {
	Node head;
	int nV;
};


// Prototype functions


// Calls GetCollection() and GetGraph and updates returns list with updated ranks
List PageRankW(double dFactor, double diffPR, int maxIterations);

// Sets all existing url ranks to 1/N
void setInitalRank(List l, int n);

// Returns array with rank values from given list
void rankToArray(List l, double *array);

// Updates ranks of list with values from array
void arrayToList(List l, double *array);

// Returns sum PR * wOut * wIn of nodes with outgoing links to i
double sumValue(Graph g, List l, int i);

// Returns wOut for given vertices(u,v)
double findWOut(Graph g, List l, int u, int v);

// Returns wOut for given vertics(u,v)
double findWIn(Graph g, List l, int u, int v);

// Writes to pagerankList.txt with given list
void writeToDocument(List l);

// Creates new node
Node newNode(char *fileName);

// Creates new list
List newList();

// Adds node to list and updates index
void addNode(List l, Node new);

// Reads collection.txt and stores filenames into list
List GetCollection();

// Creates graph with links after reading files from given list
Graph GetGraph(List l);

// Returns index of a url with given name
int findIndex(char *fileName, List l);

// Finds name of file when given index
char *findName(int i, List l);

// display graph, using names for vertices for testing purposes (taken from Lab 6)
void showGraph (Graph g, List l);

// Increases stored out degree for given index
void increaseOutdegree(List l, int i);

// Increases stored in degree for given index
void increaseIndegree(List l, int i);

// Returns rank of url given index
double showRank(int i, List l);

// Returns out degree of url given index
int showOutDegree(int i, List l);

// Returns in degree of url given index
int showInDegree(int i, List l);

// Given List prints values for testing purposes
void printList(List l);









