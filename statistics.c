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

//Creates a new Node and returns pointer to it.
struct Node *GetNewNode(Person person) {
    struct Node *newNode
            = (struct Node *) malloc(sizeof(struct Node));
    newNode->personData = person;
    newNode->prev = NULL;
    newNode->next = NULL;
    return newNode;
}

/*Inserts a Node at head of doubly linked list*/
void InsertAtHead(Person p) {
    struct Node *newNode = GetNewNode(p);
    if (head == NULL) {
        head = newNode;
        return;
    } else {
        head->prev = newNode;
        newNode->next = head;
        head = newNode;
    }
}

int main(int argc, char *argv[]) {

    head = NULL;
    int fd, n;
    if ((fd = open(argv[1], O_RDWR, 0666)) < 0) {
        perror("Error opening file");
        exit(-1);
    }
    Person p;
    while ((n = read(fd, &p, sizeof(p))) > 0) {
        if (n < 0 || n > sizeof(p)) {
            perror("Read error occured:");
            exit(-1);
        }
        InsertAtHead(p);
    }
    close(fd);
    int medianAge = 0, partialSumMA = 0, medianAgeCC = 0, partialSumAgeCC = 0, i, max = 0;
    double partialSumMI = 0, medianIncome = 0;
    char modalCC[26] = {0}, modalCCF = 'A';
    struct Node *head2 = head;

    for (i = 0; head != NULL; i++) {
        partialSumMI += head->personData.salary;
        partialSumMA += head->personData.age;
        modalCC[((int) head->personData.id_ctrl) - 65]++;
        head = head->next;
    }
    head = head2;
    medianAge = partialSumMA / i;
    medianIncome = partialSumMI / i;
    for (int j = 0; j < 26; j++) {
        if (modalCC[j] > max) {
            max = modalCC[j];
        }
    }
    for (int j = 0; j < 26; j++) {
        if (max == modalCC[j]) {
            modalCCF = (char) (j + 65);
        }
    }
    while (head != NULL) {
        if (head->personData.id_ctrl == modalCCF) {
            partialSumAgeCC = partialSumAgeCC + ((int) head->personData.age);

        }
        head = head->next;
    }
    medianAgeCC = partialSumAgeCC / max;
    int realPart = (int) medianIncome;
    double decimal = medianIncome - realPart;
    if (decimal >= 0.5) {
        decimal = medianIncome+1;
    } else {
        decimal = medianIncome;
    }
    printf("Renta Media: %i\nEdad Media: %i\nCaracter de control de DNI mas frecuente: %c\nEdad media para el caracter de control de DNI mas frecuente: %i",
           (int) decimal, medianAge, modalCCF, medianAgeCC);

    exit(0);
}