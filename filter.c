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

struct Node *head; /* global variable - pointer to head node.*/

/*Creates a new Node and returns pointer to it.*/
struct Node *GetNewNode(Person person) {
    struct Node *newNode
            = (struct Node *) malloc(sizeof(struct Node));
    newNode->personData = person;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}


void InsertAtTail(Person p) {
    struct Node *temp = head;
    struct Node *newNode = GetNewNode(p);
    if (head == NULL) {
        head = newNode;
        return;
    }
    while (temp->next != NULL) temp = temp->next; /* Go To last Node*/
    temp->next = newNode;
    newNode->prev = temp;
}

int main(int argc, char *argv[]) {
    head = NULL;
    int fd, n;
    if ((fd = open(argv[2], O_RDWR, 0666)) < 0) {
        perror("Error opening input file");
        exit(-1);
    }
    Person p;
    while ((n = read(fd, &p, sizeof(p))) > 0) {
        if (n < 0 || n > sizeof(p)) {
            perror("Read error occured:");
            exit(-1);
        }
        InsertAtTail(p);
    }
    close(fd);
    int a = (int) argv[1][0];
    if (a > 97 && a < 122) {
        a = a - 32;
    }
    if (a < 65 || a > 90) {
        write(2, "Input Error", 11);
        exit(-1);
    }
    int fp;
    if ((fp = open(argv[3], O_TRUNC | O_CREAT | O_RDWR, 0666)) < 0) {
        perror("Error opening/creating output file 1");
        exit(-1);
    }
    while (head != NULL) {
        p = head->personData;
        if ((int) p.id_ctrl == a) {
            if (write(fp, &p, sizeof(p)) < 0) {
                perror("write error file 1");
                exit(-1);
            }
        }
        head = head->next;
    }
    exit(0);
}