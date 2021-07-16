#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/btree.h"
#include "../headers/lines.h"
#include "../headers/util.h"
#include "../headers/vehicles.h"
#include "../structs/btree.c"
#include "../structs/line.c"
#include "../structs/vehicle.c"


void nested_loop_join(char *vehiclesFilename, char *linesFilename){
    // string that has the .bin filepath (inside the "binaries" directory)
    char *vehicles_filepath = get_filepath(vehiclesFilename, 'b');
    char *lines_filepath    = get_filepath(linesFilename,    'b');

    // opens both files in reading mode
    FILE *f_vehicles = fopen(vehicles_filepath, "r");
    FILE *f_lines    = fopen(lines_filepath,    "r");

    // if either file doesn't exist, raises error and exists program
    if (!f_vehicles || !f_lines) { raise_error(""); }

    // vehicle structs
    vehicle_header   v_header; // vehicle header
    vehicle_register v_data;   // vehicle data register

    // line structs
    line_header   l_header; // line header
    line_register l_data;   // line data register

    // reads vehicle file's header status and if it's
    // inconsistent, raises error and exists program
    fread(&v_header.status, sizeof(char), 1, f_vehicles);
    if (v_header.status != '1') { raise_error(""); }

    // does the same for the line file
    fread(&l_header.status, sizeof(char), 1, f_vehicles);
    if (l_header.status != '1') { raise_error(""); }

    // reads both files' header's byteProxReg and nroRegistros
    // vehicles
    fread(&v_header.byteProxReg,  sizeof(long long), 1, f_vehicles);
    fread(&v_header.nroRegistros, sizeof(int),       1, f_vehicles);
    // lines
    fread(&l_header.byteProxReg,  sizeof(long long), 1, f_lines);
    fread(&l_header.nroRegistros, sizeof(int),       1, f_lines);

    // if either file has no register, raises error
    if(!v_header.nroRegistros || !l_header.nroRegistros){
        raise_error("Registro inexistente.");
    }

    // reads both headers' remaining fields
    // vehicles
    fread(&v_header.nroRegRemovidos,  sizeof(int),  1,  f_vehicles);
    fread(v_header.descrevePrefixo,   sizeof(char), 18, f_vehicles);
    fread(v_header.descreveData,      sizeof(char), 35, f_vehicles);
    fread(v_header.descreveLugares,   sizeof(char), 42, f_vehicles);
    fread(v_header.descreveLinha,     sizeof(char), 26, f_vehicles);
    fread(v_header.descreveModelo,    sizeof(char), 17, f_vehicles);
    fread(v_header.descreveCategoria, sizeof(char), 20, f_vehicles);
    // lines
    fread(&l_header.nroRegRemovidos,  sizeof(int),  1,  f_lines);
    fread(l_header.descreveCodigo,    sizeof(char), 15, f_lines);
    fread(l_header.descreveCartao,    sizeof(char), 13, f_lines);
    fread(l_header.descreveNome,      sizeof(char), 13, f_lines);
    fread(l_header.descreveCor,       sizeof(char), 24, f_lines);

    // reads and prints each matching register
    // outer loop (through vehicles)
    int i = 0;
    while(i < v_header.nroRegRemovidos){
        // reads the current vehicle's "removido" and "tamanhoRegistro" fields
        fread(&v_data.removido, sizeof(char), 1, f_vehicles);
        fread(&v_data.tamanhoRegistro, sizeof(int), 1, f_vehicles);

        // if the current vehicle was removed, it'll not be printed
        if(v_data.removido == '0'){
            fseek(f_vehicles, v_data.tamanhoRegistro, SEEK_CUR);
            continue;
        }

        // reads the current vehicle's remaining fixed size fields
        fread(v_data.prefixo, sizeof(char), 5, f_vehicles);
        fread(v_data.data, sizeof(char), 10, f_vehicles);
        fread(&v_data.quantidadeLugares, sizeof(int), 1, f_vehicles);
        fread(&v_data.codLinha, sizeof(int), 1, f_vehicles);

        // reads the current vehicle's "modelo" field (variable size)
        fread(&v_data.tamanhoModelo, sizeof(int), 1, f_vehicles);
        v_data.modelo = (char *)malloc(sizeof(char) * v_data.tamanhoModelo);
        fread(v_data.modelo, sizeof(char), v_data.tamanhoModelo, f_vehicles);

        // reads the current vehicle's "categoria" field (variable size)
        fread(&v_data.tamanhoCategoria, sizeof(int), 1, f_vehicles);
        v_data.categoria = (char *)malloc(sizeof(char) * v_data.tamanhoCategoria);
        fread(v_data.categoria, sizeof(char), v_data.tamanhoCategoria, f_vehicles);

        // gets formatted date or null message
        char *date = v_data.data[0] != '\0' ? format_date(v_data.data) : "campo com valor nulo";

        // takes the cursor at the lines' file to the first
        // data register's start (if it's not already there)
        if(ftell(f_lines) != LINE_HEADER_LENGTH){
            fseek(f_lines, LINE_HEADER_LENGTH, SEEK_SET);
        }

        // inner loop (through lines)
        int j = 0;
        while(j < l_header.nroRegRemovidos){
            // reads the current register's "removido" and "tamanhoRegistro" fields
            fread(&l_data.removido, sizeof(char), 1, f_lines);
            fread(&l_data.tamanhoRegistro, sizeof(int), 1, f_lines);

            // if the current register was removed, it'll not be printed
            if(l_data.removido == '0'){
                fseek(f_lines, l_data.tamanhoRegistro, SEEK_CUR);
                continue;
            }

            // reads the current register's remaining fixed size fields
            fread(&l_data.codLinha, sizeof(int), 1, f_lines);
            fread(&l_data.aceitaCartao, sizeof(char), 1, f_lines);

            // if this is the current vehicle's line, print all info
            if(v_data.codLinha == l_data.codLinha){
                // reads the current register's "nomeLinha" field (variable size)
                fread(&l_data.tamanhoNome, sizeof(int), 1, f_lines);
                l_data.nomeLinha = (char *)malloc(sizeof(char) * l_data.tamanhoNome);
                fread(l_data.nomeLinha, sizeof(char), l_data.tamanhoNome, f_lines);

                // reads the current register's "corLinha" field (variable size)
                fread(&l_data.tamanhoCor, sizeof(int), 1, f_lines);
                l_data.corLinha = (char *)malloc(sizeof(char) * l_data.tamanhoCor);
                fread(l_data.corLinha, sizeof(char), l_data.tamanhoCor, f_lines);

                // gets formatted card status
                char *card_status = format_card(l_data.aceitaCartao);

                // prints the current vehicle's fields
                print_string_field(v_header.descrevePrefixo,   18, v_data.prefixo,   5);
                print_string_field(v_header.descreveModelo,    17, v_data.modelo,    v_data.tamanhoModelo);
                print_string_field(v_header.descreveCategoria, 20, v_data.categoria, v_data.tamanhoCategoria);
                print_string_field(v_header.descreveData,      35, date,             strlen(date));
                print_int_field(v_header.descreveLugares, 42, v_data.quantidadeLugares);

                // prints the current line's fields
                print_int_field(l_header.descreveCodigo,  15, l_data.codLinha);
                print_string_field(l_header.descreveNome,   13, l_data.nomeLinha, l_data.tamanhoNome);
                print_string_field(l_header.descreveCor,    24, l_data.corLinha,  l_data.tamanhoCor);
                print_string_field(l_header.descreveCartao, 13, card_status,      strlen(card_status));

                // prints newline
                printf("\n");

                // frees allocated line strings
                free(l_data.nomeLinha);
                free(l_data.corLinha);

                break;
            }

            // if it isn't, goes to next line register
            else{ fseek(f_lines, l_data.tamanhoRegistro - 4, SEEK_CUR); }

            // increments j
            j++;
        }

        // frees allocated vehicle strings
        free(v_data.modelo);
        free(v_data.categoria);
        if(v_data.data[0] != '\0' ){ free(date); }

        // increments i
        i++;
    }

    return;
}
