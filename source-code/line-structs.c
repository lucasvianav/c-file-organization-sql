#define LINE_HEADER_LENGTH 82
#define LINE_DATA_STATIC_LENGTH 13

typedef struct {
    char status;
    long long byteProxReg;
    int nroRegistros;
    int nroRegRemovidos;
    char descreveCodigo[15];
    char descreveCartao[13];
    char descreveNome[13];
    char descreveCor[24];
} line_header;

typedef struct {
    char removido;
    int tamanhoRegistro;
    int codLinha;
    char aceitaCartao;
    int tamanhoNome;
    char *nomeLinha; // variable size
    int tamanhoCor;
    char *corLinha; // variable size
} line_register;

typedef struct {
    line_header *header;
    line_register *data;
} line;