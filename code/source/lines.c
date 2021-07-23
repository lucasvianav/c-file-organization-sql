/**
 * AUTHOR: Lucas Viana Vilela
 * NO.USP: 10748409
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../structs/line.h"
#include "../structs/btree.h"
#include "../headers/lines.h"
#include "../headers/util.h"
#include "../headers/btree.h"
#include "../headers/file-handling.h"

// PRIVATE FUNCTIONS

// receives a line-csv string and parses it,
// returning the pointer to a "line" struct
line *parse_line_csv(char *content){
    // temporary/auxiliar variables to read strings
    char *tmp_row, *tmp_string;

    // header register
    line_header *header = (line_header *)malloc(sizeof(line_header));

    // array of data registers
    line_register *data = (line_register *)malloc(0);
    int data_length = 0;

    // gets header row
    tmp_row = strsep(&content, "\n");

    // parses the header row's values
    header->status = '0';
    header->byteProxReg = LINE_HEADER_LENGTH;
    header->nroRegRemovidos = 0;
    strcpy(header->descreveCodigo, strsep(&tmp_row, ","));
    strcpy(header->descreveCartao, strsep(&tmp_row, ","));
    strcpy(header->descreveNome, strsep(&tmp_row, ","));
    strcpy(header->descreveCor, strsep(&tmp_row, ","));

    // loops through every data row (register)
    // strcmp returns 0 if both are equal and != 0 otherwise
    while(strcmp(content, "")){
        // gets current row (register)
        tmp_row = strsep(&content, "\n");

        // allocates a new register to the array
        data = (line_register *)realloc(data, ++data_length * sizeof(line_register));

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

        // reads and parses "codLinha" (will never be null)
        tmp_string = strsep(&tmp_row, ",");
        data[data_length-1].codLinha = atoi(tmp_string);

        // same as above but for the "aceitaCartao" and checking if it's null
        tmp_string = strsep(&tmp_row, ",");
        data[data_length-1].aceitaCartao = strcmp(tmp_string, "NULO") ? tmp_string[0] : '\0';

        // same as above but for the "nomeLinha"
        tmp_string = strsep(&tmp_row, ",");
        data[data_length-1].nomeLinha = strcmp(tmp_string, "NULO") ? tmp_string : "";

        // same as above but for the "corLinha"
        tmp_string = strsep(&tmp_row, ",");
        data[data_length-1].corLinha = strcmp(tmp_string, "NULO") ? tmp_string : "";

        // sets the variable fields's sizes
        data[data_length-1].tamanhoNome = strlen(data[data_length-1].nomeLinha);
        data[data_length-1].tamanhoCor = strlen(data[data_length-1].corLinha);

        // sets this register's size
        data[data_length-1].tamanhoRegistro = LINE_DATA_STATIC_LENGTH + data[data_length-1].tamanhoNome + data[data_length-1].tamanhoCor;

        // sets the header's next free byte position
        // "removido" and "tamanhoRegistro" aren't considered to
        // "tamanhoRegistro"'s value, so it's necessary to add 5 bytes
        header->byteProxReg += data[data_length-1].tamanhoRegistro + 5;
    }

    // sets the header's number of registers in the file
    header->nroRegistros = data_length - header->nroRegRemovidos;

    line *parsed = (line *)malloc(sizeof(line));
    parsed->header = header;
    parsed->data = data;
    parsed->data_length = data_length;

    return parsed;
}

// reads from stdin "no_inputs" line registers and parses it's fields
line *read_line_input(int no_inputs){
    int tmp_codLinha;
    char tmp_aceitaCartao[20], tmp_nomeLinha[100], tmp_corLinha[100];

    line_register *data = (line_register *)malloc(0);
    int data_length = 0;

    // receives from stdin "no_inputs" line registers and parses it's fields
    for(int i = 0; i < no_inputs; i++){
        scanf("%d", &tmp_codLinha);
        scan_quote_string(tmp_aceitaCartao);
        scan_quote_string(tmp_nomeLinha);
        scan_quote_string(tmp_corLinha);

        // allocates a new register to the array
        data = (line_register *)realloc(data, ++data_length * sizeof(line_register));

        // sets the "removido" field to 1 (meaning it was not removed)
        data[data_length-1].removido = '1';

        data[data_length-1].codLinha = tmp_codLinha;

        data[data_length-1].aceitaCartao = strcmp(tmp_aceitaCartao, "NULO") ? tmp_aceitaCartao[0] : '\0';

        // same as above but for the model
        data[data_length-1].nomeLinha = strdup(tmp_nomeLinha);

        // same as above but for the category
        data[data_length-1].corLinha = strdup(tmp_corLinha);

        // sets the variable fields's sizes
        data[data_length-1].tamanhoNome = strlen(data[data_length-1].nomeLinha);
        data[data_length-1].tamanhoCor = strlen(data[data_length-1].corLinha);

        // sets this register's size
        data[data_length-1].tamanhoRegistro = LINE_DATA_STATIC_LENGTH + data[data_length-1].tamanhoNome + data[data_length-1].tamanhoCor;

    }

    line *parsed = (line *)malloc(sizeof(line));
    parsed->header = NULL;
    parsed->data = data;
    parsed->data_length = data_length;

    return parsed;
}

// writes the reveived line's data
// register to the received file
void fwrite_line_data_register(line_register data, FILE *file){
    fwrite(&(data.removido),        sizeof(char), 1,                file);
    fwrite(&(data.tamanhoRegistro), sizeof(int),  1,                file);
    fwrite(&(data.codLinha),        sizeof(int),  1,                file);
    fwrite(&(data.aceitaCartao),    sizeof(char), 1,                file);
    fwrite(&(data.tamanhoNome),     sizeof(int),  1,                file);
    fwrite(data.nomeLinha,          sizeof(char), data.tamanhoNome, file);
    fwrite(&(data.tamanhoCor),      sizeof(int),  1,                file);
    fwrite(data.corLinha,           sizeof(char), data.tamanhoCor,  file);
}

// writes the reveived line's header
// register to the received file
void fwrite_lines_header(line_header header, FILE *file){
    fwrite(&(header.status),          sizeof(char), 1,  file);
    fwrite(&(header.byteProxReg),     sizeof(long), 1,  file);
    fwrite(&(header.nroRegistros),    sizeof(int),  1,  file);
    fwrite(&(header.nroRegRemovidos), sizeof(int),  1,  file);
    fwrite(header.descreveCodigo,     sizeof(char), 15, file);
    fwrite(header.descreveCartao,     sizeof(char), 13, file);
    fwrite(header.descreveNome,       sizeof(char), 13, file);
    fwrite(header.descreveCor,        sizeof(char), 24, file);
}










// PUBLIC FUNCTIONS

void write_line_bin(char *filename, char *content){
    char *basepath = "./binaries/";

    // string that has the .csv filepath (inside the "data" directory)
    char *filepath = (char *)malloc((strlen(basepath) + strlen(filename) + 1) * sizeof(char));

    // sets filepath's value
    strcpy(filepath, basepath);
    strcat(filepath, filename);

    // opens file in binary-writing mode
    FILE *binary = fopen(filepath, "wb");

    // if the files could not be created, raises error and exists program
    if(!binary){ raise_error(""); }

    // parses the content string
    line *parsed = parse_line_csv(content);

    // writes header to disk
    fwrite(&(parsed->header->status), sizeof(char), 1, binary);
    fwrite(&(parsed->header->byteProxReg), sizeof(long), 1, binary);
    fwrite(&(parsed->header->nroRegistros), sizeof(int), 1, binary);
    fwrite(&(parsed->header->nroRegRemovidos), sizeof(int), 1, binary);
    fwrite(&(parsed->header->descreveCodigo), sizeof(char), 15, binary);
    fwrite(&(parsed->header->descreveCartao), sizeof(char), 13, binary);
    fwrite(&(parsed->header->descreveNome), sizeof(char), 13, binary);
    fwrite(&(parsed->header->descreveCor), sizeof(char), 24, binary);

    // writes through each data register and writes it to disk
    for(int i = 0; i < parsed->data_length; i++){
        fwrite(&(parsed->data[i].removido), sizeof(char), 1, binary);
        fwrite(&(parsed->data[i].tamanhoRegistro), sizeof(int), 1, binary);
        fwrite(&(parsed->data[i].codLinha), sizeof(int), 1, binary);
        fwrite(&(parsed->data[i].aceitaCartao), sizeof(char), 1, binary);
        fwrite(&(parsed->data[i].tamanhoNome), sizeof(int), 1, binary);
        fwrite(parsed->data[i].nomeLinha, sizeof(char), parsed->data[i].tamanhoNome, binary);
        fwrite(&(parsed->data[i].tamanhoCor), sizeof(int), 1, binary);
        fwrite(parsed->data[i].corLinha, sizeof(char), parsed->data[i].tamanhoCor, binary);
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

void append_line_bin(char *filename, int no_inputs){
    char *basepath = "./binaries/";

    // string that has the .csv filepath (inside the "data" directory)
    char *filepath = (char *)malloc((strlen(basepath) + strlen(filename) + 1) * sizeof(char));

    // sets filepath's value
    strcpy(filepath, basepath);
    strcat(filepath, filename);

    // opens file in binary-appending+ mode
    FILE *binary = fopen(filepath, "r+b");

    // if the files could not be created, raises error and exists program
    if(!binary){ raise_error(""); }

    // receives from stdin "no_inputs" line registers and parses it's fields
    line *parsed = read_line_input(no_inputs);

    // the file header's fields that'll be edited
    char header_status;
    long long header_byteProxReg;
    int header_nroRegistros;

    // reads header status and if the file is inconsistent, raises error and exists program
    fread(&header_status, sizeof(char), 1, binary);
    if(header_status != '1'){ raise_error(""); }

    // goes to the start of file, sets status to '0' (not consistent)
    // and header the byteProxReg and nroRegistros's values to variables
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
        fwrite(&(parsed->data[i].codLinha), sizeof(int), 1, binary);
        fwrite(&(parsed->data[i].aceitaCartao), sizeof(char), 1, binary);
        fwrite(&(parsed->data[i].tamanhoNome), sizeof(int), 1, binary);
        fwrite(parsed->data[i].nomeLinha, sizeof(char), parsed->data[i].tamanhoNome, binary);
        fwrite(&(parsed->data[i].tamanhoCor), sizeof(int), 1, binary);
        fwrite(parsed->data[i].corLinha, sizeof(char), parsed->data[i].tamanhoCor, binary);

        // frees allocated strings
        free(parsed->data[i].nomeLinha);
        free(parsed->data[i].corLinha);
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

void append_line_bin_btree(char *linesFilename, char *btreeFilename, int no_inputs){
    // string that has the .bin filepath (inside the "binaries" directory)
    char *lines_filepath = get_filepath(linesFilename, 'b');
    char *btree_filepath = get_filepath(btreeFilename, 'b');

    // opens file in binary-appending+ mode
    FILE *f_lines = fopen(lines_filepath, "r+b");

    // if the file does not exist, raises error and exists program
    if (!f_lines) { raise_error(""); }

    // line header's status field
    char f_lines_status;

    // reads header status and if the file is inconsistent, raises error and exists program
    fread(&f_lines_status, sizeof(char), 1, f_lines);
    if (f_lines_status != '1') { raise_error(""); }

    // opens the btree file in binary-writing mode
    FILE *f_btree = fopen(btree_filepath, "r+b"); // btree file (wb)

    // if the file could not be created, raises error and exists program
    if(!f_btree) { raise_error(""); }

    // vehicle header struct (leading underscore in order
    // to distinguish it from the typedef'd struct)
    btree_header _btree_header;

    // reads header status from btree file and if the
    // file is inconsistent, raises error and exists program
    fread(&_btree_header.status, sizeof(char), 1, f_btree);
    if (_btree_header.status != '1') { raise_error(""); }

    // receives from stdin "no_inputs" line registers and parses it's fields
    line *parsed = read_line_input(no_inputs);

    // other line header's fields that'll be edited
    long long f_lines_byteProxReg;
    int f_lines_nroRegistros;

    // goes to the start of line file, sets status to '0' (not consistent)
    // and reads the byteProxReg and nroRegistros's values to variables
    f_lines_status = '0';
    fseek(f_lines, 0, SEEK_SET);
    fwrite(&f_lines_status, sizeof(char), 1, f_lines);
    fread(&f_lines_byteProxReg,  sizeof(long long), 1, f_lines);
    fread(&f_lines_nroRegistros, sizeof(int),       1, f_lines);

    // goes to the start of btree file, sets status to '0' (not consistent)
    _btree_header.status = '0';
    fseek(f_lines, 0, SEEK_SET);
    fwrite(&_btree_header.status, sizeof(char), 1, f_lines);

    // reads the btree's header info
    fread(&_btree_header.noRaiz    , sizeof(int)  , 1  , f_btree);
    fread(&_btree_header.RRNproxNo , sizeof(int)  , 1  , f_btree);

    // calculates the new number of registers
    f_lines_nroRegistros += parsed->data_length;

    // goes to the end of file
    fseek(f_lines, 0, SEEK_END);

    // loops through each data register and writes it to disk
    for(int i = 0; i < parsed->data_length; i++){
        // current line register's btye offset
        long byte_offset = ftell(f_lines);

        fwrite(&(parsed->data[i].removido),        sizeof(char), 1,                           f_lines);
        fwrite(&(parsed->data[i].tamanhoRegistro), sizeof(int),  1,                           f_lines);
        fwrite(&(parsed->data[i].codLinha),        sizeof(int),  1,                           f_lines);
        fwrite(&(parsed->data[i].aceitaCartao),    sizeof(char), 1,                           f_lines);
        fwrite(&(parsed->data[i].tamanhoNome),     sizeof(int),  1,                           f_lines);
        fwrite(parsed->data[i].nomeLinha,          sizeof(char), parsed->data[i].tamanhoNome, f_lines);
        fwrite(&(parsed->data[i].tamanhoCor),      sizeof(int),  1,                           f_lines);
        fwrite(parsed->data[i].corLinha,           sizeof(char), parsed->data[i].tamanhoCor,  f_lines);

        // frees allocated strings
        free(parsed->data[i].nomeLinha);
        free(parsed->data[i].corLinha);

        // inserts the current line to the btree
        __btree_insert(
                parsed->data[i].codLinha,
                byte_offset,
                f_btree,
                &_btree_header.noRaiz,
                &_btree_header.RRNproxNo
                );
    }

    // sets byteProxReg to end of file
    f_lines_byteProxReg = ftell(f_lines);

    // writes byteProxReg and nroRegistros to the file's header
    fseek(f_lines, 1, SEEK_SET);
    fwrite(&f_lines_byteProxReg, sizeof(long long), 1, f_lines);
    fwrite(&f_lines_nroRegistros, sizeof(int), 1, f_lines);

    // sets the file's header's status to '1'
    f_lines_status = '1';
    fseek(f_lines, 0, SEEK_SET);
    fwrite(&f_lines_status, sizeof(char), 1, f_lines);

    // writes the btree's root node rrn and next free rrn
    fseek(f_btree, 1, SEEK_SET);
    fwrite(&_btree_header.noRaiz    , sizeof(int)  , 1  , f_btree);
    fwrite(&_btree_header.RRNproxNo , sizeof(int)  , 1  , f_btree);

    // sets the btree header "status" field to 1
    _btree_header.status = '1';
    fseek(f_btree, 0, SEEK_SET);
    fwrite(&_btree_header.status, sizeof(char), 1, f_btree);

    // closes file and frees allocated data
    fclose(f_lines);
    fclose(f_btree);
    free(parsed->data);
    free(parsed);
    free(lines_filepath);
    free(btree_filepath);

    return;
}

void print_line_bin(char *filename){
    // string that has the .bin filepath (inside the "binaries" directory)
    char *filepath = get_filepath(filename, 'b');

    FILE *binary;
    line_header header;
    line_register data;

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
    fread(header.descreveCodigo, sizeof(char), 15, binary);
    fread(header.descreveCartao, sizeof(char), 13, binary);
    fread(header.descreveNome, sizeof(char), 13, binary);
    fread(header.descreveCor, sizeof(char), 24, binary);

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
        fread(&data.codLinha, sizeof(int), 1, binary);
        fread(&data.aceitaCartao, sizeof(char), 1, binary);

        // reads the current register's "nomeLinha" field (variable size)
        fread(&data.tamanhoNome, sizeof(int), 1, binary);
        data.nomeLinha = (char *)malloc(sizeof(char) * data.tamanhoNome);
        fread(data.nomeLinha, sizeof(char), data.tamanhoNome, binary);

        // reads the current register's "corLinha" field (variable size)
        fread(&data.tamanhoCor, sizeof(int), 1, binary);
        data.corLinha = (char *)malloc(sizeof(char) * data.tamanhoCor);
        fread(data.corLinha, sizeof(char), data.tamanhoCor, binary);

        // gets formatted card status
        char *card_status = format_card(data.aceitaCartao);

        // prints the current register's fields
        print_int_field(header.descreveCodigo, 15, data.codLinha);
        print_string_field(header.descreveNome, 13, data.nomeLinha, data.tamanhoNome);
        print_string_field(header.descreveCor, 24, data.corLinha, data.tamanhoCor);
        print_string_field(header.descreveCartao, 13, card_status, strlen(card_status));

        // prints newline
        printf("\n");

        // frees allocated strings
        free(data.nomeLinha);
        free(data.corLinha);

        // increments index
        index++;
    }

    fclose(binary);
    free(filepath);
}

void search_line_bin(char *filename, char *key, char *value){
    // string that has the .bin filepath (inside the "binaries" directory)
    char *filepath = get_filepath(filename, 'b');

    FILE *binary;
    line_header header;
    line_register data;

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
    fread(header.descreveCodigo, sizeof(char), 15, binary);
    fread(header.descreveCartao, sizeof(char), 13, binary);
    fread(header.descreveNome, sizeof(char), 13, binary);
    fread(header.descreveCor, sizeof(char), 24, binary);

    // if any matching vehicle was found
    int found_any = 0;

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
        fread(&data.codLinha, sizeof(int), 1, binary);
        fread(&data.aceitaCartao, sizeof(char), 1, binary);

        // reads the current register's "nomeLinha" field (variable size)
        fread(&data.tamanhoNome, sizeof(int), 1, binary);
        data.nomeLinha = (char *)malloc(sizeof(char) * data.tamanhoNome);
        fread(data.nomeLinha, sizeof(char), data.tamanhoNome, binary);

        // reads the current register's "corLinha" field (variable size)
        fread(&data.tamanhoCor, sizeof(int), 1, binary);
        data.corLinha = (char *)malloc(sizeof(char) * data.tamanhoCor);
        fread(data.corLinha, sizeof(char), data.tamanhoCor, binary);

        if(
                ( !strcmp(key, "codLinha") && atoi(value) == data.codLinha ) ||
                ( !strcmp(key, "aceitaCartao") && value[0] == data.aceitaCartao ) ||
                ( !strcmp(key, "nomeLinha") && !cmp_string_field(value, strlen(value), data.nomeLinha, data.tamanhoNome) ) ||
                ( !strcmp(key, "corLinha") && !cmp_string_field(value, strlen(value), data.corLinha, data.tamanhoCor) )
          ){
            // gets formatted card status
            char *card_status = format_card(data.aceitaCartao);

            // prints the current register's fields
            print_int_field(header.descreveCodigo, 15, data.codLinha);
            print_string_field(header.descreveNome, 13, data.nomeLinha, data.tamanhoNome);
            print_string_field(header.descreveCor, 24, data.corLinha, data.tamanhoCor);
            print_string_field(header.descreveCartao, 13, card_status, strlen(card_status));

            // prints newline
            printf("\n");

            found_any = 1;
        }

        // frees allocated strings
        free(data.nomeLinha);
        free(data.corLinha);

        // increments index
        index++;
    }

    // if no matching register was found
    if(!found_any){ raise_error("Registro inexistente."); }

    fclose(binary);
    free(filepath);
}

void sort_lines_bin(char *originalFilename, char *sortedFilename){
    FILE *f_original = fopen_validate_binary(originalFilename, "rb");
    FILE *f_sorted   = fopen_validate_binary(sortedFilename, "wb");

    // headers
    line_header original_header;
    line_header sorted_header;

    // array of data (from non-removed registers)
    line_register *data;

    // reads the files' header
    original_header = fread_line_header(f_original);

    // allocates memory for the array with
    // "original_header.nroRegistros" elements
    data = (line_register *)malloc(original_header.nroRegistros * sizeof(line_register));

    // sets the sorted file's header info
    sorted_header.status                   = '0';
    sorted_header.byteProxReg              = LINE_HEADER_LENGTH;
    sorted_header.nroRegistros             = original_header.nroRegistros;
    sorted_header.nroRegRemovidos          = 0;
    copy_char_vector(sorted_header.descreveCodigo, original_header.descreveCodigo, 15);
    copy_char_vector(sorted_header.descreveCartao, original_header.descreveCartao, 13);
    copy_char_vector(sorted_header.descreveNome,   original_header.descreveNome,   13);
    copy_char_vector(sorted_header.descreveCor,    original_header.descreveCor,    24);

    // writes the sorted file's header
    fwrite_lines_header(sorted_header, f_sorted);

    // reads the whole original data file into RAM
    int i = 0; // while-loop index
    while (i < original_header.nroRegistros) {
        // reads the current register
        data[i] = fread_line_register(f_original);

        // if the current register was removed, it'll not be saved
        if (data[i].removido == '0') {
            fseek(f_original, data[i].tamanhoRegistro, SEEK_CUR);
            continue;
        }

        // increments index
        i++;
    }

    // sorts the data array in place with C's quick sort
    qsort(data, original_header.nroRegistros, sizeof(line_register), cmp_lines);

    // loops through each sorted data register
    for(i = 0; i < sorted_header.nroRegistros; i++){
        // writes the sorted data array to f_sorted
        fwrite_line_data_register(data[i], f_sorted);

        // frees this register allocated strings
        free(data[i].nomeLinha);
        free(data[i].corLinha);
    }

    // sets byteProxReg to end of file
    sorted_header.byteProxReg = ftell(f_sorted);

    // rewrites byteProxReg to the sorted file's header
    fseek(f_sorted, 1, SEEK_SET);
    fwrite(&sorted_header.byteProxReg,  sizeof(long long), 1, f_sorted);
    fwrite(&sorted_header.nroRegistros, sizeof(int),       1, f_sorted);

    // sets the sorted file status as consistent
    sorted_header.status = '1';
    fseek(f_sorted, 0, SEEK_SET);
    fwrite(&sorted_header.status, sizeof(char), 1, f_sorted);

    // closes file and frees allocated memory
    fclose(f_original);
    fclose(f_sorted);
    free(data);

    return;
}

