/** AUTHORS:
 * Lucas Viana Vilela 10748409
 * Stéfane Tame Monteiro Oliveira 10829970
*/

#ifndef LINES_H
#define LINES_H

// receives a filename, parses the content and writes it to the file
void write_line_bin(char *filename, char *content);

// receives a filename, reads "no_inputs" lines and appends it to the file
void append_line_bin(char *filename, int no_inputs);

// receives a filename, reads "no_inputs" lines and appends it to the file
// also inserts it's reference on the passed btree
void append_line_bin_btree(char *linesFilename, char *btreeFilename, int no_inputs);

// receives a filename, reads and parses all of the
// binary file's registers and prints the parsed data
void print_line_bin(char *filename);

// receives a filename as well as a query (field key-value pair),
// parses all of the binary file's registers and prints every
// register that matches the query's parsed data
void search_line_bin(char *filename, char *key, char *value);

#endif
