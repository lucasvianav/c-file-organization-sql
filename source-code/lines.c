/** AUTHORS:
 * Lucas Viana Vilela 10748409
 * Stéfane Tame Monteiro Oliveira 10829970
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./line-structs.c"
#include "lines.h"
#include "util.h"

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

// receives a filename, parses the content and writes it to the file
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

// receives a filename, reads "no_inputs" lines and appends it to the file
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

// receives a card status character and returns a 
// formatted card status string (no need to free it)
char *format_card(char card_status){
    switch(card_status){
        case 'S':
            return "PAGAMENTO SOMENTE COM CARTAO SEM PRESENCA DE COBRADOR";
            break;

        case 'N':
            return "PAGAMENTO EM CARTAO E DINHEIRO";
            break;

        case 'F':
            return "PAGAMENTO EM CARTAO SOMENTE NO FINAL DE SEMANA";
            break;

        default:
            return "";
            break;
    }
}

// receives a filename, reads and parses all of the 
// binary file's registers and prints the parsed data
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

// receives a filename as well as a query (field key-value pair),
// parses all of the binary file's registers and prints every
// register that matches the query's parsed data
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
        }

        // frees allocated strings
        free(data.nomeLinha);
        free(data.corLinha);

        // increments index
        index++;
    }

    fclose(binary);
    free(filepath);
}
