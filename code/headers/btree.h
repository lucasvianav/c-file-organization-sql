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
// root_rrn: pointer to a variable that holds the btree's root rrn
// free_rrn: points to a variable that hold the btree's next free rrn
void __btree_insert(int key, long long reference, FILE *file, int *root_rrn, int *free_rrn);

// searches for the "key" on the btree at "file"
// returns the byte offset of the
// corresponding value on the data file
// (or -1 if the key was not found)
long long __btree_search(int key, FILE *file);

#endif
