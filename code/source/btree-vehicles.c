/** AUTHORS:
 * Lucas Viana Vilela 10748409
 * Stéfane Tame Monteiro Oliveira 10829970
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../structs/btree.c"
#include "../structs/vehicle.c"
#include "../headers/util.h"
#include "../headers/vehicles.h"
#include "../headers/btree-vehicles.h"
#include "../headers/btree.h"

void create_vehicle_btree(char *vehiclesFilename, char *btreeFilename) {
    // string that has the .bin filepath (inside the "binaries" directory)
    char *vehicles_filepath = get_filepath(vehiclesFilename, 'b');
    char *btree_filepath = get_filepath(btreeFilename, 'b');

    // opens the vehicles file in reading-binary mode
    FILE *f_vehicles = fopen(vehicles_filepath, "rb"); // vehicles file (rb)

    // if the file does not exist, raises error and exists program
    if(!f_vehicles) { raise_error(""); }

    // vehicle structs (leading underscore in order
    // to distinguish it from the typedef'd struct)
    vehicle_header   _vehicle_header; // vehicle header struct
    vehicle_register _vehicle_data;   // vehicle register struct

    // reads header status from vehicles file and if the
    // file is inconsistent, raises error and exists program
    fread(&_vehicle_header.status, sizeof(char), 1, f_vehicles);
    if(_vehicle_header.status != '1'){ raise_error(""); }

    // opens the btree file in binary-writing mode
    FILE *f_btree = fopen(btree_filepath, "wb+"); // btree file (wb)

    // if the file could not be created, raises error and exists program
    if(!f_btree) { raise_error(""); }

    // vehicle header struct (leading underscore in order
    // to distinguish it from the typedef'd struct)
    btree_header _btree_header;

    // sets the btree header's starting values
    _btree_header.status = '0';
    _btree_header.noRaiz = -1;
    _btree_header.RRNproxNo = 0;
    memset(&(_btree_header.lixo), '@', 68); // @@@@@@ ...

    // writes btree header to disk
    fwrite(&_btree_header.status    , sizeof(char) , 1  , f_btree);
    fwrite(&_btree_header.noRaiz    , sizeof(int)  , 1  , f_btree);
    fwrite(&_btree_header.RRNproxNo , sizeof(int)  , 1  , f_btree);
    fwrite(_btree_header.lixo       , sizeof(char) , 68 , f_btree);

    // reads the vehicles file's header's remaining relevant fields
    fread(&_vehicle_header.byteProxReg  , sizeof(long long) , 1 , f_vehicles);
    fread(&_vehicle_header.nroRegistros , sizeof(int)       , 1 , f_vehicles);

    // moves to the start of the first vehicle data register
    fseek(f_vehicles, 175, SEEK_SET);

    // reads each register from the vehicles
    // file and inserts it in the btree
    int index = 0;
    while (index < _vehicle_header.nroRegistros) {
        // current vehicle register's btye offset
        long byte_offset = ftell(f_vehicles);

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

        // inserts the current vehicle to the btree
        __btree_insert(
            converted_prefix,
            byte_offset,
            f_btree,
            &_btree_header.noRaiz,
            &_btree_header.RRNproxNo
        );

        // increments index
        index++;
    }

    // writes the btree's root node rrn and next free rrn
    fseek(f_btree, 1, SEEK_SET);
    fwrite(&_btree_header.noRaiz    , sizeof(int)  , 1  , f_btree);
    fwrite(&_btree_header.RRNproxNo , sizeof(int)  , 1  , f_btree);

    // sets the btree header "status" field to 1
    _btree_header.status = '1';
    fseek(f_btree, 0, SEEK_SET);
    fwrite(&_btree_header.status, sizeof(char), 1, f_btree);

    // closes files and frees allocated strings
    fclose(f_vehicles);
    fclose(f_btree);
    free(btree_filepath);
    free(vehicles_filepath);

    return;
}

void search_vehicle_btree(char *vehiclesFilename, char *btreeFilename, char *prefix) {
    // string that has the .bin filepath (inside the "binaries" directory)
    char *btree_filepath = get_filepath(btreeFilename, 'b');

    // opens the btree file in binary-reading mode
    FILE *f_btree = fopen(btree_filepath, "rb"); // btree file (rb)

    // if the file could not be created, raises error and exists program
    if(!f_btree) { raise_error(""); }

    // btree file header's status field
    char btree_status;

    // reads header status from btree file and if the
    // file is inconsistent, raises error and exists program
    fread(&btree_status, sizeof(char), 1, f_btree);
    if(btree_status != '1'){ raise_error(""); }

    // convers the current vehicle's prefix to an integer
    int converted_prefix = convertePrefixo(prefix);

    // searches for the prefix on the btree
    // and recovers the register's byte offset
    // (or -1 if it doesn't exist)
    long long byte_offset = __btree_search(converted_prefix, f_btree);

    // if that register doesn't exist, raises error
    if(byte_offset == -1){ raise_error("Registro inexistente."); }

    // string that has the .bin filepath (inside the "binaries" directory)
    char *vehicles_filepath = get_filepath(vehiclesFilename, 'b');

    // opens the vehicles file in reading-binary mode
    FILE *f_vehicles = fopen(vehicles_filepath, "rb"); // vehicles file (rb)

    // if the file does not exist, raises error and exists program
    if(!f_vehicles) { raise_error(""); }

    // vehicle structs (leading underscore in order
    // to distinguish it from the typedef'd struct)
    vehicle_header   _vehicle_header; // vehicle header struct
    vehicle_register _vehicle_data;   // vehicle register struct

    // reads header status from vehicles file and if the
    // file is inconsistent, raises error and exists program
    fread(&_vehicle_header.status, sizeof(char), 1, f_vehicles);
    if(_vehicle_header.status != '1'){ raise_error(""); }

    // reads the _vehicle_header's byteProxReg and nroRegistros
    fread(&_vehicle_header.byteProxReg,  sizeof(long long), 1, f_vehicles);
    fread(&_vehicle_header.nroRegistros, sizeof(int),       1, f_vehicles);

    // if there are no registers, raises error
    if(!_vehicle_header.nroRegistros){ raise_error(""); }

    // reads the header's remaining fields
    fread(&_vehicle_header.nroRegRemovidos,  sizeof(int),  1,  f_vehicles);
    fread(_vehicle_header.descrevePrefixo,   sizeof(char), 18, f_vehicles);
    fread(_vehicle_header.descreveData,      sizeof(char), 35, f_vehicles);
    fread(_vehicle_header.descreveLugares,   sizeof(char), 42, f_vehicles);
    fread(_vehicle_header.descreveLinha,     sizeof(char), 26, f_vehicles);
    fread(_vehicle_header.descreveModelo,    sizeof(char), 17, f_vehicles);
    fread(_vehicle_header.descreveCategoria, sizeof(char), 20, f_vehicles);

    // goes to the start of the register's prefix field
    // (not necessary to check if it's removed cause
    // removed registers do not appear on the btree)
    fseek(f_vehicles, byte_offset + 5, SEEK_SET);

    // reads the current register's fixed size fields
    fread(_vehicle_data.prefixo,            sizeof(char), 5,  f_vehicles);
    fread(_vehicle_data.data,               sizeof(char), 10, f_vehicles);
    fread(&_vehicle_data.quantidadeLugares, sizeof(int),  1,  f_vehicles);
    fread(&_vehicle_data.codLinha,          sizeof(int),  1,  f_vehicles);

    // reads the current register's "modelo" field (variable size)
    fread(&_vehicle_data.tamanhoModelo, sizeof(int),  1,                           f_vehicles);
    _vehicle_data.modelo = (char *)malloc(sizeof(char) * _vehicle_data.tamanhoModelo);
    fread(_vehicle_data.modelo,         sizeof(char), _vehicle_data.tamanhoModelo, f_vehicles);

    // reads the current register's "categoria" field (variable size)
    fread(&_vehicle_data.tamanhoCategoria, sizeof(int),  1,                              f_vehicles);
    _vehicle_data.categoria = (char *)malloc(sizeof(char) * _vehicle_data.tamanhoCategoria);
    fread(_vehicle_data.categoria,         sizeof(char), _vehicle_data.tamanhoCategoria, f_vehicles);

    // gets formatted date or null message
    char *date = _vehicle_data.data[0] != '\0' ? format_date(_vehicle_data.data) : "campo com valor nulo";

    // prints the current register's fields
    print_string_field(_vehicle_header.descrevePrefixo   , 18 , _vehicle_data.prefixo   , 5);
    print_string_field(_vehicle_header.descreveModelo    , 17 , _vehicle_data.modelo    , _vehicle_data.tamanhoModelo);
    print_string_field(_vehicle_header.descreveCategoria , 20 , _vehicle_data.categoria , _vehicle_data.tamanhoCategoria);
    print_string_field(_vehicle_header.descreveData      , 35 , date                    , strlen(date));
    print_int_field(_vehicle_header.descreveLugares, 42, _vehicle_data.quantidadeLugares);

    // prints newline
    printf("\n");

    // frees the date string if necessary
    if(_vehicle_data.data[0] != '\0' ){ free(date); }

    // frees allocated strings
    free(_vehicle_data.modelo);
    free(_vehicle_data.categoria);

    // closes files and frees allocated strings
    fclose(f_vehicles);
    fclose(f_btree);
    free(btree_filepath);
    free(vehicles_filepath);

    return;
}

