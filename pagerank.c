// Implementation of part 1 functions for COMP2521 Assignment 2
// by Samuel Thorley (z5257239) Trimester 1 2020


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "pagerank.h"


int main(int argc, char *argv[]) {
    char *p;
    double d = strtod(argv[1], &p);
    double diffPR = strtod(argv[2], &p);
    int maxIterations = atoi(argv[3]);
    
    List adjustedL = PageRankW(d, diffPR, maxIterations);
    
    // Output values to pagerankList.txt
    writeToDocument(adjustedL);
    
    return 0;
}


// Helper functions

List PageRankW(double d, double diffPR, int maxIterations) {
    List l = GetCollection();
    Graph g = GetGraph(l);
    int n = g->nV;
    setInitalRank(l, n);
    
    int iteration = 0;
    double diff = diffPR;
    while (iteration < maxIterations && diff >= diffPR) {
        double prevArray[MAX_LINKS];
        rankToArray(l, prevArray);
        double newArray[MAX_LINKS];
        // Update values of newArray
        int i = 0;
        while (i < g->nV) {
            newArray[i] = (1 - d)/n + d * sumValue(g, l, i);
            i++;
        }
        // Update list ranks with new ranks
        arrayToList(l, newArray);
        // Check absolute difference
        int j = 0;
        diff = 0;
        while (j < g->nV) {
            double abs = prevArray[j] - newArray[j];
            if (abs < 0) {
                abs = abs * -1;
            }
            diff = diff + abs;
            j++;
        }
        iteration++;
    }
    return l;
}

void setInitalRank(List l, int n) {
    double r = (double) 1/n;
    Node curr = l->head;
    while (curr != NULL) {
        curr->rank = r;
        curr = curr->next;
    }
    return;
}

void rankToArray(List l, double *array) {
    int i = 0;
    Node curr = l->head;
    while (curr != NULL && i < MAX_LINKS) {
        array[i] = curr->rank;
        i++;
        curr = curr->next;
    }
    return;
}

void arrayToList(List l, double *array) {
    int i = 0;
    Node curr = l->head;
    while (i < l->nV && curr != NULL) {
        curr->rank = array[i];
        curr = curr->next;
        i++;
    }
    return;
}

double sumValue(Graph g, List l, int i) {
    double sum = 0;
    int j = 0;
    while (j < g->nV) {
        // Has edge, ignoring self loops
        if (g->edges[j][i] != 0 && i != j) {
            // calculate PR, wIn, wOut add to sum
            double jPR = showRank(j, l);
            double wOut = findWOut(g, l, j, i);
            double wIn = findWIn(g, l, j, i);
            sum = sum + jPR * wIn * wOut;
        }
        j++;
    }
    return sum;
}

double findWOut(Graph g, List l, int u, int v) {
    double uOutDegree = 0;
    double vOutDegree = showOutDegree(v, l);
    int j = 0;
    while (j < g->nV) {
        // Has outlink ignoring self loops
        if (g->edges[u][j] != 0 && u != j) {
            double jOutDegree = showOutDegree(j, l);
            if (jOutDegree == 0) {
                jOutDegree = 0.5;
            }
            uOutDegree = uOutDegree + jOutDegree;
        }
        j++;
    }
    if (uOutDegree == 0) {
        uOutDegree = 0.5;
    }
    if (vOutDegree == 0) {
        vOutDegree = 0.5;
    }
    double wOut = (double) vOutDegree / uOutDegree;
    return wOut;
};

double findWIn(Graph g, List l, int u, int v) {
    double uInDegree = 0;
    double vInDegree = showInDegree(v, l);
    int j = 0;
    while (j < g->nV) {
        if (g->edges[u][j] != 0 && u != j) {
            double jInDegree = showInDegree(j, l);
            if (jInDegree == 0) {
                jInDegree = 0.5;
            }
            uInDegree = uInDegree + jInDegree;
        }
        j++;
    }
    if (uInDegree == 0) {
        uInDegree = 0.5;
    }
    if (vInDegree == 0) {
        vInDegree = 0.5;
    }
    double wIn = (double) vInDegree / uInDegree;
    return wIn;
}


void writeToDocument(List l) {
    FILE *f = NULL;
    f = fopen("pagerankList.txt", "w+");
    assert(f != NULL);
    int read[MAX_LINKS] = {0};
    int iteration = 0;
    while (iteration < l->nV) {
        Node curr = l->head->next;
        Node highest = l->head;
        while (read[highest->index] != 0) {
            curr = curr->next;
            highest = highest->next;
        }
        while (curr != NULL) {
            if (curr->rank > highest->rank && read[curr->index] == 0) {
                highest = curr;
            }
            curr = curr->next;
        }
        read[highest->index] = 1;
        fprintf(f, "%s, %d, %.7f\n", highest->name, highest->outDegree, highest->rank);
        iteration++;
    }
    fclose(f);
    return;
}

List GetCollection() {
    FILE *collection = NULL;
    char temp[MAX_NAME] = "\0";
    collection = fopen("collection.txt", "r");
    assert(collection != NULL);
    List l = newList();
    while (fscanf(collection, "%6s", temp) != EOF) {
        Node new = newNode(temp);
        addNode(l, new);
    }
    fclose(collection);
    return l;
}

Graph GetGraph(List l) {
    Graph g = newGraph(l->nV);
    Node curr = l->head;
    while (curr != NULL) {
        FILE *url = NULL;
        char temp[MAX_URL] = "\0";
        strcpy(temp, curr->name);
        char type[5] = ".txt";
        strcat(temp, type);
        url = fopen(temp, "r");
        assert(url != NULL);
        char buffer[MAX_LINE_LENGTH] = "\0";
        while (fscanf(url, "%999s", buffer) == 1) {
            // Read filenames only in section 1
            if (strcmp(buffer, "#start") == 0 || 
                strcmp(buffer, "Section-1") == 0
            ) {
                continue;
            } else if (strcmp(buffer, "#end") == 0) {
                break;
            }
            int inLink = findIndex(buffer, l);
            int outLink = findIndex(curr->name, l);
            // Non existent file
            if (inLink == -1 || outLink == -1) {
                continue;
            }
            // Create link and update inDegree and outDegree number if not self loop
            insertEdge(g, outLink, inLink, 1);
            if (outLink != inLink) {
                increaseOutdegree(l, outLink);
                increaseIndegree(l, inLink);
            }
        }
        fclose(url);
        url = NULL;
        curr = curr->next;
    }
    return g;
}

Node newNode(char *fileName) {
    struct node *new = malloc(sizeof(*new));
    strcpy(new->name, fileName);
    new->next = NULL;
    new->index = 0;
    new->outDegree = 0;
    new->inDegree = 0;
    new->rank = 1;
    return new;
}

List newList() {
    struct list *l = malloc(sizeof(*l));
    l->head = NULL;
    l->nV = 0;
    return l;
}

void addNode(List l, Node new) {
    Node curr = l->head;
    int i = 0;
    if (curr == NULL) {
        new->index = i;
        l->head = new;
    } else {
        while (curr->next != NULL) {
            curr = curr->next;
            i++;
        }
        i++;
        new->index = i;
        curr->next = new;
    }
    l->nV++;
}

int findIndex(char *fileName, List l) {
    Node curr = l->head;
    while (strcmp(fileName, curr->name) != 0 && curr != NULL) {
        curr = curr->next;
    }
    // File not in list
    if (curr == NULL) {
        return -1; 
    } else {
        return curr->index;
    }
}

char *findName(int i, List l) {
    Node curr = l->head;
    while (i != curr->index && curr != NULL) {
        curr = curr->next;
    }
    // File not in list
    if (curr == NULL) {
        return 0;
    } else {
        return curr->name;
    }
}

void increaseOutdegree(List l, int i) {
    Node curr = l->head;
    while (i != curr->index && curr != NULL) {
        curr = curr->next;
    }
    // File not in list
    if (curr == NULL) {
        return;
    } else {
        curr->outDegree++;
    }
}

void increaseIndegree(List l, int i) {
    Node curr = l->head;
    while (i != curr->index && curr != NULL) {
        curr = curr->next;
    }
    // File not in list
    if (curr == NULL) {
        return;
    } else {
        curr->inDegree++;
    }
}

double showRank(int i, List l) {
    Node curr = l->head;
    while (i != curr->index && curr != NULL) {
        curr = curr->next;
    }
    // File not in list
    if (curr == NULL) {
        return -1;
    } else {
        return curr->rank;
    }
}

int showOutDegree(int i, List l) {
    Node curr = l->head;
    while (i != curr->index && curr != NULL) {
        curr = curr->next;
    }
    // File not in list
    if (curr == NULL) {
        return -1;
    } else {
        return curr->outDegree;
    }
}

int showInDegree(int i, List l) {
    Node curr = l->head;
    while (i != curr->index && curr != NULL) {
        curr = curr->next;
    }
    // File not in list
    if (curr == NULL) {
        return -1;
    } else {
        return curr->inDegree;
    }
}

// (taken from lab 6)
void showGraph (Graph g, List l)
{
	assert (g != NULL);
	printf ("#vertices=%d, #edges=%d\n\n", g->nV, g->nE);
	int v, w;
	for (v = 0; v < g->nV; v++) {
		printf ("%d %s\n", v, findName(v, l));
		for (w = 0; w < g->nV; w++) {
			if (g->edges[v][w]) {
				printf ("\t%s (%d)\n", findName(w, l), g->edges[v][w]);
			}
		}
		printf("Rank = %lf, Out Degree = %d.\n", showRank(v, l), showOutDegree(v, l));
		printf ("\n");
	}
}

void printList(List l) {
    Node curr = l->head;
    while (curr != NULL) {
        printf(
            "Index: %d, Name: %s, InDegree: %d, OutDegree: %d, Rank: %.7f\n", 
            curr->index, curr->name, curr->inDegree, curr->outDegree, curr->rank
        );
        curr = curr->next;
    }
    return;
}











