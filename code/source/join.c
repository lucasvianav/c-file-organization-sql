#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/btree.h"
#include "../headers/lines.h"
#include "../headers/util.h"
#include "../headers/vehicles.h"
#include "../structs/btree.h"
#include "../structs/line.h"
#include "../structs/vehicle.h"


void nested_loop_join(char *vehiclesFilename, char *linesFilename){
    FILE *f_vehicles = open_validate_binary(vehiclesFilename, "rb");
    FILE *f_lines = open_validate_binary(linesFilename, "rb");

    // vehicle structs
    vehicle_header   v_header; // vehicle header
    vehicle_register v_data;   // vehicle data register

    // line structs
    line_header   l_header; // line header
    line_register l_data;   // line data register

    // reads both files' header's data
    v_header = read_vehicle_header(f_vehicles);
    l_header = read_line_header(f_lines);

    // if either file has no register, raises error
    if(!v_header.nroRegistros || !l_header.nroRegistros){
        raise_error("Registro inexistente.");
    }

    // if any matching pair was found
    int found_any = 0;

    // reads and prints each matching register
    // outer loop (through vehicles)
    int i = 0;
    while(i < v_header.nroRegistros){
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

        // only search for this vehicle's line if it has any
        if(v_data.codLinha != -1){
            // takes the cursor at the lines' file to the first
            // data register's start (if it's not already there)
            if(ftell(f_lines) != LINE_HEADER_LENGTH){
                fseek(f_lines, LINE_HEADER_LENGTH, SEEK_SET);
            }

            // inner loop (through lines)
            int j = 0;
            while(j < l_header.nroRegistros){
                // reads the current register's "removido" and "tamanhoRegistro" fields
                fread(&l_data.removido,        sizeof(char), 1, f_lines);
                fread(&l_data.tamanhoRegistro, sizeof(int),  1, f_lines);

                // if the current register was removed, it'll not be printed
                if(l_data.removido == '0'){
                    fseek(f_lines, l_data.tamanhoRegistro, SEEK_CUR);
                    continue;
                }

                // reads the current register's remaining fixed size fields
                fread(&l_data.codLinha,     sizeof(int),  1, f_lines);
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

                    // prints the data
                    print_vehicle(v_header, v_data, 0);
                    print_line(l_header, l_data, 1);

                    // frees allocated line strings
                    free(l_data.nomeLinha);
                    free(l_data.corLinha);

                    found_any = 1;

                    break;
                }

                // if it isn't, goes to next line register (-5 is because
                // of the already read codLinha and aceitaCartao field)
                else{ fseek(f_lines, l_data.tamanhoRegistro - 5, SEEK_CUR); }

                // increments j
                j++;
            }
        }

        // frees allocated vehicle strings
        free(v_data.modelo);
        free(v_data.categoria);

        // increments i
        i++;
    }

    // if no matching pair was found
    if(!found_any){ raise_error("Registro inexistente."); }

    return;
}

void single_loop_join(char *vehiclesFilename, char *linesFilename, char *linesBtreeFilename){
    FILE *f_vehicles = open_validate_binary(vehiclesFilename, "rb");
    FILE *f_lines = open_validate_binary(linesFilename, "rb");
    FILE *f_btree = open_validate_binary(linesBtreeFilename, "rb");

    // vehicle structs
    vehicle_header   v_header; // vehicle header
    vehicle_register v_data;   // vehicle data register

    // line structs
    line_header   l_header; // line header
    line_register l_data;   // line data register

    // reads both files' header's data
    v_header = read_vehicle_header(f_vehicles);
    l_header = read_line_header(f_lines);

    // if either file has no register, raises error
    if(!v_header.nroRegistros || !l_header.nroRegistros){
        raise_error("Registro inexistente.");
    }

    // if any matching pair was found
    int found_any = 0;

    // reads and prints each matching register
    // outer loop (through vehicles)
    int i = 0;
    while(i < v_header.nroRegistros){
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

        // searches for the code on the btree
        // and recovers the register's byte offset
        // (or -1 if it doesn't exist)
        long long byte_offset = __btree_search(v_data.codLinha, f_btree);

        // if the line was found
        if(byte_offset != -1){
            found_any = 1;

            // goes to the start of the line codLinha field
            // (not necessary to check if it's removed cause
            // removed registers do not appear on the btree)
            fseek(f_lines, byte_offset + 5, SEEK_SET);

            // reads the current line's fixed size fields
            fread(&l_data.codLinha,     sizeof(int),  1, f_lines);
            fread(&l_data.aceitaCartao, sizeof(char), 1, f_lines);

            // reads the current line's "nomeLinha" field (variable size)
            fread(&l_data.tamanhoNome, sizeof(int),  1,                  f_lines);
            l_data.nomeLinha = (char *)malloc(sizeof(char) * l_data.tamanhoNome);
            fread(l_data.nomeLinha,    sizeof(char), l_data.tamanhoNome, f_lines);

            // reads the current line's "corLinha" field (variable size)
            fread(&l_data.tamanhoCor, sizeof(int),  1,                     f_lines);
            l_data.corLinha = (char *)malloc(sizeof(char) * l_data.tamanhoCor);
            fread(l_data.corLinha,    sizeof(char), l_data.tamanhoCor, f_lines);

            // prints the data
            print_vehicle(v_header, v_data, 0);
            print_line(l_header, l_data, 1);

            // frees allocated line strings
            free(l_data.nomeLinha);
            free(l_data.corLinha);
        }

        // frees allocated vehicle strings
        free(v_data.modelo);
        free(v_data.categoria);

        // increments i
        i++;
    }

    // if no matching pair was found
    if(!found_any){ raise_error("Registro inexistente."); }

    return;
}

void sorted_interpolation_join(char *vehiclesFilename, char *linesFilename){
}

