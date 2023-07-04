#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "user.c"

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

        fscanf(file,"%d", &temp.id);
        fgetc(file);
        fscanf(file, "%99[^;]", temp.name);
        fgetc(file);
        fscanf(file, "%d;%f", &temp.age, &temp.amount);

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

    while (1) {
        struct User temp;

        if (fscanf(file, "%d;%99[^;];%d;%f", &temp.id, temp.name, &temp.age, &temp.amount) != 4)
            break;

        if (temp.id == id) {
            user = (struct User*)malloc(sizeof(struct User));
            *user = temp;

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
    int firstTime = 0;

    if (file == NULL) {
        printf("Error opening file: %s\n", FILENAME);
        return;
    }

    while (fscanf(file, "%d;%99[^;];%d;%f", &user.id, user.name, &user.age, &user.amount) == 4) {
        if(firstTime == 0)
            firstTime = 1;
        else
            fprintf(tempFile, "\n");
        if (user.id == id) {
            continue;
        }

        fprintf(tempFile, "%d;%s;%d;%.2f", user.id, user.name, user.age, user.amount);
    }

    fclose(file);
    fclose(tempFile);

    remove(FILENAME);
    rename("temp.txt", FILENAME);
}

void executeTransfer(int senderId, int receiverId, float amount) {
    FILE *file = fopen(FILENAME, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    struct User* user = (struct User *)malloc(sizeof(struct User));
    struct User* sender = readUser(senderId);
    struct User* receiver = readUser(receiverId);
    int firstTime = 0;

    if (file == NULL) {
        printf("Error opening file: %s\n", FILENAME);
        return;
    }

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

     while (fscanf(file, "%d;%99[^;];%d;%f", &user->id, user->name, &user->age, &user->amount) == 4) {
        if(firstTime == 0)
            firstTime = 1;
        else
            fprintf(tempFile, "\n");
        if (user->id == receiver->id) {
            fprintf(tempFile, "%d;%s;%d;%.2f", receiver->id, receiver->name, receiver->age, receiver->amount);
        }
        else if (user->id == sender->id) {
            fprintf(tempFile, "%d;%s;%d;%.2f", sender->id, sender->name, sender->age, sender->amount);
        }
        else {
            fprintf(tempFile, "%d;%s;%d;%.2f", user->id, user->name, user->age, user->amount);
        }
    }

    fclose(file);
    fclose(tempFile);

    remove(FILENAME);
    rename("temp.txt", FILENAME);
}

void printMenu() {
    printf("MENU\n");
    printf("1. Adicionar usuario\n");
    printf("2. Adicionar multiplos usuarios\n");
    printf("3. Listar usuarios\n");
    printf("4. Fazer transferencia entre usuarios\n");
    printf("5. Excluir usuario\n");
    printf("6. Fechar programa\n");
}

int main() {
    int option = 0;

    do {
        system("clear");
        printMenu();
        scanf("%d", &option);

        switch (option) {
            case 1:
                system("clear");

                struct User user;
                printf("Digite o nome do usuario\n");
                scanf("%s", &user.name);
                printf("Digite a idade do usuario\n");
                scanf("%d", &user.age);
                printf("Digite o saldo do usuario\n");
                scanf("%d", &user.amount);

                printf("Usuario adicionado!\n");
            break;
            case 2:
            case 3:
            case 4:
            case 5:
            case 6:
            default:;
                option = 1;
            break;
        }
    }  while (option != 0);
/*
    printf("ESCREVENDO\n");

    writeNewLine("1;Nicolas;20;1000.0");
    writeNewLine("2;Romanhole;20;1200.0");
    writeNewLine("3;Prato;21;2200.0");
    writeNewLine("4;Jaime;25;3200.0");

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

    for (int i = 0; i < usersResponse.size; i++) {
        printUser(users[i]);
        printf("\n");
    }

    printf("FAZENDO TRANSFERENCIA\n");
    executeTransfer(3, 4, 1000.40);

    usersResponse = readUsersFromFile();
    users = usersResponse.users;

    for (int i = 0; i < usersResponse.size; i++) {
        printUser(users[i]);
        printf("\n");
    }

    free(user);
    free(users);
*/
    return 0;
}
