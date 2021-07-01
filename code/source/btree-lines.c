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

        // // inserts the current line to the btree
        // PROMO_R_CHILD E PROMO_KEY E PON (?)
        // __btree_insert(btree_header.noRaiz, prefixo_int, PROMO_R_CHILD,
        // PROMO_KEY, PON, arq2);

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
    _btree_header.status = '1';
    fseek(f_lines, 0, SEEK_SET);
    fwrite(&_btree_header.status, sizeof(char), 1, f_btree);

    // closes files
    fclose(f_lines);
    fclose(f_btree);
}

void search_line_btree(char *arquivoVeiculo, char *arquivoArvore, char *prefixo, int valor){
    // A manipulação do arquivo de índice árvore-B deve ser feita em disco, de acordo com o conteúdo ministrado em sala de aula.
    //Falha no processamento do arquivo.

    FILE *arq1, *arq2;
    line header;
    line_register data;
    btree_header headerT;

    // opens the file in arq1-reading mode
    arq1 = fopen(arquivoVeiculo, "rb");
    arq2 = fopen(arquivoArvore, "wb");

    // if the file does not exist, raise error
    if((arq1 == NULL) || (arq2 == NULL)){ raise_error(""); }


    // procura na arvore
    fread(&headerT.status, sizeof(char), 1, arq2);
    if(headerT.status == '0'){ raise_error(""); }

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
        fread(header.descreveCodigo, sizeof(char), 15, arq1);
        fread(header.descreveCartao, sizeof(char), 13, arq1);
        fread(header.descreveNome, sizeof(char), 13, arq1);
        fread(header.descreveCor, sizeof(char), 24, arq1);

        fseek(arq1, achou_Pr, SEEK_SET);

        // reads the current register's "removido" and "tamanhoRegistro" fields
        fread(&data.removido, sizeof(char), 1, arq1);
        fread(&data.tamanhoRegistro, sizeof(int), 1, arq1);

        // if the current register was removed, it'll not be printed
        if(data.removido == '0'){
            fseek(arq1, data.tamanhoRegistro, SEEK_CUR);
            continue;
        }

        // reads the current register's remaining fixed size fields
        fread(&data.codLinha, sizeof(int), 1, arq1);
        fread(&data.aceitaCartao, sizeof(char), 1, arq1);

        // reads the current register's "nomeLinha" field (variable size)
        fread(&data.tamanhoNome, sizeof(int), 1, arq1);
        data.nomeLinha = (char *)malloc(sizeof(char) * data.tamanhoNome);
        fread(data.nomeLinha, sizeof(char), data.tamanhoNome, arq1);

        // reads the current register's "corLinha" field (variable size)
        fread(&data.tamanhoCor, sizeof(int), 1, arq1);
        data.corLinha = (char *)malloc(sizeof(char) * data.tamanhoCor);
        fread(data.corLinha, sizeof(char), data.tamanhoCor, arq1);

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
    }
    else{
        printf("Registro inexistente.\n");
    }

    fclose(arq1);
    fclose(arq2);
}

