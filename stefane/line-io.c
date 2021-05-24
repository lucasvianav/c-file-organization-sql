#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./line-structs.c"
// #include "./util.c"

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
    if(!binary){ raise_error(); }

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
    if(!binary){ raise_error(); }

    // receives from stdin "no_inputs" line registers and parses it's fields
    line *parsed = read_line_input(no_inputs);

    // the file header's fields that'll be edited
    char header_status;
    long long header_byteProxReg;
    int header_nroRegistros;

    // reads header status and if the file is inconsistent, raises error and exists program
    fread(&header_status, sizeof(char), 1, binary);
    if(header_status != '1'){ raise_error(); }

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

//stefane

void printfL(char *header, int h, char *dado, int d){
    for(int i=0; i < h; i++){
        printf("%c", header[i]);
    }
    printf(": ");
    for(int i=0; i < d; i++){
        printf("%c", dado[i]);
    }
    if(d == 0){
        printf("campo com valor nulo");
    }
    printf("\n");
}

void printfN(char *header, int h, int dado){
    for(int i=0; i < h;i++){
        printf("%c", header[i]);
    }
    if(dado == 0){
        printf(": campo com valor nulo\n");
    }
    else{
        printf(": %d\n", dado);
    }
}

void cartao(char *header, int h, char *cartao){
    for(int i=0; i < h;i++){
        printf("%c", header[i]);
    }

    if(cartao[0] == 'S'){ 
        printf(": PAGAMENTO SOMENTE COM CARTAO SEM PRESENCA DE COBRADOR\n");
    }
    else if(cartao[0] == 'N'){
        printf(": PAGAMENTO EM CARTAO E DINHEIRO\n");
    }
    else if(cartao[0] == 'F'){
        printf(": PAGAMENTO EM CARTAO SOMENTE NO FINAL DE SEMANA\n"); 
    }
}

void impressaoL(char *filename){
    FILE *arquivo;
    line_header header;
    line_register dado;

    arquivo = fopen(filename, "rb"); 

    if(arquivo == NULL){
        printf("Falha no processamento do arquivo.");
    }

    fread(&header.status, sizeof(char), 1, arquivo);
    if(strcmp(&header.status, "0")==0){
        printf("Falha no processamento do arquivo.");
    }
    else{
        fread(&header.byteProxReg, sizeof(long long), 1, arquivo);
        fread(&header.nroRegistros, sizeof(int), 1, arquivo);
        if(header.nroRegistros == 0){
            printf("Registro inexistente.");
        }
        else{
            fread(&header.nroRegRemovidos, sizeof(int), 1, arquivo);
            fread(header.descreveCodigo, sizeof(char), 15, arquivo);
            fread(header.descreveCartao, sizeof(char), 13, arquivo);
            fread(header.descreveNome, sizeof(char), 13, arquivo);
            fread(header.descreveCor, sizeof(char), 24, arquivo);

            for(int i = 0; i<header.nroRegistros; i++){
                fread(&dado.removido, sizeof(char), 1, arquivo);
                fread(&dado.tamanhoRegistro, sizeof(int), 1, arquivo);
                fread(&dado.codLinha, sizeof(int), 1, arquivo);
                fread(&dado.aceitaCartao, sizeof(char), 1, arquivo);
                fread(&dado.tamanhoNome, sizeof(int), 1, arquivo);
                dado.nomeLinha = (char *) malloc(sizeof(char)*dado.tamanhoNome);
                fread(dado.nomeLinha, sizeof(char), dado.tamanhoNome, arquivo);
                fread(&dado.tamanhoCor, sizeof(int), 1, arquivo);
                dado.corLinha = (char *) malloc(sizeof(char)*dado.tamanhoCor);
                fread(dado.corLinha, sizeof(char), dado.tamanhoCor, arquivo);

                printfN(header.descreveCodigo, sizeof(header.descreveCodigo), dado.codLinha);
                printfL(header.descreveNome, sizeof(header.descreveNome), dado.nomeLinha, dado.tamanhoNome);
                printfL(header.descreveCor, sizeof(header.descreveCor), dado.corLinha, dado.tamanhoCor);
                cartao(header.descreveCartao, sizeof(header.descreveCartao), &dado.aceitaCartao);
                
                printf("\n");

                free(dado.nomeLinha);
                free(dado.corLinha);
            }
        }
    }

    fclose(arquivo);
}

void buscaL(char *filename, char *campo, char *valor){
    FILE *arquivo;
    line_header header;
    line_register dado;
    int i = 0;

    arquivo = fopen(filename, "rb"); 

    if(arquivo == NULL){
        printf("Falha no processamento do arquivo.");
    }

    fread(&header.status, sizeof(char), 1, arquivo);
    if(strcmp(&header.status, "0")==0){
        printf("Falha no processamento do arquivo.");
    }
    else{
        fread(&header.byteProxReg, sizeof(long long), 1, arquivo);
        fread(&header.nroRegistros, sizeof(int), 1, arquivo);
        if(header.nroRegistros == 0){
            printf("Registro inexistente.");
        }
        else{
            fread(&header.nroRegRemovidos, sizeof(int), 1, arquivo);
            fread(header.descreveCodigo, sizeof(char), 15, arquivo);
            fread(header.descreveCartao, sizeof(char), 13, arquivo);
            fread(header.descreveNome, sizeof(char), 13, arquivo);
            fread(header.descreveCor, sizeof(char), 24, arquivo);

            for(int i = 0; i<header.nroRegistros; i++){
                fread(&dado.removido, sizeof(char), 1, arquivo);
                fread(&dado.tamanhoRegistro, sizeof(int), 1, arquivo);
                fread(&dado.codLinha, sizeof(int), 1, arquivo);
                fread(&dado.aceitaCartao, sizeof(char), 1, arquivo);
                fread(&dado.tamanhoNome, sizeof(int), 1, arquivo);
                dado.nomeLinha = (char *) malloc(sizeof(char)*dado.tamanhoNome);
                fread(dado.nomeLinha, sizeof(char), dado.tamanhoNome, arquivo);
                fread(&dado.tamanhoCor, sizeof(int), 1, arquivo);
                dado.corLinha = (char *) malloc(sizeof(char)*dado.tamanhoCor);
                fread(dado.corLinha, sizeof(char), dado.tamanhoCor, arquivo);

                if(strcmp(campo, "codLinha") == 0){
                    int codigo;
                    sscanf(valor, "%d", &codigo);
                    if (codigo == dado.codLinha){
                        i = 1;
                    }
                }
                else if(strcmp(campo, "aceitaCartao") == 0){
                    if(strcmp(valor, &dado.aceitaCartao) == 0){
                        i = 1;
                    }
                }
                else if(strcmp(campo, "nomeLinha") == 0){
                    if(strcmp(valor, dado.nomeLinha) == 0){
                        i = 1;
                    }
                }
                else if(strcmp(campo, "corLinha") == 0){
                    if(strcmp(valor, dado.corLinha) == 0){
                        i = 1;
                    }
                }

                if(i == 1){
                    printfN(header.descreveCodigo, sizeof(header.descreveCodigo), dado.codLinha);
                    printfL(header.descreveNome, sizeof(header.descreveNome), dado.nomeLinha, dado.tamanhoNome);
                    printfL(header.descreveCor, sizeof(header.descreveCor), dado.corLinha, dado.tamanhoCor);
                    cartao(header.descreveCartao, sizeof(header.descreveCartao), &dado.aceitaCartao);
                    
                    printf("\n");
                }

                free(dado.nomeLinha);
                free(dado.corLinha);
            }
        }
    }

    fclose(arquivo);
}
