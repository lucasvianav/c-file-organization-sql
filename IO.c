#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./structs.c"

// reads a CSV file and returns the content as a string
char *read_csv(char *filename){
    char *basepath = "./data/";

    // string that has the .csv filepath (inside the "data" directory)
    char *filepath = (char *)malloc((strlen(basepath) + strlen(filename) + 1) * sizeof(char));

    // sets filepath's value
    strcpy(filepath, basepath);
    strcat(filepath, filename);
    
    // opens the file in reading mode
    FILE *fp = fopen(filepath, "r");

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

// receives a filename, parses the content and writes it to the file
void write_vehicle_bin(char *filename, char *content){
    // opens file in binary-writing mode
    FILE *binary = fopen(filename, "wb");

    // temporary/auxiliar variables to read strings
    char *tmp_row, *tmp_string;

    // header register
    vehicle_header header;

    // array of data registers
    vehicle_data *data = (vehicle_data *)malloc(0);
    int data_length = 0;

    // gets header row
    tmp_row = strsep(&content, "\n");

    // parses the header row's values
    strcpy(header.descrevePrefixo, strsep(&tmp_row, ","));
    strcpy(header.descreveData, strsep(&tmp_row, ","));
    strcpy(header.descreveLugares, strsep(&tmp_row, ","));
    strcpy(header.descreveLinha, strsep(&tmp_row, ","));
    strcpy(header.descreveModelo, strsep(&tmp_row, ","));
    strcpy(header.descreveCategoria, strsep(&tmp_row, ","));
    
    header.status = '0';

    header.byteProxReg = VEHICLE_HEADER_LENGTH;
    header.nroRegRemovidos = 0;

    // loops through every data row (register)
    while(strcmp(content, "")){
        // gets current row (register)
        tmp_row = strsep(&content, "\n");

        // allocates a new register to the array
        data = (vehicle_data *)realloc(data, ++data_length * sizeof(vehicle_data));
        
        // sets the "removido" field to 1 (meaning it was not removed)
        // if it was removed, it'll later be set to 0
        data[data_length-1].removido = '1';

        // if the current register was deletes
        if(tmp_row[0] == '*'){
            // increments the number of deleted register in the header
            (header.nroRegRemovidos)++;
            
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

        // debug
        // printf("%s %c %c %c %c %c %c %c %c %c %c\n", tmp_string, data[data_length-1].data[0], data[data_length-1].data[1], data[data_length-1].data[2], data[data_length-1].data[3], data[data_length-1].data[4], data[data_length-1].data[5], data[data_length-1].data[6], data[data_length-1].data[7], data[data_length-1].data[8], data[data_length-1].data[9]);

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
        
        // debug
        // printf("%d %d %d %d\n", VECHILE_DATA_STATIC_LENGTH, data[data_length-1].tamanhoModelo, data[data_length-1].tamanhoCategoria, data[data_length-1].tamanhoRegistro);

        // sets the header's next free byte position
        header.byteProxReg += data[data_length-1].tamanhoRegistro;
    }

    // sets the header's number of registers in the file
    header.nroRegistros = data_length;

    // writes header to disk
    fwrite(&(header.status), sizeof(char), 1, binary);
    fwrite(&(header.byteProxReg), sizeof(long), 1, binary);
    fwrite(&(header.nroRegistros), sizeof(int), 1, binary);
    fwrite(&(header.nroRegRemovidos), sizeof(int), 1, binary);
    fwrite(header.descrevePrefixo, sizeof(char), 18, binary);
    fwrite(header.descreveData, sizeof(char), 35, binary);
    fwrite(header.descreveLugares, sizeof(char), 42, binary);
    fwrite(header.descreveLinha, sizeof(char), 26, binary);
    fwrite(header.descreveModelo, sizeof(char), 17, binary);
    fwrite(header.descreveCategoria, sizeof(char), 20, binary);

    // writes through each data register and writes it to disk
    for(int i = 0; i < data_length; i++){
        fwrite(&(data[i].removido), sizeof(char), 1, binary);
        fwrite(&(data[i].tamanhoRegistro), sizeof(int), 1, binary);
        fwrite(data[i].prefixo, sizeof(char), 5, binary);
        fwrite(data[i].data, sizeof(char), 10, binary);
        fwrite(&(data[i].quantidadeLugares), sizeof(int), 1, binary);
        fwrite(&(data[i].codLinha), sizeof(int), 1, binary);
        fwrite(&(data[i].tamanhoModelo), sizeof(int), 1, binary);
        fwrite(data[i].modelo, sizeof(char), data[i].tamanhoModelo, binary);
        fwrite(&(data[i].tamanhoCategoria), sizeof(int), 1, binary);
        fwrite(data[i].categoria, sizeof(char), data[i].tamanhoCategoria, binary);
    }

    // sets the header "status" field to 1
    header.status = '1';
    fseek(binary, 0, SEEK_SET);
    fwrite(&(header.status), sizeof(char), 1, binary);
    
    // closes file and frees allocated data
    fclose(binary);
    free(data);

    return;
}
