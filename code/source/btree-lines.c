/** AUTHORS:
 * Lucas Viana Vilela 10748409
 * Stéfane Tame Monteiro Oliveira 10829970
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../structs/btree.c"
#include "../structs/line.c"
#include "../headers/util.h"
#include "../headers/lines.h"
#include "../headers/btree-lines.h"
#include "../headers/btree.h"

void create_line_btree(char *linesFilename, char *btreeFilename) {
    // string that has the .bin filepath (inside the "binaries" directory)
    char *lines_filepath = get_filepath(linesFilename, 'b');
    char *btree_filepath = get_filepath(btreeFilename, 'b');

    // opens the lines file in reading-binary mode
    FILE *f_lines = fopen(linesFilename, "rb"); // lines file (rb)

    // if the file does not exist, raises error and exists program
    if (!f_lines) { raise_error(""); }

    // line structs (leading underscore in order
    // to distinguish it from the typedef'd struct)
    line_header   _line_header; // line header struct
    line_register _line_data;   // line register struct

    // reads header status from lines file and if the
    // file is inconsistent, raises error and exists program
    fread(&_line_header.status, sizeof(char), 1, f_lines);
    if (_line_header.status != '1') {
        raise_error("");
    }

    // opens the btree file in binary-writing mode
    FILE *f_btree = fopen(btreeFilename, "wb"); // btree file (wb)

    // if the file could not be created, raises error and exists program
    if (!f_btree) { raise_error(""); }

    // btree structs (leading underscore in order
    // to distinguish it from the typedef'd struct)
    btree_header _btree_header; // line header struct
    btree_page _btree_node;     // line register struct

    // sets the btree header's starting values
    _btree_header.status = '0';
    _btree_header.noRaiz = -1;
    _btree_header.RRNproxNo = 1;
    memset(&(_btree_header.lixo), '@', 68); // @@@@@@ ...

    // writes btree header to disk
    fwrite(&_btree_header.status    , sizeof(char) , 1  , f_btree);
    fwrite(&_btree_header.noRaiz    , sizeof(int)  , 1  , f_btree);
    fwrite(&_btree_header.RRNproxNo , sizeof(int)  , 1  , f_btree);
    fwrite(_btree_header.lixo       , sizeof(char) , 68 , f_btree);

    // reads the lines file's header's remaining relevant fields
    fread(&_line_header.byteProxReg  , sizeof(long long) , 1 , f_lines);
    fread(&_line_header.nroRegistros , sizeof(int)       , 1 , f_lines);

    // moves to the start of the first line data register
    fseek(f_lines, 82, SEEK_SET);

    // reads each register from the lines
    // file and inserts it in the btree
    int index = 0;
    while (index < _line_header.nroRegistros) {
        // current line register's btye offset
        long byte_offset = ftell(f_lines);

        // reads the current register's "removido" and "tamanhoRegistro" fields
        fread(&_line_data.removido, sizeof(char), 1, f_lines);
        fread(&_line_data.tamanhoRegistro, sizeof(int), 1, f_lines);

        // if the current register was removed, it'll not be inserted
        if (_line_data.removido == '0') {
            fseek(f_lines, _line_data.tamanhoRegistro, SEEK_CUR);
            continue;
        }

        // reads the current line's prefix and skips to the next register
        fread(&_line_data.codLinha, sizeof(int), 1, f_lines);
        fseek(f_lines, _line_data.tamanhoRegistro - sizeof(int), SEEK_CUR);

        // inserts the current line to the btree
        __btree_insert(_line_data.codLinha, byte_offset, f_btree);

        // increments index
        index++;
    }

    // sets the header "status" field to 1
    _btree_header.status = '1';
    fseek(f_lines, 0, SEEK_SET);
    fwrite(&_btree_header.status, sizeof(char), 1, f_btree);

    // closes files
    fclose(f_lines);
    fclose(f_btree);
}

void search_line_btree(char *linesFilename, char *btreeFilename, int code) {
    // string that has the .bin filepath (inside the "binaries" directory)
    char *btree_filepath = get_filepath(btreeFilename, 'b');

    // opens the btree file in binary-reading mode
    FILE *f_btree = fopen(btreeFilename, "rb"); // btree file (rb)

    // if the file could not be created, raises error and exists program
    if(!f_btree) { raise_error(""); }

    // searches for the code on the btree
    // and recovers the register's byte offset
    // (or -1 if it doesn't exist)
    long long byte_offset = __btree_search(code, f_btree);

    // if that register doesn't exist, raises error
    if(byte_offset == -1){ raise_error("Registro inexistente."); }

    // string that has the .bin filepath (inside the "binaries" directory)
    char *lines_filepath = get_filepath(linesFilename, 'b');

    // opens the lines file in reading-binary mode
    FILE *f_lines = fopen(linesFilename, "rb"); // lines file (rb)

    // if the file does not exist, raises error and exists program
    if(!f_lines) { raise_error(""); }

    // line structs (leading underscore in order
    // to distinguish it from the typedef'd struct)
    line_header   _line_header; // line header struct
    line_register _line_data;   // line register struct

    // reads header status from lines file and if the
    // file is inconsistent, raises error and exists program
    fread(&_line_header.status, sizeof(char), 1, f_lines);
    if(_line_header.status != '1'){ raise_error(""); }

    // reads the _line_header's byteProxReg and nroRegistros
    fread(&_line_header.byteProxReg,  sizeof(long long), 1, f_lines);
    fread(&_line_header.nroRegistros, sizeof(int),       1, f_lines);

    // if there are no registers, raises error
    if(!_line_header.nroRegistros){ raise_error(""); }

    // reads the header's remaining fields
    fread(&_line_header.nroRegRemovidos, sizeof(int),  1,  f_lines);
    fread(_line_header.descreveCodigo,   sizeof(char), 15, f_lines);
    fread(_line_header.descreveCartao,   sizeof(char), 13, f_lines);
    fread(_line_header.descreveNome,     sizeof(char), 13, f_lines);
    fread(_line_header.descreveCor,      sizeof(char), 24, f_lines);

    // goes to the start of the register's code field
    // (not necessary to check if it's removed cause
    // removed registers do not appear on the btree)
    fseek(f_lines, byte_offset + 5, SEEK_SET);

    // reads the current register's fixed size fields
    fread(&_line_data.codLinha,     sizeof(int),  1, f_lines);
    fread(&_line_data.aceitaCartao, sizeof(char), 1, f_lines);

    // reads the current register's "nomeLinha" field (variable size)
    fread(&_line_data.tamanhoNome, sizeof(int),  1,                      f_lines);
    _line_data.nomeLinha = (char *)malloc(sizeof(char) * _line_data.tamanhoNome);
    fread(_line_data.nomeLinha,    sizeof(char), _line_data.tamanhoNome, f_lines);

    // reads the current register's "corLinha" field (variable size)
    fread(&_line_data.tamanhoCor, sizeof(int),  1,                     f_lines);
    _line_data.corLinha = (char *)malloc(sizeof(char) * _line_data.tamanhoCor);
    fread(_line_data.corLinha,    sizeof(char), _line_data.tamanhoCor, f_lines);

    // gets formatted card status
    char *card_status = format_card(_line_data.aceitaCartao);

    // prints the current register's fields
    print_int_field(_line_header.descreveCodigo, 15, _line_data.codLinha);
    print_string_field(_line_header.descreveNome   , 13 , _line_data.nomeLinha , _line_data.tamanhoNome);
    print_string_field(_line_header.descreveCor    , 24 , _line_data.corLinha  , _line_data.tamanhoCor);
    print_string_field(_line_header.descreveCartao , 13 , card_status          , strlen(card_status));

    // prints newline
    printf("\n");

    // frees allocated strings
    free(_line_data.nomeLinha);
    free(_line_data.corLinha);

    // closes files
    fclose(f_lines);
    fclose(f_btree);

    return;
}

