
/**
 * AUTHOR: Lucas Viana Vilela
 * NO.USP: 10748409
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../headers/file-handling.h"
#include "../headers/util.h"

// reads a CSV file and returns the content as a string
char *fread_csv(char *filename){
    // string that has the .csv filepath (inside the "data" directory)
    char *filepath = get_filepath(filename, 'c');

    // opens the file in reading mode
    FILE *fp = fopen(filepath, "r");

    // if the files does not exist, raises error and exists program
    if(!fp){ raise_error(""); }

    // string in which all of the file's content will be stored
    // it's initial value is 0 bytes because each char will be read
    // individually
    int contentSize = 0;
    char *content = (char *)malloc(0);

    // temporary/auxiliar variable to read individual chars
    char tmp;

    // loop to read each of the file's characters
    while(fread(&tmp, sizeof(char), 1, fp)){
        content = (char *)realloc(content, ++contentSize * sizeof(char));
        content[contentSize-1] = tmp;
    }

    // terminates the string
    content = (char *)realloc(content, ++contentSize * sizeof(char));
    content[contentSize-1] = '\0';

    // closes file and frees allocated data
    fclose(fp);
    free(filepath);

    return content;
}

// opens a binary file, checks if it is
// consistent and returns a pointer to it
FILE *fopen_validate_binary(char *filename,  char *mode){
    // if it's not a binary file (binary mode), raise error
    if(mode[strlen(mode)-1] != 'b'){
        raise_error("The function open_validate_binary() must be used only for binary files.");
    }

    // string that has the .bin filepath (inside the "binaries" directory)
    char *filepath = get_filepath(filename, 'b');

    // opens the file in reading mode
    FILE *file = fopen(filepath, mode);

    // if the file doesn't exist, raises error and exists program
    if (!file) { raise_error(""); }

    // if it's not on writing-mode, reads the file's header status
    // and if it's inconsistent, raises error and exists program
    if(mode[0] != 'w') {
        char status;
        fread(&status, sizeof(char), 1, file);
        if (status != '1') { raise_error(""); }
    }

    free(filepath);

    return file;
}

// reads all data from the
// vehicle's header but the status
vehicle_header fread_vehicle_header(FILE *file){
    vehicle_header header;

    fread(&header.byteProxReg,      sizeof(long long), 1,  file);
    fread(&header.nroRegistros,     sizeof(int),       1,  file);
    fread(&header.nroRegRemovidos,  sizeof(int),       1,  file);
    fread(header.descrevePrefixo,   sizeof(char),      18, file);
    fread(header.descreveData,      sizeof(char),      35, file);
    fread(header.descreveLugares,   sizeof(char),      42, file);
    fread(header.descreveLinha,     sizeof(char),      26, file);
    fread(header.descreveModelo,    sizeof(char),      17, file);
    fread(header.descreveCategoria, sizeof(char),      20, file);

    return header;
}

// reads all data from the
// line's header but the status
line_header fread_line_header(FILE *file){
    line_header header;

    fread(&header.byteProxReg,     sizeof(long long), 1,  file);
    fread(&header.nroRegistros,    sizeof(int),       1,  file);
    fread(&header.nroRegRemovidos, sizeof(int),       1,  file);
    fread(header.descreveCodigo,   sizeof(char),      15, file);
    fread(header.descreveCartao,   sizeof(char),      13, file);
    fread(header.descreveNome,     sizeof(char),      13, file);
    fread(header.descreveCor,      sizeof(char),      24, file);

    return header;
}

// reads a vehicle data register from
// the received file and returns it
vehicle_register fread_vehicle_register(FILE *file){
    vehicle_register data;

    // reads the current register's "removido" and "tamanhoRegistro" fields
    fread(&data.removido, sizeof(char), 1, file);
    fread(&data.tamanhoRegistro, sizeof(int), 1, file);

    // if the current register wasn't logically removed
    if(data.removido != '0'){
        // reads the current register's remaining fixed size fields
        fread(data.prefixo, sizeof(char), 5, file);
        fread(data.data, sizeof(char), 10, file);
        fread(&data.quantidadeLugares, sizeof(int), 1, file);
        fread(&data.codLinha, sizeof(int), 1, file);

        // reads the current register's "modelo" field (variable size)
        fread(&data.tamanhoModelo, sizeof(int), 1, file);
        data.modelo = (char *)malloc(sizeof(char) * data.tamanhoModelo);
        fread(data.modelo, sizeof(char), data.tamanhoModelo, file);

        // reads the current register's "categoria" field (variable size)
        fread(&data.tamanhoCategoria, sizeof(int), 1, file);
        data.categoria = (char *)malloc(sizeof(char) * data.tamanhoCategoria);
        fread(data.categoria, sizeof(char), data.tamanhoCategoria, file);
    }

    return data;
}

// reads a line data register from
// the received file and returns it
line_register fread_line_register(FILE *file){
    line_register data;

    // reads the current register's "removido" and "tamanhoRegistro" fields
    fread(&data.removido, sizeof(char), 1, file);
    fread(&data.tamanhoRegistro, sizeof(int), 1, file);

    // if the current register wasn't logically removed
    if(data.removido != '0'){
        // reads the current register's remaining fixed size fields
        fread(&data.codLinha, sizeof(int), 1, file);
        fread(&data.aceitaCartao, sizeof(char), 1, file);

        // reads the current register's "nomeLinha" field (variable size)
        fread(&data.tamanhoNome, sizeof(int), 1, file);
        data.nomeLinha = (char *)malloc(sizeof(char) * data.tamanhoNome);
        fread(data.nomeLinha, sizeof(char), data.tamanhoNome, file);

        // reads the current register's "corLinha" field (variable size)
        fread(&data.tamanhoCor, sizeof(int), 1, file);
        data.corLinha = (char *)malloc(sizeof(char) * data.tamanhoCor);
        fread(data.corLinha, sizeof(char), data.tamanhoCor, file);
    }

    return data;
}

// writes the passed consistency
// to the passed file's header
// '0' means inconsistent
// '1' means consistent
void set_consistency(char consistency, FILE *file){
    if(consistency != '0' && consistency != '1'){
        raise_error("Valor inválido de consistência de arquivo - deve ser '1' ou '0'.");
    }

    // goes to start of file
    fseek(file, 0, SEEK_SET);

    // and writes the value
    fwrite(&consistency, sizeof(char), 1, file);

    return;
}

// creates a binary file with the received filename
FILE *fcreate_binary(char *filename){
    // string that has the .bin filepath (inside the "binaries" directory)
    char *filepath = get_filepath(filename, 'b');

    // opens file in binary-writing mode
    FILE *file = fopen(filepath, "wb");

    // if the files could not be created, raises error and exists program
    if(!file){ raise_error(""); }

    free(filepath);

    return file;
}

// deletes a binary file with the received filename
void fremove_binary(char *filename){
    // string that has the .bin filepath (inside the "binaries" directory)
    char *filepath = get_filepath(filename, 'b');

    remove(filepath);
    free(filepath);

    return;
}

