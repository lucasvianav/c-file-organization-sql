#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../headers/structs.h"

typedef struct {
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
} vehicle_header;

typedef struct {
    char removido;
    int tamanhoRegistro;
    char prefixo[5];
    char data[10];
    int quantidadeLugares;
    int codLinha;
    int tamanhoModelo;
    char *modelo; // variable size
    int tamanhoCategoria;
    char *categoria; // variable size
} vehicle_register;

struct vehicle_ {
    vehicle_header *header;
    vehicle_register *data;
    vehicle_register draft;
    int data_length;
};

vehicle *vehicle_new(){
    vehicle *vehicle_file = (vehicle *)malloc(sizeof(vehicle));

    vehicle_file->header = (vehicle_header *)malloc(sizeof(vehicle_header));
    vehicle_file->data = (vehicle_register *)malloc(0);
    vehicle_file->data_length = 0;

    return vehicle_file;
}

// header setters
void vehicle_set_header(vehicle *vehicle_file, char status, long long byteProxReg, int nroRegistros, int nroRegRemovidos, char *descrevePrefixo, char *descreveData, char *descreveLugares, char *descreveLinha, char *descreveModelo, char *descreveCategoria){
    vehicle_set_header_status(vehicle_file, status);
    vehicle_set_header_byteProxReg(vehicle_file, byteProxReg);
    vehicle_set_header_nroRegistros(vehicle_file, nroRegistros);
    vehicle_set_header_nroRegRemovidos(vehicle_file, nroRegRemovidos);
    vehicle_set_header_descrevePrefixo(vehicle_file, *descrevePrefixo);
    vehicle_set_header_descreveData(vehicle_file, *descreveData);
    vehicle_set_header_descreveLugares(vehicle_file, *descreveLugares);
    vehicle_set_header_descreveLinha(vehicle_file, *descreveLinha);
    vehicle_set_header_descreveModelo(vehicle_file, *descreveModelo);
    vehicle_set_header_descreveCategoria(vehicle_file, *descreveCategoria);
}
void vehicle_set_header_status(vehicle *vehicle_file, char status){ vehicle_file->header->status = status; }
void vehicle_set_header_byteProxReg(vehicle *vehicle_file, long long byteProxReg){ vehicle_file->header->byteProxReg = byteProxReg; }
void vehicle_set_header_nroRegistros(vehicle *vehicle_file, int nroRegistros){ vehicle_file->header->nroRegistros = nroRegistros; }
void vehicle_set_header_nroRegRemovidos(vehicle *vehicle_file, int nroRegRemovidos){ vehicle_file->header->nroRegRemovidos = nroRegRemovidos; }
void vehicle_set_header_descrevePrefixo(vehicle *vehicle_file, char *descrevePrefixo){ strcpy(vehicle_file->header->descrevePrefixo, descrevePrefixo); }
void vehicle_set_header_descreveData(vehicle *vehicle_file, char *descreveData){ strcpy(vehicle_file->header->descreveData, descreveData); }
void vehicle_set_header_descreveLugares(vehicle *vehicle_file, char *descreveLugares){ strcpy(vehicle_file->header->descreveLugares, descreveLugares); }
void vehicle_set_header_descreveLinha(vehicle *vehicle_file, char *descreveLinha){ strcpy(vehicle_file->header->descreveLinha, descreveLinha); }
void vehicle_set_header_descreveModelo(vehicle *vehicle_file, char *descreveModelo){ strcpy(vehicle_file->header->descreveModelo, descreveModelo); }
void vehicle_set_header_descreveCategoria(vehicle *vehicle_file, char *descreveCategoria){ strcpy(vehicle_file->header->descreveCategoria, descreveCategoria); }

// draft setters

void vehicle_increment_header_nroRegRemovidos(vehicle *vehicle_file){ ++(vehicle_file->header->nroRegRemovidos); }

// getters
char vehicle_get_header_status(vehicle *vehicle_file){ return vehicle_file->header->status; }
long long vehicle_get_header_byteProxReg(vehicle *vehicle_file){ return vehicle_file->header->byteProxReg; }
int vehicle_get_header_nroRegistros(vehicle *vehicle_file){ return vehicle_file->header->nroRegistros; }
int vehicle_get_header_nroRegRemovidos(vehicle *vehicle_file){ return vehicle_file->header->nroRegRemovidos; }
char vehicle_get_header_descrevePrefixo(vehicle *vehicle_file){ return vehicle_file->header->descrevePrefixo; }
char vehicle_get_header_descreveData(vehicle *vehicle_file){ return vehicle_file->header->descreveData; }
char vehicle_get_header_descreveLugares(vehicle *vehicle_file){ return vehicle_file->header->descreveLugares; }
char vehicle_get_header_descreveLinha(vehicle *vehicle_file){ return vehicle_file->header->descreveLinha; }
char vehicle_get_header_descreveModelo(vehicle *vehicle_file){ return vehicle_file->header->descreveModelo; }
char vehicle_get_header_descreveCategoria(vehicle *vehicle_file){ return vehicle_file->header->descreveCategoria; }

void vehicle_push_from_draft(vehicle *vehicle_file){
    // allocates a new register to the array
    vehicle_file->data = (vehicle_register *)realloc(vehicle_file->data, ++(vehicle_file->data_length) * sizeof(vehicle_register));
    vehicle_file->data[vehicle_file->data_length-1] = vehicle_file->draft;
}

