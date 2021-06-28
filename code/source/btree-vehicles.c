#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./btree-structs.c"
#include "./vehicle-structs.c"
#include "../headers/util.h"
#include "../headers/vehicles.h"
#include "../headers/btree-vehicles.h"
#include "../headers/btree.h"

void create_vehicle_btree(char *vehiclesFilename, char *btreeFilename) {
    // string that has the .bin filepath (inside the "binaries" directory)
    char *vehicles_filepath = get_filepath(vehiclesFilename, 'b');
    char *btree_filepath = get_filepath(btreeFilename, 'b');

    // opens the vehicles file in reading-binary mode
    FILE *f_vehicles = fopen(vehiclesFilename, "rb"); // vehicles file (rb)

    // if the file does not exist, raises error and exists program
    if(!f_vehicles) { raise_error(""); }

    // vehicle structs (leading underscore in order
    // to distinguish it from the typedef'd struct)
    vehicle_header _vehicle_header; // vehicle header struct
    vehicle_register _vehicle_data; // vehicle register struct

    // reads header status from vehicles file and if the
    // file is inconsistent, raises error and exists program
    fread(&_vehicle_header.status, sizeof(char), 1, f_vehicles);
    if(_vehicle_header.status != '1'){ raise_error(""); }

    // opens the btree file in binary-writing mode
    FILE *f_btree = fopen(btreeFilename, "wb"); // btree file (wb)

    // if the file could not be created, raises error and exists program
    if(!f_btree) { raise_error(""); }

    btree_header btree_header;
    btree_page btree_node;

    // sets the btree header's starting values
    btree_header.status = '0';
    btree_header.noRaiz = -1;
    btree_header.RRNproxNo = 1;
    memset(&(btree_header.lixo), '@', 68); // @@@@@@ ...

    // writes btree header to disk
    fwrite(&btree_header.status, sizeof(char), 1, f_btree);
    fwrite(&btree_header.noRaiz, sizeof(int), 1, f_btree);
    fwrite(&btree_header.RRNproxNo, sizeof(int), 1, f_btree);
    fwrite(btree_header.lixo, sizeof(char), 68, f_btree);

    // reads the vehicles file's header's remaining relevant fields
    fread(&_vehicle_header.byteProxReg, sizeof(long long), 1, f_vehicles);
    fread(&_vehicle_header.nroRegistros, sizeof(int), 1, f_vehicles);

    // moves to the start of the first vehicle data register
    fseek(f_vehicles, 175, SEEK_SET);

    // reads each register from the vehicles
    // file and inserts it in the btree
    int index = 0;
    while (index < _vehicle_header.nroRegistros) {
        // reads the current register's "removido" and "tamanhoRegistro" fields
        fread(&_vehicle_data.removido, sizeof(char), 1, f_vehicles);
        fread(&_vehicle_data.tamanhoRegistro, sizeof(int), 1, f_vehicles);

        // if the current register was removed, it'll not be inserted
        if (_vehicle_data.removido == '0') {
            fseek(f_vehicles, _vehicle_data.tamanhoRegistro, SEEK_CUR);
            continue;
        }

        // reads the current vehicle's prefix and skips to the next register
        fread(_vehicle_data.prefixo, sizeof(char), 5, f_vehicles);
        fseek(f_vehicles, _vehicle_data.tamanhoRegistro-5, SEEK_CUR);

        // convers the current vehicle's prefix to an integer
        int converted_prefix = convertePrefixo(_vehicle_data.prefixo);

        // // inserts the current vehicle to the btree
        // PROMO_R_CHILD E PROMO_KEY E PON (?)
        // __btree_insert(btree_header.noRaiz, prefixo_int, PROMO_R_CHILD, PROMO_KEY, PON, arq2);

        // // calculates btree's new root node and first free RRN
        // btree_header.noRaiz = PROMO_R_CHILD;
        // btree_header.RRNproxNo = PROMO_R_CHILD + 1;

        // // updates the btree's root node on disk
        // fseek(f_btree, 1, SEEK_SET);
        // fwrite(btree_header.noRaiz, sizeof(int), 1, f_btree);
        // fwrite(btree_header.RRNproxNo, sizeof(int), 1, f_btree);

        // increments index
        index++;
    }

    // sets the header "status" field to 1
    btree_header.status = '1';
    fseek(f_vehicles, 0, SEEK_SET);
    fwrite(&btree_header.status, sizeof(char), 1, f_btree);

    // closes files
    fclose(f_vehicles);
    fclose(f_btree);
}

void search_vehicle_btree(char *arquivoVeiculo, char *arquivoArvore, char *prefixo, char *valor){
    //converte prefixo para poder encontrar a chave
    // A manipulação do arquivo de índice árvore-B deve ser feita em disco, de acordo com o conteúdo ministrado em sala de aula.
    //Falha no processamento do arquivo.

    FILE *arq1, *arq2;
    vehicle_header header;
    vehicle_register data;
    btree_header btree_header;
    int valor_int;

    valor_int = convertePrefixo(valor);

    // opens the file in arq1-reading mode
    arq1 = fopen(arquivoVeiculo, "rb");
    arq2 = fopen(arquivoArvore, "rb");

    // if the file does not exist, raise error
    if((arq1 == NULL) || (arq2 == NULL)){ raise_error(""); }


    // procura na arvore
    fread(&btree_header.status, sizeof(char), 1, arq2);
    if(btree_header.status == '0'){ raise_error(""); }

    fread(&header.noRaiz, sizeof(int), 1, arq2);

    if(search(header.noRaiz, valor_int, achou_RRN, achou_POS, achou_Pr, arq2)){
        // if the file is inconsistent, raise error
        fread(&header.status, sizeof(char), 1, arq1);
        if(header.status == '0'){ raise_error(""); }

        // reads the header's byteProxReg and nroRegistros
        fread(&header.byteProxReg, sizeof(long long), 1, arq1);
        fread(&header.nroRegistros, sizeof(int), 1, arq1);

        // if there are no registers, raises error
        if(!header.nroRegistros){ raise_error("Registro inexistente."); }

        // reads the header's remaining fields
        fread(&header.nroRegRemovidos, sizeof(int), 1, arq1);
        fread(header.descrevePrefixo, sizeof(char), 18, arq1);
        fread(header.descreveData, sizeof(char), 35, arq1);
        fread(header.descreveLugares, sizeof(char), 42, arq1);
        fread(header.descreveLinha, sizeof(char), 26, arq1);
        fread(header.descreveModelo, sizeof(char), 17, arq1);
        fread(header.descreveCategoria, sizeof(char), 20, arq1);

        fseek(arq1, achou_Pr, SEEK_SET);

        // reads the current register's "removido" and "tamanhoRegistro" fields
        fread(&data.removido, sizeof(char), 1, arq1);
        fread(&data.tamanhoRegistro, sizeof(int), 1, arq1);

        // reads the current register's remaining fixed size fields
        fread(data.prefixo, sizeof(char), 5, arq1);
        fread(data.data, sizeof(char), 10, arq1);
        fread(&data.quantidadeLugares, sizeof(int), 1, arq1);
        fread(&data.codLinha, sizeof(int), 1, arq1);

        // reads the current register's "modelo" field (variable size)
        fread(&data.tamanhoModelo, sizeof(int), 1, arq1);
        data.modelo = (char *)malloc(sizeof(char) * data.tamanhoModelo);
        fread(data.modelo, sizeof(char), data.tamanhoModelo, arq1);

        // reads the current register's "categoria" field (variable size)
        fread(&data.tamanhoCategoria, sizeof(int), 1, arq1);
        data.categoria = (char *)malloc(sizeof(char) * data.tamanhoCategoria);
        fread(data.categoria, sizeof(char), data.tamanhoCategoria, arq1);

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
    }
    else{
        printf("Registro inexistente.\n");
    }

    fclose(arq1);
    fclose(arq2);
}

