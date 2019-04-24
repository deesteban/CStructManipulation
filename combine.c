#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include "persona.h"

struct Node {
    Person personData;
    struct Node *next;
    struct Node *prev;
};

struct Node *head1;
struct Node *head2;

/*Creates a new Node and returns pointer to it.*/
struct Node *GetNewNode(Person person) {
    struct Node *newNode
            = (struct Node *) malloc(sizeof(struct Node));
    newNode->personData = person;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

/*Inserts a Node at head of doubly linked list*/
void InsertAtHead1(Person p) {
    struct Node *newNode = GetNewNode(p);
    if (head1 == NULL) {
        head1 = newNode;
        return;
    } else {
        head1->prev = newNode;
        newNode->next = head1;
        head1 = newNode;
    }
}

void InsertAtHead2(Person p) {
    struct Node *newNode = GetNewNode(p);
    if (head2 == NULL) {
        head2 = newNode;
        return;
    } else {
        head2->prev = newNode;
        newNode->next = head2;
        head2 = newNode;
    }
}


int main(int argc, char *argv[]) {
    head1 = NULL;
    head2 = NULL;
    int fd, n;
    if ((fd = open(argv[1], O_RDWR, 0666)) < 0) {
        perror("Error opening input file 1");
        exit(-1);
    }
    Person p, p2;
    while ((n = read(fd, &p, sizeof(p))) > 0) {
        if (n < 0 || n > sizeof(p)) {
            perror("Read error occured:");
            exit(-1);
        }
        InsertAtHead1(p);
    }

    if ((fd = open(argv[2], O_RDWR, 0666)) < 0) {
        perror("Error opening input file 2");
        exit(-1);
    }
    while ((n = read(fd, &p, sizeof(p))) > 0) {
        if (n < 0 || n > sizeof(p)) {
            perror("Read error occured:");
            exit(-1);
        }
        InsertAtHead2(p);
    }
    close(fd);
    int fp;
    if ((fp = open(argv[3], O_CREAT | O_RDWR | O_TRUNC, 0666)) < 0) {
        perror("Error opening/creating output file:");
        exit(-1);
    }

    while (head1 != NULL || head2 != NULL) {
        if (head1 != NULL) {
            p = head1->personData;
            if (write(fp, &p, sizeof(p)) < 0) {
                perror("write error file 1");
                exit(-1);
            }
            head1 = head1->next;
        }
        if (head2 != NULL) {
            p2 = head2->personData;
            if (write(fp, &p2, sizeof(p)) < 0) {
                perror("write error file 1");
                exit(-1);
            }
            head2 = head2->next;
        }
    }
    close(fp);
    exit(0);
}
