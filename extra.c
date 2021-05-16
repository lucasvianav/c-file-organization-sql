#include <stdio.h>
#include <stdlib.h>

//Nome do programa: programaTrab
//LIXO: @
//MODULARIZAR!!!

typedef struct Cabecalho1{ // total: 175 Bytes
    char status;
    long long byteProxReg;
    int nroRegistros;
    int nroRegRemovidos;
    char descrevePrefixo[18];
    char descreveData[35];
    char descreveLugares[42];
    char descreveLinha[26];
    char descreveModelo[17];
    char descreveCategoria[20];
} cabecalho_veiculo;

typedef struct Dados1{
    char removido;
    int tamanhoRegistro;
    char prefixo[5];
    char data[10];
    int quantidadeLugares;
    int codLinha;
    //tamanho variavel (metodo de indicador de tamanho):
    int tamanhoModelo;
    char *modelo;
    int tamanhoCategoria;
    char *categoria;
} dados_veiculo;

typedef struct Cabecalho2{ // total: 82 Bytes
    char status;
    long long byteProxReg;
    int nroRegistros;
    int nroRegRemovidos;
    char descreveCodigo[15];
    char descreveCartao[13];
    char descreveNome[13];
    char descreveCor[24];
} cabecalho_linha;

typedef struct Dados2{
    char removido;
    int tamanhoRegistro;
    int codLinha;
    char aceitaCartao;
    //tamanho variavel (metodo de indicador de tamanho):
    int tamanhoNome;
    char *nomeLinha;
    int tamanhoCor;
    char *corLinha;
} dados_linha;