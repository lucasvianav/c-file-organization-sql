/** AUTHORS:
 * Lucas Viana Vilela 10748409
 * Stéfane Tame Monteiro Oliveira 10829970
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../structs/vehicle.c"
#include "../headers/vehicles.h"
#include "../headers/util.h"
#include "../headers/btree.h"

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
    parsed->data_length = data_length;

    return parsed;
}

// receives a filename, parses the content and writes it to the file
void write_vehicle_bin(char *filename, char *content){
    // string that has the .bin filepath (inside the "binaries" directory)
    char *filepath = get_filepath(filename, 'b');

    // opens file in binary-writing mode
    FILE *binary = fopen(filepath, "wb");

    // if the files could not be created, raises error and exists program
    if(!binary){ raise_error(""); }

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
        scanf("%s %s %s %s", tmp_prefixo, tmp_data, tmp_quantidadeLugares, tmp_codLinha);
        scan_quote_string(tmp_modelo);
        scan_quote_string(tmp_categoria);

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
        data[data_length-1].modelo = strdup(tmp_modelo);

        // same as above but for the category
        data[data_length-1].categoria = strdup(tmp_categoria);

        // sets the variable fields's sizes
        data[data_length-1].tamanhoModelo = strlen(data[data_length-1].modelo);
        data[data_length-1].tamanhoCategoria = strlen(data[data_length-1].categoria);

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
    // string that has the .bin filepath (inside the "binaries" directory)
    char *filepath = get_filepath(filename, 'b');

    // opens file in binary-appending+ mode
    FILE *binary = fopen(filepath, "r+b");

    // if the file does not exist, raises error and exists program
    if(!binary){ raise_error(""); }

    // receives from stdin "no_inputs" vehicle registers and parses it's fields
    vehicle *parsed = read_vehicle_input(no_inputs);

    // the file header's fields that'll be edited
    char header_status;
    long long header_byteProxReg;
    int header_nroRegistros;

    // reads header status and if the file is inconsistent, raises error and exists program
    fread(&header_status, sizeof(char), 1, binary);
    if(header_status != '1'){ raise_error(""); }

    // goes to the start of file, sets status to '0' (not consistent)
    // and reads the byteProxReg and nroRegistros's values to variables
    header_status = '0';
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

        // frees allocated strings
        free(parsed->data[i].modelo);
        free(parsed->data[i].categoria);
    }

    // sets byteProxReg to end of file
    header_byteProxReg = ftell(binary);

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

// receives a filename, reads "no_inputs" vehicles and appends it to the file
// also inserts it's reference on the passed btree
void append_vehicle_bin_btree(char *vehiclesFilename, char *btreeFilename, int no_inputs){
    // string that has the .bin filepath (inside the "binaries" directory)
    char *vehicles_filepath = get_filepath(vehiclesFilename, 'b');
    char *btree_filepath = get_filepath(btreeFilename, 'b');

    // opens file in binary-appending+ mode
    FILE *f_vehicles = fopen(vehicles_filepath, "r+b");

    // if the file does not exist, raises error and exists program
    if (!f_vehicles) { raise_error(""); }

    // vehicle header's status field
    char f_vehicles_status;

    // reads header status and if the file is inconsistent, raises error and exists program
    fread(&f_vehicles_status, sizeof(char), 1, f_vehicles);
    if (f_vehicles_status != '1') { raise_error(""); }

    // opens the btree file in binary-writing mode
    FILE *f_btree = fopen(btree_filepath, "r+b"); // btree file (wb)

    // if the file could not be created, raises error and exists program
    if(!f_btree) { raise_error(""); }

    // btree file header's status field
    char f_btree_status;

    // reads header status from btree file and if the
    // file is inconsistent, raises error and exists program
    fread(&f_btree_status, sizeof(char), 1, f_btree);
    if (f_btree_status != '1') { raise_error(""); }

    // receives from stdin "no_inputs" vehicle registers and parses it's fields
    vehicle *parsed = read_vehicle_input(no_inputs);

    // other vehicle header's fields that'll be edited
    long long f_vehicles_byteProxReg;
    int f_vehicles_nroRegistros;

    // goes to the start of vehicle file, sets status to '0' (not consistent)
    // and reads the byteProxReg and nroRegistros's values to variables
    f_vehicles_status = '0';
    fseek(f_vehicles, 0, SEEK_SET);
    fwrite(&f_vehicles_status, sizeof(char), 1, f_vehicles);
    fread(&f_vehicles_byteProxReg,  sizeof(long long), 1, f_vehicles);
    fread(&f_vehicles_nroRegistros, sizeof(int),       1, f_vehicles);

    // goes to the start of btree file, sets status to '0' (not consistent)
    f_btree_status = '0';
    fseek(f_vehicles, 0, SEEK_SET);
    fwrite(&f_btree_status, sizeof(char), 1, f_vehicles);

    // calculates the new number of registers
    f_vehicles_nroRegistros += parsed->data_length;

    // goes to the end of file
    fseek(f_vehicles, 0, SEEK_END);

    // loops through each data register and writes it to disk
    for(int i = 0; i < parsed->data_length; i++){
        // current vehicle register's btye offset
        long byte_offset = ftell(f_vehicles);

        fwrite(&(parsed->data[i].removido),          sizeof(char), 1,                                f_vehicles);
        fwrite(&(parsed->data[i].tamanhoRegistro),   sizeof(int),  1,                                f_vehicles);
        fwrite(parsed->data[i].prefixo,              sizeof(char), 5,                                f_vehicles);
        fwrite(parsed->data[i].data,                 sizeof(char), 10,                               f_vehicles);
        fwrite(&(parsed->data[i].quantidadeLugares), sizeof(int),  1,                                f_vehicles);
        fwrite(&(parsed->data[i].codLinha),          sizeof(int),  1,                                f_vehicles);
        fwrite(&(parsed->data[i].tamanhoModelo),     sizeof(int),  1,                                f_vehicles);
        fwrite(parsed->data[i].modelo,               sizeof(char), parsed->data[i].tamanhoModelo,    f_vehicles);
        fwrite(&(parsed->data[i].tamanhoCategoria),  sizeof(int),  1,                                f_vehicles);
        fwrite(parsed->data[i].categoria,            sizeof(char), parsed->data[i].tamanhoCategoria, f_vehicles);

        // frees allocated strings
        free(parsed->data[i].modelo);
        free(parsed->data[i].categoria);

        // convers the current vehicle's prefix to an integer
        int converted_prefix = convertePrefixo(parsed->data[i].prefixo);

        // inserts the current vehicle to the btree
        __btree_insert(converted_prefix, byte_offset, f_btree);
    }

    // sets byteProxReg to end of file
    f_vehicles_byteProxReg = ftell(f_vehicles);

    // writes byteProxReg and nroRegistros to the file's header
    fseek(f_vehicles, 1, SEEK_SET);
    fwrite(&f_vehicles_byteProxReg,  sizeof(long long), 1, f_vehicles);
    fwrite(&f_vehicles_nroRegistros, sizeof(int),       1, f_vehicles);

    // sets the vehicle file's header's status to '1'
    f_vehicles_status = '1';
    fseek(f_vehicles, 0, SEEK_SET);
    fwrite(&f_vehicles_status, sizeof(char), 1, f_vehicles);

    // sets the btree header "status" field to 1
    f_btree_status = '1';
    fseek(f_vehicles, 0, SEEK_SET);
    fwrite(&f_btree_status, sizeof(char), 1, f_btree);

    // closes file and frees allocated data
    fclose(f_vehicles);
    fclose(f_btree);
    free(parsed->data);
    free(parsed);
    free(vehicles_filepath);

    return;
}

// receives a filename, reads and parses all of the
// binary file's registers and prints the parsed data
void print_vehicle_bin(char *filename){
    // string that has the .bin filepath (inside the "binaries" directory)
    char *filepath = get_filepath(filename, 'b');

    vehicle_header header;
    vehicle_register data;
    FILE *binary;

    // opens the file in binary-reading mode
    binary = fopen(filepath, "rb");

    // if the file does not exist, raise error
    if(binary == NULL){ raise_error(""); }

    // if the file is inconsistent, raise error
    fread(&header.status, sizeof(char), 1, binary);
    if(header.status == '0'){ raise_error(""); }

    // reads the header's byteProxReg and nroRegistros
    fread(&header.byteProxReg, sizeof(long long), 1, binary);
    fread(&header.nroRegistros, sizeof(int), 1, binary);

    // if there are no registers, raises error
    if(!header.nroRegistros){ raise_error("Registro inexistente."); }

    // reads the header's remaining fields
    fread(&header.nroRegRemovidos, sizeof(int), 1, binary);
    fread(header.descrevePrefixo, sizeof(char), 18, binary);
    fread(header.descreveData, sizeof(char), 35, binary);
    fread(header.descreveLugares, sizeof(char), 42, binary);
    fread(header.descreveLinha, sizeof(char), 26, binary);
    fread(header.descreveModelo, sizeof(char), 17, binary);
    fread(header.descreveCategoria, sizeof(char), 20, binary);

    // reads and prints each register
    int index = 0;
    while(index < header.nroRegistros){
        // reads the current register's "removido" and "tamanhoRegistro" fields
        fread(&data.removido, sizeof(char), 1, binary);
        fread(&data.tamanhoRegistro, sizeof(int), 1, binary);

        // if the current register was removed, it'll not be printed
        if(data.removido == '0'){
            fseek(binary, data.tamanhoRegistro, SEEK_CUR);
            continue;
        }

        // reads the current register's remaining fixed size fields
        fread(data.prefixo, sizeof(char), 5, binary);
        fread(data.data, sizeof(char), 10, binary);
        fread(&data.quantidadeLugares, sizeof(int), 1, binary);
        fread(&data.codLinha, sizeof(int), 1, binary);

        // reads the current register's "modelo" field (variable size)
        fread(&data.tamanhoModelo, sizeof(int), 1, binary);
        data.modelo = (char *)malloc(sizeof(char) * data.tamanhoModelo);
        fread(data.modelo, sizeof(char), data.tamanhoModelo, binary);

        // reads the current register's "categoria" field (variable size)
        fread(&data.tamanhoCategoria, sizeof(int), 1, binary);
        data.categoria = (char *)malloc(sizeof(char) * data.tamanhoCategoria);
        fread(data.categoria, sizeof(char), data.tamanhoCategoria, binary);

        // gets formatted date or null message
        char *date = data.data[0] != '\0' ? format_date(data.data) : "campo com valor nulo";

        // prints the current register's fields
        print_string_field(header.descrevePrefixo, 18, data.prefixo, 5);
        print_string_field(header.descreveModelo, 17, data.modelo, data.tamanhoModelo);
        print_string_field(header.descreveCategoria, 20, data.categoria, data.tamanhoCategoria);
        print_string_field(header.descreveData, 35, date, strlen(date));
        print_int_field(header.descreveLugares, 42, data.quantidadeLugares);

        // prints newline
        printf("\n");

        // frees allocated strings
        free(data.modelo);
        free(data.categoria);
        if(data.data[0] != '\0' ){ free(date); }

        // increments index
        index++;
    }

    // closes the file
    fclose(binary);
    free(filepath);
}

// receives a filename as well as a query (field key-value pair),
// parses all of the binary file's registers and prints every
// register that matches the query's parsed data
void search_vehicle_bin(char *filename, char *key, char *value){
    // string that has the .bin filepath (inside the "binaries" directory)
    char *filepath = get_filepath(filename, 'b');

    vehicle_header header;
    vehicle_register data;
    FILE *binary;

    // opens the file in binary-reading mode
    binary = fopen(filepath, "rb");

    // if the file does not exist, raise error
    if(!binary){ raise_error(""); }

    // if the file is inconsistent, raise error
    fread(&header.status, sizeof(char), 1, binary);
    if(header.status == '0'){ raise_error(""); }

    // reads the header's byteProxReg and nroRegistros
    fread(&header.byteProxReg, sizeof(long long), 1, binary);
    fread(&header.nroRegistros, sizeof(int), 1, binary);

    // if there are no registers, raises error
    if(!header.nroRegistros){ raise_error("Registro inexistente."); }

    // reads the header's remaining fields
    fread(&header.nroRegRemovidos, sizeof(int), 1, binary);
    fread(header.descrevePrefixo, sizeof(char), 18, binary);
    fread(header.descreveData, sizeof(char), 35, binary);
    fread(header.descreveLugares, sizeof(char), 42, binary);
    fread(header.descreveLinha, sizeof(char), 26, binary);
    fread(header.descreveModelo, sizeof(char), 17, binary);
    fread(header.descreveCategoria, sizeof(char), 20, binary);

    // reads and prints each register
    int index = 0;
    while(index < header.nroRegistros){
        // reads the current register's "removido" and "tamanhoRegistro" fields
        fread(&data.removido, sizeof(char), 1, binary);
        fread(&data.tamanhoRegistro, sizeof(int), 1, binary);

        // if the current register was removed, it'll not be printed
        if(data.removido == '0'){
            fseek(binary, data.tamanhoRegistro, SEEK_CUR);
            continue;
        }

        // reads the current register's remaining fixed size fields
        fread(data.prefixo, sizeof(char), 5, binary);
        fread(data.data, sizeof(char), 10, binary);
        fread(&data.quantidadeLugares, sizeof(int), 1, binary);
        fread(&data.codLinha, sizeof(int), 1, binary);

        // reads the current register's "modelo" field (variable size)
        fread(&data.tamanhoModelo, sizeof(int), 1, binary);
        data.modelo = (char *)malloc(sizeof(char) * data.tamanhoModelo);
        fread(data.modelo, sizeof(char), data.tamanhoModelo, binary);

        // reads the current register's "categoria" field (variable size)
        fread(&data.tamanhoCategoria, sizeof(int), 1, binary);
        data.categoria = (char *)malloc(sizeof(char) * data.tamanhoCategoria);
        fread(data.categoria, sizeof(char), data.tamanhoCategoria, binary);

        // checks if the current register matches the query
        if(
            ( !strcmp(key, "prefixo") && !cmp_string_field(value, strlen(value), data.prefixo, 5) ) ||
            ( !strcmp(key, "data") && !cmp_string_field(value, strlen(value), data.data, 10) ) ||
            ( !strcmp(key, "quantidadeLugares") && (atoi(value) == data.quantidadeLugares) ) ||
            ( !strcmp(key, "modelo") && !cmp_string_field(value, strlen(value), data.modelo, data.tamanhoModelo) ) ||
            ( !strcmp(key, "categoria") && !cmp_string_field(value, strlen(value), data.categoria, data.tamanhoCategoria) )
        ){
            // gets formatted date or null message
            char *date = data.data[0] != '\0' ? format_date(data.data) : "campo com valor nulo";

            // prints the current register's fields
            print_string_field(header.descrevePrefixo, 18, data.prefixo, 5);
            print_string_field(header.descreveModelo, 17, data.modelo, data.tamanhoModelo);
            print_string_field(header.descreveCategoria, 20, data.categoria, data.tamanhoCategoria);
            print_string_field(header.descreveData, 35, date, strlen(date));
            print_int_field(header.descreveLugares, 42, data.quantidadeLugares);

            // prints newline
            printf("\n");

            // frees the date string if necessary
            if(data.data[0] != '\0' ){ free(date); }
        }

        // frees allocated strings
        free(data.modelo);
        free(data.categoria);

        // increments index
        index++;
    }

    // closes the file
    fclose(binary);
    free(filepath);
}
