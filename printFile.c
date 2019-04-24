#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <math.h>
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
    if ((fd = open(argv[1], O_RDWR, 0666)) < 0) {
        printf("%i", fd);
        perror("Error opening file");
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
    int realPart;
    double decimal;
    while (head != NULL) {
        char toPrint[100] = {0};
        realPart = (int) head->personData.salary;
        decimal = head->personData.salary - realPart;
        if (decimal >= 0.5) {
            decimal = realPart + 1;
        } else {
            decimal = realPart;
        }
        sprintf(toPrint, "%s\t%i\t%i\t%c\t%i\n", head->personData.name, head->personData.age, head->personData.id,
                head->personData.id_ctrl, (int) decimal);
        head = head->next;
        write(STDOUT_FILENO, toPrint, sizeof(toPrint));
    }
    exit(0);
}
