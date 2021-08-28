// Implementation of Part 2 functions for COMP2521 assignment 2
// by Samuel Thorley (z5257239) Trimester 1 2020

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#include "searchPagerank.h"

int main(int argc, char *argv[]) {
    int i = 1;
    List l = newList();
    while (i < argc) {
        FILE *inverted = NULL;
        inverted = fopen("invertedIndex.txt", "r");
        assert(inverted != NULL);
        char word[MAX_LINE_LENGTH] = "\0";
        strcpy(word, argv[i]);
        char temp[MAX_LINE_LENGTH] = "\0";
        int condition = 0;
        while (fscanf(inverted, "%999s", temp) != EOF) {
            if (strcmp(temp, word) != 0 && condition != 1) {
                continue;
            }
            // word found
            if (strcmp(temp, word) == 0) {
                condition = 1;
                continue;
            }
            // while temp is url add to list
            if (isUrl(temp) == 1) {
                // word already in list, increase nMentions
                if (inList(l, temp) == 1) {
                    increaseMentions(l, temp);
                } else {
                    Node n = newNode(temp, 1);
                    addNode(l, n);
                }
                continue;
            } else {
                break;
            }
        }
        i++;
    }
    
    // Order list
    if (l == NULL) {
        return 0;
    }
    
    orderList(l);
    
    printList(l);

    return 0;
}

// Helper functions

List newList() {
    struct list *l = malloc(sizeof(*l));
    l->head = NULL;
    return l;
}

void addNode(List l, Node n) {
    if (l->head == NULL) {
        l->head = n;
    } else {
        Node curr = l->head;
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = n;
    }
}

Node newNode(char *url, int n) {
    struct node *new = malloc(sizeof(*new));
    strcpy(new->url, url);
    new->next = NULL;
    new->nMentions = n;
    new->rank = updateRank(new->url);
    assert(new->rank != -1);
    return new;
}

double updateRank(char *name) {
    FILE *pagerank;
    pagerank = fopen("pagerankList.txt", "r");
    assert(pagerank != NULL);
    char temp[MAX_LINE_LENGTH] = "\0";
    char url[8] = "\0";
    strcpy(url, name);
    char comma[2] = ",";
    strcat(url, comma);
    int condition = 0;
    while (fscanf(pagerank, "%11s", temp) != EOF) {
        if (strcmp(temp, url) != 0 && condition == 0) {
            continue;
        }
        if (strcmp(temp, url) == 0) {
            condition = 1;
            continue;
        }
        if (condition == 1) {
            condition++;
        } else if (condition == 2) {
            char *p;
            double rank = strtod(temp, &p);
            return rank;
        } else {
            break;
        }
    }
    return -1;
}

int isUrl(char *temp) {
    if (temp[0] == 'u' && temp[1] == 'r' && temp[2] == 'l') {
        return 1;
    } else {
        return 0;
    }
}

int inList(List l, char *word) {
    if (l->head == NULL) {
        return 0;
    }
    Node curr = l->head;
    while (curr != NULL) {
        if (strcmp(curr->url, word) == 0) {
            // found
            return 1;
        }
        curr = curr->next;
    }
    return 0;
}

void increaseMentions(List l, char *url) {
    Node curr = l->head;
    while (strcmp(curr->url, url) != 0 && curr != NULL) {
        curr = curr->next;
    }
    assert(curr != NULL);
    curr->nMentions++;
}

void orderList(List l) {
    Node curr = l->head;
    if (curr == NULL) {
        // List empty
        return;
    } else if (curr->next == NULL) {
        // Only one node
        return;
    }
    int i = 0;
    while (i == 0) {
        curr = l->head;
        Node next = curr->next;
        i = 1;
        while (next != NULL) {
            if (
                curr->nMentions < next->nMentions ||
                (curr->nMentions == next->nMentions && curr->rank < next->rank)
            ) {
                // Swap required
                swapValues(curr, next);
                i = 0;
            }
            curr = curr->next;
            next = next->next;
        }
    }
}

void swapValues(Node a, Node b) {
    char tempUrl[MAX_NAME];
    strcpy(tempUrl, a->url);
    double tempRank = a->rank;
    int tempNMentions = a->nMentions;
    
    strcpy(a->url, b->url);
    a->rank = b->rank;
    a->nMentions = b->nMentions;
    
    strcpy(b->url, tempUrl);
    b->rank = tempRank;
    b->nMentions = tempNMentions;
}

void printList(List l) {
    Node curr = l->head;
    while (curr != NULL) {
        printf("%s\n", curr->url);
        curr = curr->next;
    }
}







