#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Usuario{
    int id;
    char nome[100];
    float saldo;
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

struct Usuario * readUsuariosFromFile(char filename[]) { 
    FILE *file = fopen(filename, "r");
    int tamanho = 0;
    struct Usuario *usuarios = NULL;

    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return 0;
    }

    while (!feof(file)) {
        struct Usuario temp;

        fscanf(file,"%d %s %f", &temp.id, temp.nome, &temp.saldo);

        usuarios = (struct Usuario *)realloc(usuarios, sizeof(struct Usuario) * (tamanho + 1));
        usuarios[tamanho] = temp;
        tamanho++;
    }

    fclose(file);
    return usuarios;
}

int main() {
    char filename[] = "File1.txt";

    writeNewLine(filename, "1 Nicolas 1000.0");
    writeNewLine(filename, "2 Romanhole 1200.0");
    writeNewLine(filename, "3 Prato 2200.0");

    struct Usuario *usuarios = readUsuariosFromFile("File1.txt");

    for (int i = 0; i < 3; i++) {
        printf("%d %s %f\n", usuarios[i].id, usuarios[i].nome, usuarios[i].saldo);
    }
}