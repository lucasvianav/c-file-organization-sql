/** AUTHORS:
 * Lucas Viana Vilela 10748409
 * Stéfane Tame Monteiro Oliveira 10829970
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../structs/btree.c"
#include "../headers/util.h"
#include "../headers/vehicles.h"
#include "../headers/lines.h"

#define INVALID -1

typedef struct {
    int key;
    long long reference;
    int child_node_rrn;
} promotion_info;

// goes to specified RRN's position in file if it's
// not already on it and returns that position
long fseek_rrn(FILE *file, int rrn) {
    // the current node's position on the btree file
    // (the +1 considers the header register)
    long current_node_position = (rrn + 1) * DISK_PAGE_LENGTH;

    // if it's not on the right position, goes to it
    if(ftell(file) != current_node_position){ fseek(file, current_node_position, SEEK_SET); }

    return current_node_position;
}

// returns the root rrn for a btree file
int get_root_rrn(FILE *file){
    // btree's root RRN
    int root_rrn;

    // reads the btree's root RRN
    fseek(file, sizeof(char), SEEK_SET);
    fread(&root_rrn, sizeof(int), 1, file);

    return root_rrn;
}

// receives a node and a file and writes it to disk
// at the corret position (fseeks based on it's RRN)
void fwrite_node(btree_page node, FILE *file){
    // goes to the node's position on the btree file
    fseek_rrn(file, node.RRNdoNo);

    // writes the node to disk
    fwrite(&node.folha,              sizeof(char),      1, file);
    fwrite(&node.nroChavesIndexadas, sizeof(int),       1, file);
    fwrite(&node.RRNdoNo,            sizeof(int),       1, file);
    fwrite(&node.P1,                 sizeof(int),       1, file);
    fwrite(&node.C1,                 sizeof(int),       1, file);
    fwrite(&node.Pr1,                sizeof(long long), 1, file);
    fwrite(&node.P2,                 sizeof(int),       1, file);
    fwrite(&node.C2,                 sizeof(int),       1, file);
    fwrite(&node.Pr2,                sizeof(long long), 1, file);
    fwrite(&node.P3,                 sizeof(int),       1, file);
    fwrite(&node.C3,                 sizeof(int),       1, file);
    fwrite(&node.Pr3,                sizeof(long long), 1, file);
    fwrite(&node.P4,                 sizeof(int),       1, file);
    fwrite(&node.C4,                 sizeof(int),       1, file);
    fwrite(&node.Pr4,                sizeof(long long), 1, file);
    fwrite(&node.P5,                 sizeof(int),       1, file);
}

// receives a node's RRN and a file,
// reads and returns that node
btree_page fread_node(int rrn, FILE *file){
    btree_page node;

    // goes to the node's position on the btree file
    fseek_rrn(file, rrn);

    // writes the node to disk
    fread(&node.folha,              sizeof(char),      1, file);
    fread(&node.nroChavesIndexadas, sizeof(int),       1, file);
    fread(&node.RRNdoNo,            sizeof(int),       1, file);
    fread(&node.P1,                 sizeof(int),       1, file);
    fread(&node.C1,                 sizeof(int),       1, file);
    fread(&node.Pr1,                sizeof(long long), 1, file);
    fread(&node.P2,                 sizeof(int),       1, file);
    fread(&node.C2,                 sizeof(int),       1, file);
    fread(&node.Pr2,                sizeof(long long), 1, file);
    fread(&node.P3,                 sizeof(int),       1, file);
    fread(&node.C3,                 sizeof(int),       1, file);
    fread(&node.Pr3,                sizeof(long long), 1, file);
    fread(&node.P4,                 sizeof(int),       1, file);
    fread(&node.C4,                 sizeof(int),       1, file);
    fread(&node.Pr4,                sizeof(long long), 1, file);
    fread(&node.P5,                 sizeof(int),       1, file);

    return node;
}






promotion_info split(promotion_info inserted, btree_page current_node, FILE *file, int *free_rrn) {
    // like a disk page but with space for one extra key, child and pointer
    struct bigger_page {
        int P1;        // "pointer" to the child node
        int C1;        // search-key
        long long Pr1; // "pointer" to the data file reference
        int P2;        // "pointer" to the child node
        int C2;        // search-key
        long long Pr2; // "pointer" to the data file reference
        int P3;        // "pointer" to the child node
        int C3;        // search-key
        long long Pr3; // "pointer" to the data file reference
        int P4;        // "pointer" to the child node
        int C4;        // search-key
        long long Pr4; // "pointer" to the data file reference
        int P5;        // "pointer" to the child node
        int C5;        // search-key
        long long Pr5; // "pointer" to the data file reference
        int P6;        // "pointer" to the child node
    };
    struct bigger_page bigger_node;

    if (inserted.key < current_node.C1) {
        bigger_node.P1  = current_node.P1;

        bigger_node.C1  = inserted.key;
        bigger_node.Pr1 = inserted.reference;
        bigger_node.P2  = inserted.child_node_rrn;

        bigger_node.C2  = current_node.C1;
        bigger_node.Pr2 = current_node.Pr1;
        bigger_node.P3  = current_node.P2;

        bigger_node.C3  = current_node.C2;
        bigger_node.Pr3 = current_node.Pr2;
        bigger_node.P4  = current_node.P3;

        bigger_node.C4  = current_node.C3;
        bigger_node.Pr4 = current_node.Pr3;
        bigger_node.P5  = current_node.P4;

        bigger_node.C5  = current_node.C4;
        bigger_node.Pr5 = current_node.Pr4;
        bigger_node.P6  = current_node.P5;
    }

    else if (inserted.key < current_node.C2) {
        bigger_node.P1  = current_node.P1;

        bigger_node.C1  = current_node.C1;
        bigger_node.Pr1 = current_node.Pr1;
        bigger_node.P2  = current_node.P2;

        bigger_node.C2  = inserted.key;
        bigger_node.Pr2 = inserted.reference;
        bigger_node.P3  = inserted.child_node_rrn;

        bigger_node.C3  = current_node.C2;
        bigger_node.Pr3 = current_node.Pr2;
        bigger_node.P4  = current_node.P3;

        bigger_node.C4  = current_node.C3;
        bigger_node.Pr4 = current_node.Pr3;
        bigger_node.P5  = current_node.P4;

        bigger_node.C5  = current_node.C4;
        bigger_node.Pr5 = current_node.Pr4;
        bigger_node.P6  = current_node.P5;
    }

    else if (inserted.key < current_node.C3) {
        bigger_node.P1  = current_node.P1;

        bigger_node.C1  = current_node.C1;
        bigger_node.Pr1 = current_node.Pr1;
        bigger_node.P2  = current_node.P2;

        bigger_node.C2  = current_node.C2;
        bigger_node.Pr2 = current_node.Pr2;
        bigger_node.P3  = current_node.P3;

        bigger_node.C3  = inserted.key;
        bigger_node.Pr3 = inserted.reference;
        bigger_node.P4  = inserted.child_node_rrn;

        bigger_node.C4  = current_node.C3;
        bigger_node.Pr4 = current_node.Pr3;
        bigger_node.P5  = current_node.P4;

        bigger_node.C5  = current_node.C4;
        bigger_node.Pr5 = current_node.Pr4;
        bigger_node.P6  = current_node.P5;
    }

    else if (inserted.key < current_node.C4) {
        bigger_node.P1  = current_node.P1;

        bigger_node.C1  = current_node.C1;
        bigger_node.Pr1 = current_node.Pr1;
        bigger_node.P2  = current_node.P2;

        bigger_node.C2  = current_node.C2;
        bigger_node.Pr2 = current_node.Pr2;
        bigger_node.P3  = current_node.P3;

        bigger_node.C3  = current_node.C3;
        bigger_node.Pr3 = current_node.Pr3;
        bigger_node.P4  = current_node.P4;

        bigger_node.C4  = inserted.key;
        bigger_node.Pr4 = inserted.reference;
        bigger_node.P5  = inserted.child_node_rrn;

        bigger_node.C5  = current_node.C4;
        bigger_node.Pr5 = current_node.Pr4;
        bigger_node.P6  = current_node.P5;
    }

    else {
        bigger_node.P1  = current_node.P1;

        bigger_node.C1  = current_node.C1;
        bigger_node.Pr1 = current_node.Pr1;
        bigger_node.P2  = current_node.P2;

        bigger_node.C2  = current_node.C2;
        bigger_node.Pr2 = current_node.Pr2;
        bigger_node.P3  = current_node.P3;

        bigger_node.C3  = current_node.C3;
        bigger_node.Pr3 = current_node.Pr3;
        bigger_node.P4  = current_node.P4;

        bigger_node.C4  = current_node.C4;
        bigger_node.Pr4 = current_node.Pr4;
        bigger_node.P5  = current_node.P5;

        bigger_node.C5  = inserted.key;
        bigger_node.Pr5 = inserted.reference;
        bigger_node.P6  = inserted.child_node_rrn;
    }

    // current node's new data
    current_node.nroChavesIndexadas = 2;
    current_node.P1                 = bigger_node.P1;
    current_node.C1                 = bigger_node.C1;
    current_node.Pr1                = bigger_node.Pr1;
    current_node.P2                 = bigger_node.P2;
    current_node.C2                 = bigger_node.C2;
    current_node.Pr2                = bigger_node.Pr2;
    current_node.P3                 = bigger_node.P3;
    current_node.C3                 = INVALID;
    current_node.Pr3                = INVALID;
    current_node.P4                 = INVALID;
    current_node.C4                 = INVALID;
    current_node.Pr4                = INVALID;
    current_node.P5                 = INVALID;

    // new node's data
    btree_page new_node;
    new_node.folha              = current_node.folha;
    new_node.nroChavesIndexadas = 2;
    new_node.RRNdoNo            = *free_rrn;
    new_node.P1                 = bigger_node.P4;
    new_node.C1                 = bigger_node.C4;
    new_node.Pr1                = bigger_node.Pr4;
    new_node.P2                 = bigger_node.P5;
    new_node.C2                 = bigger_node.C5;
    new_node.Pr2                = bigger_node.Pr5;
    new_node.P3                 = bigger_node.P6;
    new_node.C3                 = INVALID;
    new_node.Pr3                = INVALID;
    new_node.P4                 = INVALID;
    new_node.C4                 = INVALID;
    new_node.Pr4                = INVALID;
    new_node.P5                 = INVALID;

    // rewrites the current node
    fwrite_node(current_node, file);

    // writes the new node to disk
    fwrite_node(new_node, file);

    // calculates the next free rrn
    (*free_rrn)++;

    // returns the promoted key
    return (promotion_info) { bigger_node.C3, bigger_node.Pr3, new_node.RRNdoNo };
}

// internal function that'll recursively insert a key to the tree
promotion_info recursive_insert(int current_rrn, int inserted_key, long long inserted_ref, FILE *file, int *free_rrn) {
    // base case - if the current node does not exist "promotes"
    // the inserted key (the past node was a leaf)
    if (current_rrn == INVALID) { return (promotion_info) { inserted_key, inserted_ref, INVALID }; }

    // current node's data
    btree_page node = fread_node(current_rrn, file);

    // if the inserted key already exists on the tree, raises error
    if ( node.C1 == inserted_key || node.C2 == inserted_key || node.C3 == inserted_key || node.C4 == inserted_key) { raise_error(""); }

    // finds the position in which the child node
    // in which the inserted key would enter
    int child_rrn;
    if (inserted_key < node.C1 || node.C1 == INVALID) { child_rrn = node.P1; }
    else if (inserted_key < node.C2 || node.C2 == INVALID) { child_rrn = node.P2; }
    else if (inserted_key < node.C3 || node.C3 == INVALID) { child_rrn = node.P3; }
    else if (inserted_key < node.C4 || node.C4 == INVALID) { child_rrn = node.P4; }
    else { child_rrn = node.P5; }

    promotion_info promotion = recursive_insert(child_rrn, inserted_key, inserted_ref, file, free_rrn);

    if(promotion.key == INVALID){ return promotion; }

    // if there is no space on the current node (split needed)
    else if(node.nroChavesIndexadas == 4) { return split(promotion, node, file, free_rrn); }

    // if no split is needed inserts the promoted key
    // to the current node while maintaining it sorted

    // if the promoted key will enter at position 1
    if(promotion.key < node.C1 ||  node.C1  == INVALID){
        // bumps all other keys one position forward
        node.C4  = node.C3;
        node.Pr4 = node.Pr3;
        node.P5  = node.P4;

        node.C3  = node.C2;
        node.Pr3 = node.Pr2;
        node.P4  = node.P3;

        node.C2  = node.C1;
        node.Pr2 = node.Pr1;
        node.P3  = node.P2;

        // and sets it to position 1 (maintaining the preceding child node)
        node.C1  = promotion.key;
        node.Pr1 = promotion.reference;
        node.P2  = promotion.child_node_rrn;
    }

    // if the promoted key will enter at position 2
    else if(promotion.key < node.C2 ||  node.C2  == INVALID){
        // bumps the next keys one position forward
        node.C4  = node.C3;
        node.Pr4 = node.Pr3;
        node.P5  = node.P4;

        node.C3  = node.C2;
        node.Pr3 = node.Pr2;
        node.P4  = node.P3;

        // and sets it to position 2 (maintaining the preceding child node)
        node.C2  = promotion.key;
        node.Pr2 = promotion.reference;
        node.P3  = promotion.child_node_rrn;
    }

    // if the promoted key will enter at position 3
    else if(promotion.key < node.C3 ||  node.C3  == INVALID){
        // bumps the next key one position forward
        node.C4  = node.C3;
        node.Pr4 = node.Pr3;
        node.P5  = node.P4;

        // and sets it to position 3 (maintaining the preceding child node)
        node.C3  = promotion.key;
        node.Pr3 = promotion.reference;
        node.P4  = promotion.child_node_rrn;
    }

    // if the promoted key will enter at position 4
    else {
        // sets it to position 4 (maintaining the preceding child node)
        node.C4  = promotion.key;
        node.Pr4 = promotion.reference;
        node.P5  = promotion.child_node_rrn;
    }

    // a new key was just indexed
    (node.nroChavesIndexadas)++;

    // rewrites the current node
    fwrite_node(node, file);

    // no promotion happened
    return (promotion_info) { INVALID, INVALID, INVALID };
}

// internal function that'll recursively search for a key on the tree
long long recursive_search(int current_rrn, int queried_key, FILE *file) {
    // base case - the current node does not exist
    if (current_rrn == INVALID) { return INVALID; }

    // current node's data
    btree_page node = fread_node(current_rrn, file);

    // searches the current node and it's children for the queried key

    if (queried_key <= node.C1) {
        return (queried_key == node.C1) ? node.Pr1 : recursive_search(node.P1, queried_key, file);
    }

    else if (queried_key <= node.C2) {
        return (queried_key == node.C2) ? node.Pr2 : recursive_search(node.P2, queried_key, file);
    }

    else if (queried_key <= node.C3) {
        return (queried_key == node.C3) ? node.Pr3 : recursive_search(node.P3, queried_key, file);
    }

    else if (queried_key <= node.C4) {
        return (queried_key == node.C4) ? node.Pr4 : recursive_search(node.P4, queried_key, file);
    }

    else {
        return recursive_search(node.P5, queried_key, file);
    }
}






void __btree_insert(int key, long long reference, FILE *file, int *root_rrn, int *free_rrn){
    // recursively inserts the key starting at the root
    promotion_info promotion = recursive_insert(*root_rrn, key, reference, file, free_rrn);

    // if a promotion was returned, creates a new root
    if(promotion.key != INVALID){
        // creates a new root node
        btree_page node;
        node.folha              = *root_rrn == INVALID ? '1' : '0';
        node.nroChavesIndexadas = 1;
        node.RRNdoNo            = *free_rrn;
        node.P1                 = *root_rrn;
        node.C1                 = promotion.key;
        node.Pr1                = promotion.reference;
        node.P2                 = promotion.child_node_rrn;
        node.C2                 = INVALID;
        node.Pr2                = INVALID;
        node.P3                 = INVALID;
        node.C3                 = INVALID;
        node.Pr3                = INVALID;
        node.P4                 = INVALID;
        node.C4                 = INVALID;
        node.Pr4                = INVALID;
        node.P5                 = INVALID;

        // writes the new node
        fwrite_node(node, file);

        // sets the node just created as the new root
        *root_rrn = node.RRNdoNo;

        // incremets the next free RRN
        (*free_rrn)++;
    }

    return;
}

long long __btree_search(int key, FILE *file) {
    // recursively searchs for the key starting at the root
    return recursive_search(get_root_rrn(file), key, file);
}

