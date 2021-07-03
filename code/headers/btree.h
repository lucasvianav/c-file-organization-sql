/** AUTHORS:
 * Lucas Viana Vilela 10748409
 * Stéfane Tame Monteiro Oliveira 10829970
*/

#include <stdio.h>

#ifndef TREE_H
#define TREE_H

// inserts "key" into the btree at "file"
// "reference" is a reference to the corresponding
// value on the data file (byte offset)
void __btree_insert(int key, long long reference, FILE *file);

// searches for the "key" on the btree at "file"
// returns the byte offset of the
// corresponding value on the data file
long long __btree_search(int key, FILE *file);

#endif
