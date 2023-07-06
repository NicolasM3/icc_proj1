#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <ctype.h>

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

void waitForEnter() {
    printf("Precione ENTER para voltar para o menu\n");
    fflush(stdin);
    getchar();
}

int getRandomId() {
    srand(time(NULL));
    return rand() % 1000;
}

int getUserName(char* username) {
    char c;
    while ((c = getchar()) != '\n' && c != EOF) { }
    fgets(username, 1000, stdin);

    username[strcspn(username, "\n")] = '\0';

    for (int i = 0; username[i] != '\0'; i++) {
        if (isnumber(username[i]) ) {
            return 0;
        }
    }

    return 1;
}

int assertInputs(int result) {
    if (result != 1) {
        return 0;
    }
    return 1;
}

int main() {
    int option = 0;
    int errorHandler = 0;
    struct UserArray usersResponse = readUsersFromFile();
    struct User *users = usersResponse.users;
    struct User user;

    do {
        system("clear");
        printMenu();
        scanf("%d", &option);

        switch (option) {
            case 1:
                system("clear");
                printf("Digite o nome do usuario\n");
                errorHandler = getUserName(user.name);

                if (strlen(user.name) > 100) {
                    printf("Nome muito grande, abortando.\n");
                    return 0;
                }
                if (assertInputs(errorHandler) != 1) {
                    printf("Nome inválido, abortando.\n");
                    return 0;
                }
                
                printf("Digite a idade do usuario\n");
                errorHandler = scanf("%d", &user.age);
                if (assertInputs(errorHandler) != 1) {
                    printf("Input inválido, abortando.\n");
                    return 0;
                }
                printf("Digite o saldo do usuario\n");
                errorHandler = scanf("%f", &user.amount);
                if (assertInputs(errorHandler) != 1) {
                    printf("Input inválido, abortando.\n");
                    return 0;
                }

                user.id = getRandomId();

                char line[1000] = "";
                char temp[100] = "";

                sprintf(temp, "%d", user.id);
                strcat(line, temp);
                strcat(line, ";");
                strcat(line, user.name);
                strcat(line, ";");
                sprintf(temp, "%d", user.age);
                strcat(line, temp);
                strcat(line, ";");
                sprintf(temp, "%.2f", user.amount);
                strcat(line, temp);

                writeNewLine(line);
                printf("Usuario adicionado!\n");
                printUser(user);
                printf("\n");
                waitForEnter();
            break;
            case 2:
                system("clear");

                int count = 0;

                printf("Digite quantos usuarios voce vai adicionar\n");
                scanf("%d", &count);

                for(int i = 0; i < count; i++) {
                    printf("Digite o nome do usuario\n");
                    errorHandler = getUserName(user.name);

                    if (strlen(user.name) > 100) {
                        printf("Nome muito grande, abortando.\n");
                        return 0;
                    }
                    if (assertInputs(errorHandler) != 1) {
                        printf("Nome inválido, abortando.\n");
                        return 0;
                    }
                    
                    printf("Digite a idade do usuario\n");
                    errorHandler = scanf("%d", &user.age);
                    if (assertInputs(errorHandler) != 1) {
                        printf("Input inválido, abortando.\n");
                        return 0;
                    }

                    printf("Digite o saldo do usuario\n");
                    errorHandler = scanf("%f", &user.amount);
                    if (assertInputs(errorHandler) != 1) {
                        printf("Input inválido, abortando.\n");
                        return 0;
                    }

                    user.id = getRandomId();

                    char line[1000] = "";
                    char temp[100] = "";

                    sprintf(temp, "%d", user.id);
                    strcat(line, temp);
                    strcat(line, ";");
                    strcat(line, user.name);
                    strcat(line, ";");
                    sprintf(temp, "%d", user.age);
                    strcat(line, temp);
                    strcat(line, ";");
                    sprintf(temp, "%.2f", user.amount);
                    strcat(line, temp);

                    writeNewLine(line);
                    printf("Usuario adicionado!\n");
                    printUser(user);
                    printf("\n");
                }

                printf("Usuarios adicionados!\n");
                waitForEnter();
            case 3:
                system("clear");

                usersResponse = readUsersFromFile();
                users = usersResponse.users;

                for (int i = 0; i <= usersResponse.size - 1; i++) {
                    printUser(users[i]);
                    printf("\n");
                }

                waitForEnter();
            break;
            case 4:
                system("clear");

                int idSender, idReceiver;
                float amount;

                printf("Digite o id do usuario que vai transferir\n");
                scanf("%d", &idSender);

                printf("Digite o valor da transferencia\n");
                scanf("%f", &amount);

                printf("Digite o id do usuario que vai receber\n");
                scanf("%d", &idReceiver);

                executeTransfer(idSender, idReceiver, amount);

                printUserFromPointer(readUser(idReceiver));
                printf("Digite o id do usuario que vai receber\n");
                printUserFromPointer(readUser(idSender));

                waitForEnter();
            break;
            case 5:
                system("clear");
                int id;
                usersResponse = readUsersFromFile();
                users = usersResponse.users;

                for (int i = 0; i <= usersResponse.size -1; i++) {
                    printUser(users[i]);
                    printf("\n");
                }

                printf("\n");
                printf("Digite o id do usuario que deseja excluir: \n");
                scanf("%d",&id);

                struct User* user = readUser(id);
                removeUser(id);

                system("clear");
                usersResponse = readUsersFromFile();
                users = usersResponse.users;

                for (int i = 0; i < usersResponse.size - 1; i++) {
                    printUser(users[i]);
                    printf("\n");
                }

                printf("\n");
                printf("Usuario deletado: \n");
                printUserFromPointer(user);
                printf("\n");

                waitForEnter();
            break;
            case 6:
                return 0;
            default:
                option = 1;
            break;
        }
    }  while (option != 0);

    return 0;
}
