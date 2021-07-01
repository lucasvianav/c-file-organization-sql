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

// hey!!!
typedef struct {
    char folha; // "bool"
    int nroChavesIndexadas; // 0 <= x <= 4
    int RRNdoNo; // this node's RRN
    int P1; // "pointer" to the child node
    int C1; // search-key
    long long Pr1; // "pointer" to the data file reference
    int P2; // "pointer" to the child node
    int C2; // search-key
    long long Pr2; // "pointer" to the data file reference
    int P3; // "pointer" to the child node
    int C3; // search-key
    long long Pr3; // "pointer" to the data file reference
    int P4; // "pointer" to the child node
    int C4; // search-key
    long long Pr4; // "pointer" to the data file reference
    int P5; // "pointer" to the child node
} btree_page;

typedef struct {
    btree_header *header;
    btree_page *data;
    int tree_length;
} btree;

