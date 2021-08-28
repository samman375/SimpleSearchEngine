// Interface to Part 2 functions for CS2521 assignment 2
// by Samuel Thorley (z5257239) Trimester 1 2020

#include<stdio.h>

#define MAX_LINE_LENGTH 1000
#define MAX_NAME 7 //'url' + up to 3 digits + '/0'

typedef struct node *Node;
struct node {
    char url[MAX_NAME];
    int nMentions;
    double rank;
    Node next;
};

typedef struct list *List;
struct list {
    Node head;
};



// Helper functions


// Returns a new list
List newList();

// Adds node to end of list
void addNode(List l, Node n);

// Returns rank of url from pagerankList.txt
double updateRank(char *name);

// Returns 1 if URL, 0 if not
int isUrl(char *temp);

// Checks if word is in list; 1 == true, 0 == false
int inList(List l, char *word);

// Returns new node
Node newNode(char *url, int n);

// Increases nMentions of specified url
void increaseMentions(List l, char *url);

// Prints list
void printList(List l);

// Rearranges list until ordered
void orderList(List l);

// Swaps values of two nodes
void swapValues(Node a, Node b);







