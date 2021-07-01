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

void split(int inserted_key, int I_RRN, int page, int PROMO_KEY, int PROMO_R_CHILD, int NEWPAGE, long long PON){
    //I_KEY -> nova chave a ser inserida
    //I_RRN-> filho a direita da nova chave a ser inserida
    //PAGE -> página de disco corrente
    //PROMO_KEY -> chave promovida
    //PROMO_R_CHILD -> filho a direita da chave promovida
    //NEWPAGE -> nova página de disco

    btree_page NEWPAGE;

    if(inserted_key < C1){
        NEWPAGE.C1 = page.C3
        NEWPAGE.Pr1 = page.Pr3
        NEWPAGE.P2 = page.P4
        NEWPAGE.C2 = page.C4
        NEWPAGE.Pr2 = page.Pr4
        NEWPAGE.P3 = page.P5

        page.P4 = page.P3;
        page.C3 = page.C2;
        page.Pr3 = page.Pr2;
        page.P3 = page.P2;
        page.C2 = page.C1;
        page.Pr2 = page.Pr1;
        page.P2 = page.P1;
        page.C1 = inserted_key;
        page.Pr1 = PON;
        page.P1 = -1;

        fseek(ARQ, CURRENT_RRN+1, SEEK_SET);
        fwrite(page.nroChavesIndexadas, sizeof(int), 1, ARQ);
        fseek(ARQ, 4, SEEK_CUR);
        fwrite(page.P1, sizeof(int), 1, ARQ);
        fwrite(page.C1, sizeof(int), 1, ARQ);
        fwrite(page.Pr1, sizeof(long long), 1, ARQ);
        fwrite(page.P2, sizeof(int), 1, ARQ);
        fwrite(page.C2, sizeof(int), 1, ARQ);
        fwrite(page.Pr2, sizeof(long long), 1, ARQ);
        fwrite(page.P3, sizeof(int), 1, ARQ);
        fwrite(page.C3, sizeof(int), 1, ARQ);
        fwrite(page.Pr3, sizeof(long long), 1, ARQ);
        fwrite(page.P4, sizeof(int), 1, ARQ);
        fwrite(page.C4, sizeof(int), 1, ARQ);
        fwrite(page.Pr4, sizeof(long long), 1, ARQ);
        fwrite(page.P5, sizeof(int), 1, ARQ);

        fseek(ARQ, 0, SEEK_END);
        fwrite(NEWPAGE.C1, sizeof(int), 1, ARQ);
        fwrite(NEWPAGE.Pr1, sizeof(long long), 1, ARQ);
        fwrite(NEWPAGE.P2, sizeof(int), 1, ARQ);
        fwrite(NEWPAGE.C2, sizeof(int), 1, ARQ);
        fwrite(NEWPAGE.Pr2, sizeof(long long), 1, ARQ);
        fwrite(NEWPAGE.P3, sizeof(int), 1, ARQ);
    }

    else if((C1 < inserted_key) && (inserted_key < C2)){
        NEWPAGE.C1 = page.C3
        NEWPAGE.Pr1 = page.Pr3
        NEWPAGE.P2 = page.P4
        NEWPAGE.C2 = page.C4
        NEWPAGE.Pr2 = page.Pr4
        NEWPAGE.P3 = page.P5

        page.P4 = page.P3;
        page.C3 = page.C2;
        page.Pr3 = page.Pr2;
        page.P3 = page.P2;
        page.C2 = inserted_key
        page.Pr2 = PON;
        page.P2 = -1;

        fseek(ARQ, CURRENT_RRN+1, SEEK_SET);
        fwrite(page.nroChavesIndexadas, sizeof(int), 1, ARQ);
        fseek(ARQ, 20, SEEK_CUR);
        fwrite(page.P2, sizeof(int), 1, ARQ);
        fwrite(page.C2, sizeof(int), 1, ARQ);
        fwrite(page.Pr2, sizeof(long long), 1, ARQ);
        fwrite(page.P3, sizeof(int), 1, ARQ);
        fwrite(page.C3, sizeof(int), 1, ARQ);
        fwrite(page.Pr3, sizeof(long long), 1, ARQ);
        fwrite(page.P4, sizeof(int), 1, ARQ);
        fwrite(page.C4, sizeof(int), 1, ARQ);
        fwrite(page.Pr4, sizeof(long long), 1, ARQ);
        fwrite(page.P5, sizeof(int), 1, ARQ);

        fseek(ARQ, 0, SEEK_END);
        fwrite(NEWPAGE.C1, sizeof(int), 1, ARQ);
        fwrite(NEWPAGE.Pr1, sizeof(long long), 1, ARQ);
        fwrite(NEWPAGE.P2, sizeof(int), 1, ARQ);
        fwrite(NEWPAGE.C2, sizeof(int), 1, ARQ);
        fwrite(NEWPAGE.Pr2, sizeof(long long), 1, ARQ);
        fwrite(NEWPAGE.P3, sizeof(int), 1, ARQ);
    }

    else if((C2 < inserted_key) && (inserted_key < C3)){
        NEWPAGE.C1 = page.C3
        NEWPAGE.Pr1 = page.Pr3
        NEWPAGE.P2 = page.P4
        NEWPAGE.C2 = page.C4
        NEWPAGE.Pr2 = page.Pr4
        NEWPAGE.P3 = page.P5

        page.P4 = page.P3;
        page.C3 = inserted_key;
        page.Pr3 = PON;
        page.P3 = -1;

        fseek(ARQ, CURRENT_RRN+1, SEEK_SET);
        fwrite(page.nroChavesIndexadas, sizeof(int), 1, ARQ);
        fseek(ARQ, 36, SEEK_CUR);
        fwrite(page.P3, sizeof(int), 1, ARQ);
        fwrite(page.C3, sizeof(int), 1, ARQ);
        fwrite(page.Pr3, sizeof(long long), 1, ARQ);
        fwrite(page.P4, sizeof(int), 1, ARQ);
        fwrite(page.C4, sizeof(int), 1, ARQ);
        fwrite(page.Pr4, sizeof(long long), 1, ARQ);
        fwrite(page.P5, sizeof(int), 1, ARQ);

        fseek(ARQ, 0, SEEK_END);
        fwrite(NEWPAGE.C1, sizeof(int), 1, ARQ);
        fwrite(NEWPAGE.Pr1, sizeof(long long), 1, ARQ);
        fwrite(NEWPAGE.P2, sizeof(int), 1, ARQ);
        fwrite(NEWPAGE.C2, sizeof(int), 1, ARQ);
        fwrite(NEWPAGE.Pr2, sizeof(long long), 1, ARQ);
        fwrite(NEWPAGE.P3, sizeof(int), 1, ARQ);
    }

    else if((C2 < inserted_key) && (inserted_key < C3)){
        NEWPAGE.C1 = page.C3
        NEWPAGE.Pr1 = page.Pr3
        NEWPAGE.P2 = page.P4
        NEWPAGE.C2 = page.C4
        NEWPAGE.Pr2 = page.Pr4
        NEWPAGE.P3 = page.P5

        page.P4 = page.P3;
        page.C3 = inserted_key;
        page.Pr3 = PON;
        page.P3 = -1;

        fseek(ARQ, CURRENT_RRN+1, SEEK_SET);
        fwrite(page.nroChavesIndexadas, sizeof(int), 1, ARQ);
        fseek(ARQ, 36, SEEK_CUR);
        fwrite(page.P3, sizeof(int), 1, ARQ);
        fwrite(page.C3, sizeof(int), 1, ARQ);
        fwrite(page.Pr3, sizeof(long long), 1, ARQ);
        fwrite(page.P4, sizeof(int), 1, ARQ);
        fwrite(page.C4, sizeof(int), 1, ARQ);
        fwrite(page.Pr4, sizeof(long long), 1, ARQ);
        fwrite(page.P5, sizeof(int), 1, ARQ);

        fseek(ARQ, 0, SEEK_END);
        fwrite(NEWPAGE.C1, sizeof(int), 1, ARQ);
        fwrite(NEWPAGE.Pr1, sizeof(long long), 1, ARQ);
        fwrite(NEWPAGE.P2, sizeof(int), 1, ARQ);
        fwrite(NEWPAGE.C2, sizeof(int), 1, ARQ);
        fwrite(NEWPAGE.Pr2, sizeof(long long), 1, ARQ);
        fwrite(NEWPAGE.P3, sizeof(int), 1, ARQ);
    }

    else if((C3 < inserted_key) && (inserted_key < C4)){
        NEWPAGE.C1 = inserted_key
        NEWPAGE.Pr1 = PON
        NEWPAGE.P2 =
        NEWPAGE.C2 = page.C4
        NEWPAGE.Pr2 = page.Pr4
        NEWPAGE.P3 = page.P5

        fseek(ARQ, CURRENT_RRN+1, SEEK_SET);
        fwrite(page.nroChavesIndexadas, sizeof(int), 1, ARQ);
        fseek(ARQ, 52, SEEK_CUR);
        fwrite(page.P4, sizeof(int), 1, ARQ);
        fwrite(page.C4, sizeof(int), 1, ARQ);
        fwrite(page.Pr4, sizeof(long long), 1, ARQ);
        fwrite(page.P5, sizeof(int), 1, ARQ);

        fseek(ARQ, 0, SEEK_END);
        fwrite(NEWPAGE.C1, sizeof(int), 1, ARQ);
        fwrite(NEWPAGE.Pr1, sizeof(long long), 1, ARQ);
        fwrite(NEWPAGE.P2, sizeof(int), 1, ARQ);
        fwrite(NEWPAGE.C2, sizeof(int), 1, ARQ);
        fwrite(NEWPAGE.Pr2, sizeof(long long), 1, ARQ);
        fwrite(NEWPAGE.P3, sizeof(int), 1, ARQ);
    }

    else if(C4 < inserted_key){
        NEWPAGE.C1 = page.C4
        NEWPAGE.Pr1 = page.Pr4
        NEWPAGE.P2 = page.P5
        NEWPAGE.C2 = inserted_key
        NEWPAGE.Pr2 = PON
        NEWPAGE.P3 = -1

        fseek(ARQ, 0, SEEK_END);
        fwrite(NEWPAGE.C1, sizeof(int), 1, ARQ);
        fwrite(NEWPAGE.Pr1, sizeof(long long), 1, ARQ);
        fwrite(NEWPAGE.P2, sizeof(int), 1, ARQ);
        fwrite(NEWPAGE.C2, sizeof(int), 1, ARQ);
        fwrite(NEWPAGE.Pr2, sizeof(long long), 1, ARQ);
        fwrite(NEWPAGE.P3, sizeof(int), 1, ARQ);
    }
}

typedef struct {
    int key;
    long long reference;
    int child_node_rrn;
} promotion_info;

// internal function that'll recursively insert a key to the tree
promotion_info recursive_insert(int current_rrn, int inserted_key, long long inserted_ref, FILE *file) {
    promotion_info return_value;

    // disk page struct
    btree_page node;

    // the current node's position on the btree file
    // (the +1 considers the header register)
    long current_node_position = (current_rrn + 1) * DISK_PAGE_LENGTH;

    // if it's not on the right position, goes to it
    if(ftell(file) != current_node_position){ fseek(file, current_node_position, SEEK_SET); }

    // reads the current node (disk page)
    fread(&node.folha              , sizeof(char)      , 1 , file);
    fread(&node.nroChavesIndexadas , sizeof(int)       , 1 , file);
    fread(&node.RRNdoNo            , sizeof(int)       , 1 , file);
    fread(&node.P1                 , sizeof(int)       , 1 , file);
    fread(&node.C1                 , sizeof(int)       , 1 , file);
    fread(&node.Pr1                , sizeof(long long) , 1 , file);
    fread(&node.P2                 , sizeof(int)       , 1 , file);
    fread(&node.C2                 , sizeof(int)       , 1 , file);
    fread(&node.Pr2                , sizeof(long long) , 1 , file);
    fread(&node.P3                 , sizeof(int)       , 1 , file);
    fread(&node.C3                 , sizeof(int)       , 1 , file);
    fread(&node.Pr3                , sizeof(long long) , 1 , file);
    fread(&node.P4                 , sizeof(int)       , 1 , file);
    fread(&node.C4                 , sizeof(int)       , 1 , file);
    fread(&node.Pr4                , sizeof(long long) , 1 , file);
    fread(&node.P5                 , sizeof(int)       , 1 , file);


    // base case - if the current node does not exist "promotes"
    // the inserted key (the past node was a leaf)
    if (current_rrn == INVALID) {
        return_value.key = inserted_key;
        return_value.reference = inserted_ref;
        return_value.child_node_rrn = INVALID;
    }

    // if the current node exists (the past node was not a leaf)
    else {
        // if the inserted key already exists on the tree, raises error
        if (node.C1 == inserted_key || node.C2 == inserted_key || node.C3 == inserted_key) { raise_error(""); }

        // finds the position in which the child node
        // in which the inserted key would enter
        int child_rrn = INVALID;
        if (inserted_key < node.C1) { child_rrn = node.P1; }
        else if (inserted_key < node.C2) { child_rrn = node.P2; }
        else if (inserted_key < node.C3) { child_rrn = node.P3; }
        else if (inserted_key < node.C4) { child_rrn = node.P4; }
        else{ child_rrn = node.P5; }

        promotion_info promotion = recursive_insert(child_rrn, inserted_key, inserted_ref, file);

        if(promotion.key == INVALID){ return promotion; }

        // if there is space on the current node (no split needed)
        else if(node.nroChavesIndexadas < 4){
            // inserts the promoted key to the current node while maintaining it sorted

            // if the promoted key will enter at position 1
            if(promotion.key < node.C1 && node.C1 != INVALID){
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
            else if(promotion.key < node.C2 && node.C2 != INVALID){
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
            else if(promotion.key < node.C3 && node.C3 != INVALID){
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
                node.C3  = promotion.key;
                node.Pr3 = promotion.reference;
                node.P4  = promotion.child_node_rrn;
            }

            // goes to the current register's "nroChavesIndexadas" field
            fseek(file, current_node_position + sizeof(char), SEEK_SET);

            // rewrites the current node
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

            // no promotion happened
            return_value.key = INVALID;
            return_value.reference = INVALID;
            return_value.child_node_rrn = INVALID;
        }

        else {
            // split()
        }
    }

    return return_value;
}

int __btree_search(int RRN, int inserted_key, FILE *ARQ){
    btree_page page;

    if (RRN == -1){
        return -1;
    }
    else{
        fseek(ARQ, (RRN+1)*DISK_PAGE_LENGTH, SEEK_SET);

        fread(&page.folha, sizeof(char), 1, ARQ);
        fread(&page.nroChavesIndexadas, sizeof(int), 1, ARQ);
        fread(&page.RRNdoNo, sizeof(int), 1, ARQ);
        fread(&page.P1, sizeof(int), 1, ARQ);
        fread(&page.C1, sizeof(int), 1, ARQ);
        fread(&page.Pr1, sizeof(long long), 1, ARQ);
        fread(&page.P2, sizeof(int), 1, ARQ);
        fread(&page.C2, sizeof(int), 1, ARQ);
        fread(&page.Pr2, sizeof(long long), 1, ARQ);
        fread(&page.P3, sizeof(int), 1, ARQ);
        fread(&page.C3, sizeof(int), 1, ARQ);
        fread(&page.Pr3, sizeof(long long), 1, ARQ);
        fread(&page.P4, sizeof(int), 1, ARQ);
        fread(&page.C4, sizeof(int), 1, ARQ);
        fread(&page.Pr4, sizeof(long long), 1, ARQ);
        fread(&page.P5, sizeof(int), 1, ARQ);

        if (inserted_key < page.C1){
            return (search(page.P1, inserted_key, ARQ));
        }
        else if ((page.C1 < inserted_key) && ( ((inserted_key < page.C2) && (page.nroChavesIndexadas > 1)) || ((page.nroChavesIndexadas == 1)))){
            return (search(page.P2, inserted_key, ARQ));
        }
        else if ((page.C2 < inserted_key) && ( ((inserted_key < page.C3) && (page.nroChavesIndexadas > 2)) || ((page.nroChavesIndexadas == 2)))){
            return (search(page.P3, inserted_key, ARQ));
        }
        else if ((page.C3 < inserted_key) && ( ((inserted_key < page.C4) && (page.nroChavesIndexadas > 3)) || ((page.nroChavesIndexadas == 3)))){
            return (search(page.P4, inserted_key, ARQ));
        }
        else if(page.C4 < inserted_key && (page.nroChavesIndexadas == 4)){
            return (search(page.P5, inserted_key, ARQ));
        }
        else{
            return RRN;
        }
    }
}
