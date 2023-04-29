#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "user.h"

#define FILENAME "File1.txt"

int isFileEmpty() {
    FILE *file = fopen(FILENAME, "a");

    fseek (file, 0, SEEK_END);
    int ret = ftell(file) == 0 ? 1 : 0;

    fclose(file);
    return ret;
}

void writeNewLine(char line[]) {
    int shouldBreakLine = !isFileEmpty();

    FILE *file = fopen(FILENAME, "a");

    if (file == NULL) {
        printf("Error opening file: %s\n", FILENAME);
        return;
    }

    if (shouldBreakLine) {
        fprintf(file, "\n%s", line);
    } else {
        fprintf(file, "%s", line);
    }

    fclose(file);
}

struct UserArray readUsersFromFile() {
    FILE *file = fopen(FILENAME, "r");
    int size = 0;
    struct User *users = NULL;

    if (file == NULL) {
        printf("Error opening file: %s\n", FILENAME);
        return (struct UserArray){NULL, 0};
    }

    while (!feof(file)) {
        struct User temp;

        fscanf(file,"%d %s %d %f", &temp.id, temp.name, &temp.age, &temp.amount);

        users = (struct User *)realloc(users, sizeof(struct User) * (size + 1));
        users[size] = temp;
        size++;
    }

    fclose(file);
    return (struct UserArray){users, size};
}

struct User* readUser(int id) {
    FILE *file = fopen(FILENAME, "r");
    struct User* user = NULL;

    if (file == NULL) {
        printf("Error opening file: %s\n", FILENAME);
        return NULL;
    }

    while (!feof(file)) {
        struct User temp;

        fscanf(file,"%d %s %d %f", &temp.id, temp.name, &temp.age, &temp.amount);

        if (temp.id == id) {
            user = (struct User *)malloc(sizeof(struct User));
            user[0] = temp;

            fclose(file);
            return user;
        }
    }

    fclose(file);
    return NULL;
}

void removeUser(int id) {
    FILE *file = fopen(FILENAME, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    struct User user;

    if (file == NULL) {
        printf("Error opening file: %s\n", FILENAME);
        return;
    }

    while (!feof(file)) {
        fscanf(file,"%d %s %d %f", &user.id, user.name, &user.age, &user.amount);

        if (user.id == id) {
            continue;
        }
        fprintf(tempFile, "%d %s %d %f\n", user.id, user.name, user.age, user.amount);
    }

    fclose(file);
    fclose(tempFile);

    remove(FILENAME);
    rename("temp.txt", FILENAME);
}

void executeTransfer(int senderId, int receiverId, float amount) {
    struct User* sender = readUser(senderId);
    struct User* receiver = readUser(receiverId);

    if (sender == NULL) {
        printf("Sender not found. ID: %d\n", senderId);
        return;
    }

    if (receiver == NULL) {
        printf("Receiver not found. ID: %d\n", receiverId);
        return;
    }

    if (sender->amount - amount < 0) {
        printf("Insufficient funds. Sender amount: %f\n", sender->amount);
        return;
    }

    sender->amount = sender->amount - amount;
    receiver->amount = receiver->amount + amount;
}

int main() {
    printf("ESCREVENDO\n");

    writeNewLine("1 Nicolas 20 1000.0");
    writeNewLine("2 Romanhole 20 1200.0");
    writeNewLine("3 Prato 21 2200.0");
    writeNewLine("4 Jaime 25 3200.0");
    
    printf("LENDO TODOS OS USUARIOS\n");

    struct UserArray usersResponse = readUsersFromFile();
    struct User *users = usersResponse.users;

    for (int i = 0; i <= usersResponse.size - 1; i++) {
        printUser(users[i]);
        printf("\n");
    }

    printf("LENDO UM USUARIO\n");

    struct User* user = readUser(2);

    printUserFromPointer(user);
    printf("\n");

    printf("REMOVENDO UM USUARIO (id: %d - %s)\n", user->id, user->name);

    removeUser(2);

    usersResponse = readUsersFromFile();
    users = usersResponse.users;

    for (int i = 0; i < usersResponse.size - 1; i++) {
        printUser(users[i]);
        printf("\n");
    }

    user = readUser(0);

    printf("FAZENDO TRANSFERENCIA\n");
    executeTransfer(1, 3, 10);

    free(user);
    free(users);

    return 0;
}