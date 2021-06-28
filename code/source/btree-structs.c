/** AUTHORS:
 * Lucas Viana Vilela 10748409
 * Stéfane Tame Monteiro Oliveira 10829970
*/

#define DISK_PAGE_LENGTH 77
#define NO_PROMOTION 0
#define PROMOTION 1

typedef struct {
    char status;
    int noRaiz; // starting value is -1
    int RRNproxNo;
    char lixo[68]; // @
} btree_header;

typedef struct {
    char folha; // "bool"
    int nroChavesIndexadas; // 0 <= x <= 4
    int RRNdoNo;
    int P1;
    int C1;
    long long Pr1;
    int P2;
    int C2;
    long long Pr2;
    int P3;
    int C3;
    long long Pr3;
    int P4;
    int C4;
    long long Pr4;
    int P5;
} btree_page;

typedef struct {
    btree_header *header;
    btree_page *data;
    int tree_length;
} btree;
