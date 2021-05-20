#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./vehicle-structs.c"

// receives a vehicle-csv string and parses it, 
// returning the pointer to a "vehicle" struct
vehicle *parse_vehicle_csv(char *content){
    // temporary/auxiliar variables to read strings
    char *tmp_row, *tmp_string;

    // header register
    vehicle_header *header = (vehicle_header *)malloc(sizeof(vehicle_header));

    // array of data registers
    vehicle_register *data = (vehicle_register *)malloc(0);
    int data_length = 0;

    // gets header row
    tmp_row = strsep(&content, "\n");

    // parses the header row's values
    header->status = '0';
    header->byteProxReg = VEHICLE_HEADER_LENGTH;
    header->nroRegRemovidos = 0;
    strcpy(header->descrevePrefixo, strsep(&tmp_row, ","));
    strcpy(header->descreveData, strsep(&tmp_row, ","));
    strcpy(header->descreveLugares, strsep(&tmp_row, ","));
    strcpy(header->descreveLinha, strsep(&tmp_row, ","));
    strcpy(header->descreveModelo, strsep(&tmp_row, ","));
    strcpy(header->descreveCategoria, strsep(&tmp_row, ","));

    // loops through every data row (register)
    // strcmp returns 0 if both are equal and != 0 otherwise
    while(strcmp(content, "")){
        // gets current row (register)
        tmp_row = strsep(&content, "\n");

        // allocates a new register to the array
        data = (vehicle_register *)realloc(data, ++data_length * sizeof(vehicle_register));

        // sets the "removido" field to 1 (meaning it was not removed)
        // if it was removed, it'll later be set to 0
        data[data_length-1].removido = '1';

        // if the current register was deletes
        if(tmp_row[0] == '*'){
            // increments the number of deleted register in the header
            (header->nroRegRemovidos)++;

            // sets the "removido" field to 0
            data[data_length-1].removido = '0';

            // ignores the prefix's first char (*) by incrementing the addres pointed to by tmp_row
            tmp_row++;
        }

        // parses the current register's data

        // reads the prefix with the auxiliar variable, then checks if it has less 
        // than 5 chars, and, if so, adds '\0' and '@' to the available space. finally,
        // copies the string in the auxiliar variable to the array
        tmp_string = strsep(&tmp_row, ",");
        if(strlen(tmp_string) < 5){ for(int i = 4; tmp_string[i] != '\0'; i--){ tmp_string[i] = '@'; } }
        strcpy(data[data_length-1].prefixo, tmp_string);

        // reads the date with the auxiliar variable, then checks if the passed 
        // date is null and correctly sets it's value from the auxiliar variable
        tmp_string = strsep(&tmp_row, ",");

        // if date is not null, copies the string from tmp_string
        if(strcmp(tmp_string, "NULO")){ strcpy(data[data_length-1].data, tmp_string); }

        // if the date is null, sets it to "\0@@@@@@@@@@"
        else{
            data[data_length-1].data[0] = '\0';
            memset(&(data[data_length-1].data[1]), '@', 9);
        }

        // same as above but for the number of seats
        tmp_string = strsep(&tmp_row, ",");
        data[data_length-1].quantidadeLugares = strcmp(tmp_string, "NULO") ? atoi(tmp_string) : -1;

        // same as above but for the line code
        tmp_string = strsep(&tmp_row, ",");
        data[data_length-1].codLinha = strcmp(tmp_string, "NULO") ? atoi(tmp_string) : -1;

        // same as above but for the model
        tmp_string = strsep(&tmp_row, ",");
        data[data_length-1].modelo = strcmp(tmp_string, "NULO") ? tmp_string : "";

        // same as above but for the category
        tmp_string = strsep(&tmp_row, ",");
        data[data_length-1].categoria = strcmp(tmp_string, "NULO") ? tmp_string : "";

        // sets the variable fields's sizes
        data[data_length-1].tamanhoModelo = strlen(data[data_length-1].modelo);
        data[data_length-1].tamanhoCategoria = strlen(data[data_length-1].categoria);

        // sets this register's size
        data[data_length-1].tamanhoRegistro = VECHILE_DATA_STATIC_LENGTH + data[data_length-1].tamanhoModelo + data[data_length-1].tamanhoCategoria;

        // sets the header's next free byte position
        // "removido" and "tamanhoRegistro" aren't considered to 
        // "tamanhoRegistro"'s value, so it's necessary to add 5 bytes
        header->byteProxReg += data[data_length-1].tamanhoRegistro + 5;
    }

    // sets the header's number of registers in the file
    header->nroRegistros = data_length - header->nroRegRemovidos;
    
    vehicle *parsed = (vehicle *)malloc(sizeof(vehicle));
    parsed->header = header;
    parsed->data = data;
    
    return parsed;
}

// receives a filename, parses the content and writes it to the file
void write_vehicle_bin(char *filename, char *content){
    // opens file in binary-writing mode
    FILE *binary = fopen(filename, "wb");
    
    // parses the content string
    vehicle *parsed = parse_vehicle_csv(content);

    // writes header to disk
    fwrite(&(parsed->header->status), sizeof(char), 1, binary);
    fwrite(&(parsed->header->byteProxReg), sizeof(long long), 1, binary);
    fwrite(&(parsed->header->nroRegistros), sizeof(int), 1, binary);
    fwrite(&(parsed->header->nroRegRemovidos), sizeof(int), 1, binary);
    fwrite(parsed->header->descrevePrefixo, sizeof(char), 18, binary);
    fwrite(parsed->header->descreveData, sizeof(char), 35, binary);
    fwrite(parsed->header->descreveLugares, sizeof(char), 42, binary);
    fwrite(parsed->header->descreveLinha, sizeof(char), 26, binary);
    fwrite(parsed->header->descreveModelo, sizeof(char), 17, binary);
    fwrite(parsed->header->descreveCategoria, sizeof(char), 20, binary);

    // writes through each data register and writes it to disk
    int data_length = parsed->header->nroRegistros + parsed->header->nroRegRemovidos;
    for(int i = 0; i < data_length; i++){
        fwrite(&(parsed->data[i].removido), sizeof(char), 1, binary);
        fwrite(&(parsed->data[i].tamanhoRegistro), sizeof(int), 1, binary);
        fwrite(parsed->data[i].prefixo, sizeof(char), 5, binary);
        fwrite(parsed->data[i].data, sizeof(char), 10, binary);
        fwrite(&(parsed->data[i].quantidadeLugares), sizeof(int), 1, binary);
        fwrite(&(parsed->data[i].codLinha), sizeof(int), 1, binary);
        fwrite(&(parsed->data[i].tamanhoModelo), sizeof(int), 1, binary);
        fwrite(parsed->data[i].modelo, sizeof(char), parsed->data[i].tamanhoModelo, binary);
        fwrite(&(parsed->data[i].tamanhoCategoria), sizeof(int), 1, binary);
        fwrite(parsed->data[i].categoria, sizeof(char), parsed->data[i].tamanhoCategoria, binary);
    }

    // sets the header "status" field to 1
    parsed->header->status = '1';
    fseek(binary, 0, SEEK_SET);
    fwrite(&(parsed->header->status), sizeof(char), 1, binary);
    
    // closes file and frees allocated data
    fclose(binary);
    free(parsed->header);
    free(parsed->data);
    free(parsed);

    return;
}
