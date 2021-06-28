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

void __btree_split(int KEY, int I_RRN, int page, int PROMO_KEY, int PROMO_R_CHILD, int NEWPAGE, long long PON){
    //I_KEY -> nova chave a ser inserida
    //I_RRN-> filho a direita da nova chave a ser inserida
    //PAGE -> página de disco corrente
    //PROMO_KEY -> chave promovida
    //PROMO_R_CHILD -> filho a direita da chave promovida
    //NEWPAGE -> nova página de disco
    btree_page NEWPAGE;

    if(KEY < C1){
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
        page.C1 = KEY;
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
    else if((C1 < KEY) && (KEY < C2)){
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
        page.C2 = KEY
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
    else if((C2 < KEY) && (KEY < C3)){
        NEWPAGE.C1 = page.C3
            NEWPAGE.Pr1 = page.Pr3
            NEWPAGE.P2 = page.P4
            NEWPAGE.C2 = page.C4
            NEWPAGE.Pr2 = page.Pr4
            NEWPAGE.P3 = page.P5

            page.P4 = page.P3;
        page.C3 = KEY;
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
    else if((C2 < KEY) && (KEY < C3)){
        NEWPAGE.C1 = page.C3
            NEWPAGE.Pr1 = page.Pr3
            NEWPAGE.P2 = page.P4
            NEWPAGE.C2 = page.C4
            NEWPAGE.Pr2 = page.Pr4
            NEWPAGE.P3 = page.P5

            page.P4 = page.P3;
        page.C3 = KEY;
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
    else if((C3 < KEY) && (KEY < C4)){
        NEWPAGE.C1 = KEY
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
    else if(C4 < KEY){
        NEWPAGE.C1 = page.C4
            NEWPAGE.Pr1 = page.Pr4
            NEWPAGE.P2 = page.P5
            NEWPAGE.C2 = KEY
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

int __btree_insert(int CURRENT_RRN, int KEY, long long PON, FILE *arq){
    btree_header header;
    btree_page node;

    if (CURRENT_RRN == -1){
        //corrige as infos da arvore
        header.noRaiz = 0;
        header.RRNproxNo = 1;

        //cria uma pagina
        node.folha = '1';
        node.nroChavesIndexadas = 1;
        node.RRNdoNo = header.noRaiz;
        node.P1 = -1;
        node.C1 = KEY;
        node.Pr1 = PON;
        node.P2 = -1;
        node.C2 = -1;
        node.Pr2 = -1;
        node.P3 = -1;
        node.C3 = -1;
        node.Pr3 = -1;
        node.P4 = -1;
        node.C4 = -1;
        node.Pr4 = -1;
        node.P5 = -1;

        //insere no arquivo
        fseek(arq, 1, SEEK_SET);
        fwrite(&header.noRaiz, sizeof(int), 1, arq);
        fwrite(&header.RRNproxNo, sizeof(int), 1, arq);

        fseek(arq, 68, SEEK_CUR);

        fwrite(&node.folha, sizeof(char), 1, arq);
        fwrite(&node.nroChavesIndexadas, sizeof(int), 1, arq);
        fwrite(&node.RRNdoNo, sizeof(int), 1, arq);
        fwrite(&node.P1, sizeof(int), 1, arq);
        fwrite(&node.C1, sizeof(int), 1, arq);
        fwrite(&node.Pr1, sizeof(long long), 1, arq);
        fwrite(&node.P2, sizeof(int), 1, arq);
        fwrite(&node.C2, sizeof(int), 1, arq);
        fwrite(&node.Pr2, sizeof(long long), 1, arq);
        fwrite(&node.P3, sizeof(int), 1, arq);
        fwrite(&node.C3, sizeof(int), 1, arq);
        fwrite(&node.Pr3, sizeof(long long), 1, arq);
        fwrite(&node.P4, sizeof(int), 1, arq);
        fwrite(&node.C4, sizeof(int), 1, arq);
        fwrite(&node.Pr4, sizeof(long long), 1, arq);
        fwrite(&node.P5, sizeof(int), 1, arq);

        return 1;
    }
    else{
        fseek(arq, (CURRENT_RRN+1)*DISK_PAGE_LENGTH, SEEK_SET);

        fread(&node.folha, sizeof(char), 1, arq);
        fread(&node.nroChavesIndexadas, sizeof(int), 1, arq);
        fread(&node.RRNdoNo, sizeof(int), 1, arq);
        fread(&node.P1, sizeof(int), 1, arq);
        fread(&node.C1, sizeof(int), 1, arq);
        fread(&node.Pr1, sizeof(long long), 1, arq);
        fread(&node.P2, sizeof(int), 1, arq);
        fread(&node.C2, sizeof(int), 1, arq);
        fread(&node.Pr2, sizeof(long long), 1, arq);
        fread(&node.P3, sizeof(int), 1, arq);
        fread(&node.C3, sizeof(int), 1, arq);
        fread(&node.Pr3, sizeof(long long), 1, arq);
        fread(&node.P4, sizeof(int), 1, arq);
        fread(&node.C4, sizeof(int), 1, arq);
        fread(&node.Pr4, sizeof(long long), 1, arq);
        fread(&node.P5, sizeof(int), 1, arq);

        if ((KEY == page.C1) || ((KEY == page.C2) && (page.nroChavesIndexadas > 1)) || ((KEY == C3) && (page.nroChavesIndexadas > 2)) || ((KEY == C4) && (page.nroChavesIndexadas > 3))){
            raise_error("");
            return -1
        } else if (KEY < node.C1) {
            RETURN_VALUE =
                insert(page.P1, KEY, PROMO_R_CHILD, PROMO_KEY, PON, ARQ)
        } else if ((node.C1 < KEY) &&
                (((KEY < node.C2) && (node.nroChavesIndexadas > 1)) ||
                 ((node.nroChavesIndexadas == 1)))) {
            RETURN_VALUE =
                insert(page.P2, KEY, PROMO_R_CHILD, PROMO_KEY, PON, ARQ)
        } else if ((node.C2 < KEY) &&
                (((KEY < node.C3) && (node.nroChavesIndexadas > 2)) ||
                 ((node.nroChavesIndexadas == 2)))) {
            RETURN_VALUE =
                insert(page.P3, KEY, PROMO_R_CHILD, PROMO_KEY, PON, ARQ)
        } else if ((node.C3 < KEY) &&
                (((KEY < node.C4) && (node.nroChavesIndexadas > 3)) ||
                 ((node.nroChavesIndexadas == 3)))) {
            RETURN_VALUE =
                insert(page.P4, KEY, PROMO_R_CHILD, PROMO_KEY, PON, ARQ)
        } else if (node.C4 < KEY && (node.nroChavesIndexadas == 4)) {
            RETURN_VALUE =
                insert(page.P4, KEY, PROMO_R_CHILD, PROMO_KEY, PON, ARQ)
        }

        if ((RETURN_VALUE == 0) or (RETURN_VALUE == -1)){
            return RETURN_VALUE
        } else if (node.nroChavesIndexadas < 4) {
            if (KEY < C1) {
                node.nroChavesIndexadas++;
                node.P5 = node.P4;
                node.C4 = node.C3;
                node.Pr4 = node.Pr3;
                node.P4 = node.P3;
                node.C3 = node.C2;
                node.Pr3 = node.Pr2;
                node.P3 = node.P2;
                node.C2 = node.C1;
                node.Pr2 = node.Pr1;
                node.P2 = node.P1;
                node.C1 = KEY;
                node.Pr1 = PON;
                node.P1 = -1;

                fseek(ARQ, CURRENT_RRN + 1, SEEK_SET);
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
            } else if ((node.C1 < KEY) &&
                    (((KEY < node.C2) && (node.nroChavesIndexadas > 1)) ||
                     ((node.nroChavesIndexadas == 1)))) {
                node.nroChavesIndexadas++;
                node.P5 = node.P4;
                node.C4 = node.C3;
                node.Pr4 = node.Pr3;
                node.P4 = node.P3;
                node.C3 = node.C2;
                node.Pr3 = node.Pr2;
                node.P3 = node.P2;
                node.C2 = KEY node.Pr2 = PON;
                node.P2 = -1;

                fseek(ARQ, CURRENT_RRN + 1, SEEK_SET);
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
            } else if ((node.C2 < KEY) &&
                    (((KEY < node.C3) && (node.nroChavesIndexadas > 2)) ||
                     ((node.nroChavesIndexadas == 2)))) {
                node.nroChavesIndexadas++;
                node.P5 = node.P4;
                node.C4 = node.C3;
                node.Pr4 = node.Pr3;
                node.P4 = node.P3;
                node.C3 = KEY;
                node.Pr3 = PON;
                node.P3 = -1;

                fseek(ARQ, CURRENT_RRN + 1, SEEK_SET);
                fwrite(page.nroChavesIndexadas, sizeof(int), 1, ARQ);
                fseek(ARQ, 36, SEEK_CUR);
                fwrite(page.P3, sizeof(int), 1, ARQ);
                fwrite(page.C3, sizeof(int), 1, ARQ);
                fwrite(page.Pr3, sizeof(long long), 1, ARQ);
                fwrite(page.P4, sizeof(int), 1, ARQ);
                fwrite(page.C4, sizeof(int), 1, ARQ);
                fwrite(page.Pr4, sizeof(long long), 1, ARQ);
                fwrite(page.P5, sizeof(int), 1, ARQ);
            } else if ((node.C3 < KEY) &&
                    (((KEY < node.C4) && (node.nroChavesIndexadas > 3)) ||
                     ((node.nroChavesIndexadas == 3)))) {
                node.nroChavesIndexadas++;
                node.P5 = node.P4;
                node.C4 = node.C3;
                node.Pr4 = node.Pr3;
                node.P4 = node.P3;
                node.C3 = KEY;
                node.Pr3 = PON;
                node.P3 = -1;

                fseek(ARQ, CURRENT_RRN + 1, SEEK_SET);
                fwrite(page.nroChavesIndexadas, sizeof(int), 1, ARQ);
                fseek(ARQ, 36, SEEK_CUR);
                fwrite(page.P3, sizeof(int), 1, ARQ);
                fwrite(page.C3, sizeof(int), 1, ARQ);
                fwrite(page.Pr3, sizeof(long long), 1, ARQ);
                fwrite(page.P4, sizeof(int), 1, ARQ);
                fwrite(page.C4, sizeof(int), 1, ARQ);
                fwrite(page.Pr4, sizeof(long long), 1, ARQ);
                fwrite(page.P5, sizeof(int), 1, ARQ);
            } else if (node.C3 < KEY) {
                node.nroChavesIndexadas++;
                node.P5 = node.P4;
                node.C4 = KEY;
                node.Pr4 = PON;
                node.P4 = -1;

                fseek(ARQ, CURRENT_RRN + 1, SEEK_SET);
                fwrite(page.nroChavesIndexadas, sizeof(int), 1, ARQ);
                fseek(ARQ, 52, SEEK_CUR);
                fwrite(page.P4, sizeof(int), 1, ARQ);
                fwrite(page.C4, sizeof(int), 1, ARQ);
                fwrite(page.Pr4, sizeof(long long), 1, ARQ);
                fwrite(page.P5, sizeof(int), 1, ARQ);
            }

            return NO_PROMOTION
        } else {
            // split(KEY, I_RRN, CURRENT_RRN, PROMO_KEY, PROMO_R_CHILD, NEWPAGE,
            // PON)

            return PROMOTION
        }
    }
}

int __btree_search(int RRN, int KEY, FILE *ARQ){
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

        if (KEY < page.C1){
            return (search(page.P1, KEY, ARQ));
        }
        else if ((page.C1 < KEY) && ( ((KEY < page.C2) && (page.nroChavesIndexadas > 1)) || ((page.nroChavesIndexadas == 1)))){
            return (search(page.P2, KEY, ARQ));
        }
        else if ((page.C2 < KEY) && ( ((KEY < page.C3) && (page.nroChavesIndexadas > 2)) || ((page.nroChavesIndexadas == 2)))){
            return (search(page.P3, KEY, ARQ));
        }
        else if ((page.C3 < KEY) && ( ((KEY < page.C4) && (page.nroChavesIndexadas > 3)) || ((page.nroChavesIndexadas == 3)))){
            return (search(page.P4, KEY, ARQ));
        }
        else if(page.C4 < KEY && (page.nroChavesIndexadas == 4)){
            return (search(page.P5, KEY, ARQ));
        }
        else{
            return RRN;
        }
    }
}
