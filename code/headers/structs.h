#ifndef STRUCTS.H
#define STRUCTS.H

#define VEHICLE_HEADER_LENGTH 175
#define VECHILE_DATA_STATIC_LENGTH 31

typedef struct vehicle_ vehicle;

vehicle *new_vehicle_file();

// setters
void vehicle_set_header_status(vehicle *vehicle_file, char status);
void vehicle_set_header_byteProxReg(vehicle *vehicle_file, long long byteProxReg);
void vehicle_set_header_nroRegistros(vehicle *vehicle_file, int nroRegistros);
void vehicle_set_header_nroRegRemovidos(vehicle *vehicle_file, int nroRegRemovidos);
void vehicle_set_header_descrevePrefixo(vehicle *vehicle_file, char *descrevePrefixo);
void vehicle_set_header_descreveData(vehicle *vehicle_file, char *descreveData);
void vehicle_set_header_descreveLugares(vehicle *vehicle_file, char *descreveLugares);
void vehicle_set_header_descreveLinha(vehicle *vehicle_file, char *descreveLinha);
void vehicle_set_header_descreveModelo(vehicle *vehicle_file, char *descreveModelo);
void vehicle_set_header_descreveCategoria(vehicle *vehicle_file, char *descreveCategoria);

// getters
char vehicle_get_header_status(vehicle *vehicle_file);
long long vehicle_get_header_byteProxReg(vehicle *vehicle_file);
int vehicle_get_header_nroRegistros(vehicle *vehicle_file);
int vehicle_get_header_nroRegRemovidos(vehicle *vehicle_file);
char vehicle_get_header_descrevePrefixo(vehicle *vehicle_file);
char vehicle_get_header_descreveData(vehicle *vehicle_file);
char vehicle_get_header_descreveLugares(vehicle *vehicle_file);
char vehicle_get_header_descreveLinha(vehicle *vehicle_file);
char vehicle_get_header_descreveModelo(vehicle *vehicle_file);
char vehicle_get_header_descreveCategoria(vehicle *vehicle_file);

void vehicle_push_from_draft(vehicle *vehicle_file);

#endif