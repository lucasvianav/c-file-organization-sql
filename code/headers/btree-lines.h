/** AUTHORS:
 * Lucas Viana Vilela 10748409
 * Stéfane Tame Monteiro Oliveira 10829970
 */

#ifndef BTREE_LINES_H
#define BTREE_LINES_H

// creates a btree index file from a line data file
// (the parameters are both the file's filenames)
void create_line_btree(char *linesFilename, char *btreeFilename);

// searches for a line with received code (codLinha) on the btree
// and, if found, prints the data register's info
// (the filename parameters are both the file's)
void search_line_btree(char *linesFilename, char *btreeFilename, int code);

#endif
