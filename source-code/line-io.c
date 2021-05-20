#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./line-structs.c"

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

    return parsed;
}

// receives a filename, parses the content and writes it to the file
void write_line_bin(char *filename, char *content){
    // opens file in binary-writing mode
    FILE *binary = fopen(filename, "wb");

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
    int data_length = parsed->header->nroRegistros + parsed->header->nroRegRemovidos;
    for(int i = 0; i < data_length; i++){
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

    return;
}
