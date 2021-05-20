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
        data[data_length-1].tamanhoRegistro = VEHICLE_DATA_STATIC_LENGTH + data[data_length-1].tamanhoModelo + data[data_length-1].tamanhoCategoria;

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
    char *basepath = "./binaries/";

    // string that has the .csv filepath (inside the "data" directory)
    char *filepath = (char *)malloc((strlen(basepath) + strlen(filename) + 1) * sizeof(char));

    // sets filepath's value
    strcpy(filepath, basepath);
    strcat(filepath, filename);

    // opens file in binary-writing mode
    FILE *binary = fopen(filepath, "wb");
    
    // if the files could not be created, raises error and exists program
    if(!binary){
        printf("Falha no processamento do arquivo.\n");
        exit(0);
    }

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
    free(filepath);

    return;
}

// reads from stdin "no_inputs" vehicle registers and parses it's fields
vehicle *read_vehicle_input(int no_inputs){
    char tmp_prefixo[20], tmp_data[20], tmp_modelo[100], tmp_categoria[100], tmp_quantidadeLugares[20], tmp_codLinha[20];
    
    vehicle_register *data = (vehicle_register *)malloc(0);
    int data_length = 0;

    // receives from stdin "no_inputs" vehicle registers and parses it's fields
    for(int i = 0; i < no_inputs; i++){
        scanf("%s %s %s %s %s %s", tmp_prefixo, tmp_data, tmp_quantidadeLugares, tmp_codLinha, tmp_modelo, tmp_categoria);


        // allocates a new register to the array
        data = (vehicle_register *)realloc(data, ++data_length * sizeof(vehicle_register));

        // sets the "removido" field to 1 (meaning it was not removed)
        data[data_length-1].removido = '1';

        // "prefixo" won't ever be null (ignores the surrounding double quotes)
        strncpy(data[data_length-1].prefixo, &(tmp_prefixo[1]), 5);

        // if date is not null, copies the string from tmp_data 
        // (the copying ignores the surrounding double quotes)
        if(strcmp(tmp_data, "NULO")){ strncpy(data[data_length-1].data, &(tmp_data[1]), 10); }

        // if the date is null, sets it to "\0@@@@@@@@@@"
        else{
            data[data_length-1].data[0] = '\0';
            memset(&(data[data_length-1].data[1]), '@', 9);
        }

        // same as above but for the number of seats
        data[data_length-1].quantidadeLugares = strcmp(tmp_quantidadeLugares, "NULO") ? atoi(tmp_quantidadeLugares) : -1;

        // same as above but for the line code
        data[data_length-1].codLinha = strcmp(tmp_codLinha, "NULO") ? atoi(tmp_codLinha) : -1;

        // same as above but for the model
        data[data_length-1].modelo = strcmp(tmp_modelo, "NULO") ? tmp_modelo : "";

        // same as above but for the category
        data[data_length-1].categoria = strcmp(tmp_categoria, "NULO") ? tmp_categoria : "";

        // sets the variable fields's sizes
        data[data_length-1].tamanhoModelo = strlen(data[data_length-1].modelo);
        data[data_length-1].tamanhoCategoria = strlen(data[data_length-1].categoria);

        // if model isn't null, removes the surrounding double quotes from the string
        if(data[data_length-1].tamanhoModelo){
            data[data_length-1].modelo[data[data_length-1].tamanhoModelo-1] = '\0';
            (data[data_length-1].modelo)++;
            data[data_length-1].tamanhoModelo -= 2;
        }

        // if category isn't null, removes the surrounding double quotes from the string
        if(data[data_length-1].tamanhoCategoria){
            data[data_length-1].categoria[data[data_length-1].tamanhoCategoria] = '\0';
            (data[data_length-1].categoria)++;
            data[data_length-1].tamanhoCategoria -= 2;
        }

        // sets this register's size
        data[data_length-1].tamanhoRegistro = VEHICLE_DATA_STATIC_LENGTH + data[data_length-1].tamanhoModelo + data[data_length-1].tamanhoCategoria;

    }

    vehicle *parsed = (vehicle *)malloc(sizeof(vehicle));
    parsed->header = NULL;
    parsed->data = data;
    parsed->data_length = data_length;

    return parsed;
}

// receives a filename, reads "no_inputs" vehicles and appends it to the file
void append_vehicle_bin(char *filename, int no_inputs){
    char *basepath = "./binaries/";

    // string that has the .csv filepath (inside the "data" directory)
    char *filepath = (char *)malloc((strlen(basepath) + strlen(filename) + 1) * sizeof(char));

    // sets filepath's value
    strcpy(filepath, basepath);
    strcat(filepath, filename);

    // opens file in binary-appending+ mode
    FILE *binary = fopen(filepath, "a+b");

    // if the files could not be created, raises error and exists program
    if(!binary){
        printf("Falha no processamento do arquivo.\n");
        exit(0);
    }

    // receives from stdin "no_inputs" vehicle registers and parses it's fields
    vehicle *parsed = read_vehicle_input(no_inputs);

    // the file header's fields that'll be edited
    char header_status = '0';
    long long header_byteProxReg;
    int header_nroRegistros;

    // goes to the start of file, sets status to '0' (not consistent)
    // and header the byteProxReg and nroRegistros's values to variables
    fseek(binary, 0, SEEK_SET);
    fwrite(&header_status, sizeof(char), 1, binary);
    fread(&header_byteProxReg, sizeof(long long), 1, binary);
    fread(&header_nroRegistros, sizeof(int), 1, binary);
    
    // calculates the new number of registers
    header_nroRegistros += parsed->data_length;

    // goes to the end of file
    fseek(binary, 0, SEEK_END);

    // writes through each data register and writes it to disk
    for(int i = 0; i < parsed->data_length; i++){
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
        
        // calculates the header's next free byte position
        // "removido" and "tamanhoRegistro" aren't considered to 
        // "tamanhoRegistro"'s value, so it's necessary to add 5 bytes
        header_byteProxReg += parsed->data[i].tamanhoRegistro + 5;
    }

    // writes byteProxReg and nroRegistros to the file's header
    fseek(binary, 1, SEEK_SET);
    fwrite(&header_byteProxReg, sizeof(long long), 1, binary);
    fwrite(&header_nroRegistros, sizeof(int), 1, binary);

    // sets the file's header's status to '1'
    header_status = '1';
    fseek(binary, 0, SEEK_SET);
    fwrite(&header_status, sizeof(char), 1, binary);

    // closes file and frees allocated data
    fclose(binary);
    free(parsed->data);
    free(parsed);
    free(filepath);

    return;
}
