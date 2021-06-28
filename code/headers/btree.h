/** AUTHORS:
 * Lucas Viana Vilela 10748409
 * Stéfane Tame Monteiro Oliveira 10829970
*/

#include <stdio.h>

#ifndef TREE_H
#define TREE_H

int __btree_search(int RRN, int KEY, FILE *ARQ);

void __btree_split(int KEY, int I_RRN, int page, int PROMO_KEY, int PROMO_R_CHILD, int NEWPAGE, long long PON);

int __btree_insert(int CURRENT_RRN, int KEY, long long PON, FILE *arq);

#endif
