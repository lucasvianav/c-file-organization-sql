#include "../headers/btree.h"
#include "../headers/btree-lines.h"

void create_tree_line(char *arquivoVeiculo, char *arquivoArvore){
    FILE *arq1, *arq2;
    line_header header;
    line_register data;
    btree_header headerT; // = (btree_header *)malloc(sizeof(btree_header));
    btree_page page;
    int prefixo_int;

    headerT.status = "0";
    headerT.noRaiz = -1;
    header.RRNproxNo = 1;
    for(int i = 0; i < 68; i++){
        header.lixo[i] = "@"
    }

    // opens the file in arq1-reading mode
    arq1 = fopen(arquivoVeiculo, "rb");
    arq2 = fopen(arquivoArvore, "wb");

    // if the file does not exist, raise error
    if((arq1 == NULL) || (arq2 == NULL)){ raise_error(""); }

    // if the file is inconsistent, raise error
    fread(&header.status, sizeof(char), 1, arq1);
    if(header.status == '0'){ raise_error(""); }

    // reads the header's byteProxReg and nroRegistros
    fread(&header.byteProxReg, sizeof(long long), 1, arq1);
    fread(&header.nroRegistros, sizeof(int), 1, arq1);

    // if there are no registers, raises error
    if(!header.nroRegistros){ raise_error("Registro inexistente."); }

    // reads the header's remaining fields
    // fread(&header.nroRegRemovidos, sizeof(int), 1, arq1);
    // fread(header.descreveCodigo, sizeof(char), 15, arq1);
    // fread(header.descreveCartao, sizeof(char), 13, arq1);
    // fread(header.descreveNome, sizeof(char), 13, arq1);
    // fread(header.descreveCor, sizeof(char), 24, arq1);
    fseek(arq1, 69, SEEK_CUR);

    //insere o cabeçalho da arvore
    fwrite(headerT.status, sizeof(char), 1, arq2);
    fwrite(headerT.noRaiz, sizeof(int), 1, arq2);
    fwrite(headerT.RRNproxNo, sizeof(int), 1, arq2);
    fwrite(headerT.lixo, sizeof(char), 68, arq2);


    // reads and prints each register
    int index = 0;
    while(index < header.nroRegistros){
        // reads the current register's "removido" and "tamanhoRegistro" fields
        fread(&data.removido, sizeof(char), 1, arq1);
        fread(&data.tamanhoRegistro, sizeof(int), 1, arq1);

        // if the current register was removed, it'll not be printed
        if(data.removido == '0'){
            fseek(arq1, data.tamanhoRegistro, SEEK_CUR);
            continue;
        }

        // reads the current register's remaining fixed size fields
        // fread(&data.codLinha, sizeof(int), 1, arq1);
        // fread(&data.aceitaCartao, sizeof(char), 1, arq1);

        // // reads the current register's "nomeLinha" field (variable size)
        // fread(&data.tamanhoNome, sizeof(int), 1, arq1);
        // data.nomeLinha = (char *)malloc(sizeof(char) * data.tamanhoNome);
        // fread(data.nomeLinha, sizeof(char), data.tamanhoNome, arq1);

        // // reads the current register's "corLinha" field (variable size)
        // fread(&data.tamanhoCor, sizeof(int), 1, arq1);
        // data.corLinha = (char *)malloc(sizeof(char) * data.tamanhoCor);
        // fread(data.corLinha, sizeof(char), data.tamanhoCor, arq1);
        fseek(arq1, data.tamanhoRegistro, SEEK_CUR);



        // PROMO_R_CHILD E PROMO_KEY (?)
        insert(headerT.noRaiz, data.codLinha, PROMO_R_CHILD, PROMO_KEY, PON, arq2);

        //atualizar o headerT.noRaiz
        fseek(arq2, 1, SEEK_SET);
        headerT.noRaiz = PROMO_R_CHILD; //(?)
        headerT.RRNproxNo = PROMO_R_CHILD + 1; //(?)
        fwrite(headerT.noRaiz, sizeof(int), 1, arq2);
        fwrite(headerT.RRNproxNo, sizeof(int), 1, arq2);


        // frees allocated strings
        // free(data.nomeLinha);
        // free(data.corLinha);

        // increments index
        index++;
    }


    // MUDAR O STATUS PARA 1
    fseek(arq1, 0, SEEK_SET);
    headerT.status = "1";
    fwrite(headerT.status, sizeof(char), 1, arq2);


    fclose(arq1);
    fclose(arq2);
}

void search_tree_line(char *arquivoVeiculo, char *arquivoArvore, char *prefixo, int valor){
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
