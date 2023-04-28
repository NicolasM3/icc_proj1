#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct User{
    int id;
    char name[100];
    int age;
    float amount;
};

struct UserArray {
    struct User *users;
    int size;
};

void writeNewLine(char filename[], char line[]) {
    FILE *file = fopen(filename, "a");

    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    fprintf(file, "\n%s", line);

    fclose(file);
}

struct UserArray readUsersFromFile(char filename[]) {
    FILE *file = fopen(filename, "r");
    int size = 0;
    struct User *users = NULL;

    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
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

struct User readUser(char filename[], int id) {
    FILE *file = fopen(filename, "r");
    struct User user;

    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return user;
    }

    while (!feof(file)) {
        fscanf(file,"%d %s %d %f", &user.id, user.name, &user.age, &user.amount);

        if (user.id == id) {
            fclose(file);
            return user;
        }
    }

    fclose(file);
    return user;
}

// Esse metodo esta duplicando a ultima linha do arquivo
void removeUser(char filename[], int id) {
    FILE *file = fopen(filename, "r");
    FILE *tempFile = fopen("temp.txt", "w");
    struct User user;

    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
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

    remove(filename);
    rename("temp.txt", filename);
}

int main() {
    char filename[] = "File1.txt";

    printf("ESCREVENDO\n");

    writeNewLine(filename, "1 Nicolas 20 1000.0");
    writeNewLine(filename, "2 Romanhole 20 1200.0");
    writeNewLine(filename, "3 Prato 21 2200.0");
    writeNewLine(filename, "4 Jaime 25 3200.0");
    
    printf("LENDO TODOS OS USUARIOS\n");

    struct UserArray usersResponse = readUsersFromFile(filename);
    struct User *users = usersResponse.users;

    for (int i = 0; i < usersResponse.size - 1; i++) {
        printf("%d %s %d %f\n", users[i].id, users[i].name, users[i].age, users[i].amount);
    }

    printf("LENDO UM USUARIO\n");

    struct User user = readUser(filename, 2);

    printf("%d %s %d %f\n", user.id, user.name, user.age, user.amount);

    printf("REMOVENDO UM USUARIO\n");

    removeUser(filename, 2);

    usersResponse = readUsersFromFile(filename);
    users = usersResponse.users;

    for (int i = 0; i < usersResponse.size - 1; i++) {
        printf("%d %s %d %f\n", users[i].id, users[i].name, users[i].age, users[i].amount);
    }
    
    free(users);

    return 0;
}