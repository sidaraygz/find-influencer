#include <stdio.h>
#include <stdlib.h>
#include <string.h>



typedef struct Person{
    int vertex;
    int counter;
    char fname[20];
    char lname[20];
    int indegree;
    struct Person *next;
    int *friends;
    int connections;
}Person;

void deleteNode(Person **head_ref, int key)
{
    Person *temp = *head_ref, *prev;

    if (temp != NULL && temp->vertex == key) {
        *head_ref = temp->next;
        free(temp);
        return;
    }

    while (temp != NULL && temp->vertex != key) {
        prev = temp;
        temp = temp->next;
    }


    if (temp == NULL)
        return;

    prev->next = temp->next;
}



void addPerson(Person **head, char fname[20], char lname[20], int v){
    Person *newPerson;
    Person *tmp = *head;

    newPerson = malloc(sizeof(Person));
    strcpy(newPerson->fname, fname);
    strcpy(newPerson->lname, lname);
    newPerson->counter = 0;
    newPerson->connections = 0;
    newPerson->indegree = 0;
    newPerson->vertex = v;
    newPerson->next = NULL;
    newPerson->friends = malloc(sizeof(Person) * 20);
    if (*head == NULL){
        *head = newPerson;
    }
    else{
        while (tmp->next != NULL)
            tmp = tmp->next;

        tmp->next = newPerson;
    }
}

int addFriend(Person **hdr, int v, int fv){
    Person *current = *hdr;
    while(current != NULL && current->vertex != v){
        current = current->next;
    }

    current->friends[current->counter] = fv;
    current->counter++;
    return 1;
}

void inDegree(Person **header){
    Person *iter = *header;
    int i;
    while(iter != NULL){
        Person *check = *header;
        while(check != NULL){
            i = 0;
            int checkBit = 0;
            while (checkBit == 0 && i < iter->counter){
                if(iter->friends[i] == check->vertex){
                    check->indegree++;
                    checkBit = 1;
                }
                i++;
            }
            check = check->next;
        }
        iter = iter->next;
    }
}

void ele(Person **header, int M){
    Person *iter = *header;
    int i;
    while(iter != NULL){
        Person *check = *header;
        if((iter->indegree) < M){
            for(i = 0; i<(iter->counter); i++){
                int flag = 0;
                while (check != NULL && flag == 0){
                    if(iter->friends[i] == check->vertex){
                        iter->friends[i] = 0;
                        check->indegree--;
                        flag = 1;
                    }
                    check = check->next;
                }
            }
            Person *prev = iter;
            iter = iter->next;
            deleteNode(header, prev->vertex);
            ele(header, M);
        }
        else
            iter = iter->next;

    }
}
int isPath(int start, int end, Person **header, int **visited){
    Person *iter = *header;
    Person *p1 = NULL;
    Person *p2 = NULL;
    int flag = 0;
    int i;
    if (start == end)
        return 1;

    while(iter != NULL && flag == 0){
        if(iter->vertex == start){
            p1 = iter;
        }
        else if(iter->vertex == end){
            p2 = iter;
        }
        iter = iter->next;
        if(p1 != NULL && p2 != NULL)
            flag = 1;
    }
    if(p1 == NULL || p2 == NULL){
        return 0;
    }
    ((*visited)[start]) = 1;
    for (i = 0; i<p1->counter; i++){
        if(((*visited)[p1->friends[i]]) == 0){
            if(isPath(p1->friends[i], end, header, visited) == 1)
                return 1;
        }
    }
    return 0;
}


void findConnectted(Person **header, int personLen){
    Person *iter = *header;
    int i;

    while(iter != NULL) {

        Person *check = *header;
        while (check != NULL) {
                if(check->vertex != iter->vertex){
                    int *visited;
                    visited = (int *) malloc(sizeof(int) * personLen);
                    for (i = 0; i < personLen; ++i) {
                        visited[i] = 0;
                    }
                    if (isPath(check->vertex, iter->vertex, header, &visited) == 1) {
                        iter->connections++;
                    }
                }
            check = check->next;
        }

        iter = iter->next;

    }

}


int main() {
    FILE *fp;
    Person *header = NULL;
    char name[40];
    char fname[20];
    char lname[20];
    char *pt;
    int node = 0;
    int personLen = 0;
    int M, X, Y, mod;
    printf("M, X ve Y degerlerini sirasiyla giriniz: ");
    scanf("%d %d %d",&M, &X, &Y);
    printf("Mod secimi (Normal 0, Detayl? 1): ");
    scanf("%d", &mod);
    fp = fopen("socialNET.txt", "r");

    while (!feof(fp)) {
        fscanf(fp, "%s", name);
        char ch = name[strlen(name) - 1];
        if(ch != ','){
            pt = strtok (name,",");
            node = atoi(pt);
            pt = strtok (NULL, ",");
            strcpy(fname, pt);
            pt = strtok (NULL, ",");
            strcpy(lname, pt);
            addPerson(&header, fname, lname, node);
            personLen++;
        }
        else{
            pt = strtok (name,",");
            while (pt != NULL) {
                int a = atoi(pt);
                addFriend(&header, node, a);
                pt = strtok (NULL, ",");
                }
        }
    }


    Person *iter = header;
    inDegree(&header);
    printf("%-9s\n", "");
    if(mod == 0){
        ele(&header, M);
        findConnectted(&header, personLen);

        printf("Influencerlar: \n");
        while(iter != NULL){
            if(iter->indegree>= X && iter->connections >= Y)
                printf("Ad Soyad: %s %s\n", iter->fname, iter->lname);

            iter = iter->next;
        }
    }
    else if(mod == 1) {
        printf("---------------Elemeden Once Degerler---------------");
        while (iter != NULL){
            printf("\n                Node: %d, indegree: %d", iter->vertex, iter->indegree);
            iter = iter->next;
        }
        printf("\n\n---------------Elemeden Sonra Degerler---------------");
        ele(&header, M);
        iter = header;
        while(iter != NULL){
            printf("\n       Node: %d, %s %s, indegree: %d",iter->vertex, iter->fname, iter->lname, iter->indegree);
            iter = iter->next;
        }
        findConnectted(&header, personLen);
        iter = header;
        printf("\n\n-------------------------------Influencerlar-------------------------------");
        while(iter != NULL){
            if(iter->indegree>= X && iter->connections >= Y)
                printf("\n      Node: %d, %s %s, indegree: %d, baglanti sayisi: %d",iter->vertex, iter->fname, iter->lname, iter->indegree, iter->connections);

            iter = iter->next;
        }

    }

    return 0;
}
