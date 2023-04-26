#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct User{
    int id;
    char name[100];
    float amount;
};

void writeNewLine(char filename[], char line[]) {
    FILE *file = fopen(filename, "a");

    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    fprintf(file, "%s\n", line);

    fclose(file);
}

struct User * readUsersFromFile(char filename[]) {
    FILE *file = fopen(filename, "r");
    int size = 0;
    struct User *users = NULL;

    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return 0;
    }

    while (!feof(file)) {
        struct User temp;

        fscanf(file,"%d %s %f", &temp.id, temp.name, &temp.amount);

        users = (struct User *)realloc(users, sizeof(struct User) * (size + 1));
        users[size] = temp;
        size++;
    }

    fclose(file);
    return users;
}

int main() {
    char filename[] = "File1.txt";

    writeNewLine(filename, "1 Nicolas 1000.0");
    writeNewLine(filename, "2 Romanhole 1200.0");
    writeNewLine(filename, "3 Prato 2200.0");

    struct User *users = readUsersFromFile("File1.txt");

    for (int i = 0; i < 3; i++) {
        printf("%d %s %f\n", users[i].id, users[i].name, users[i].amount);
    }
}