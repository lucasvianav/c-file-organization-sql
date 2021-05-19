#define VEHICLE_HEADER_LENGTH 175
#define VECHILE_DATA_STATIC_LENGTH 36

typedef struct {
    char status;
    long byteProxReg;
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
    char *modelo; // tamanho variável
    int tamanhoCategoria;
    char *categoria; // tamanho variável
} vehicle_data;