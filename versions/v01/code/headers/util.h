/** AUTHORS:
 * Lucas Viana Vilela 10748409
 * Stéfane Tame Monteiro Oliveira 10829970
*/

#ifndef UTIL_H
#define UTIL_H

// compares two arrays char-by-char 
// return-pattern the same as strcmp()
int cmp_string_field(char *str1, int str1_length, char *str2, int str2_length);

// prints a register's string-type field's title (description) and value
void print_string_field(char *key, int key_length, char *value, int value_length);

// prints a register's int-type field's title (description) and value
void print_int_field(char *key, int key_length, int value);

// receives a filename and filetype ('b' for .bin and 'c' for .csv) 
// and returns a string that has the file's full relative path
char *get_filepath(char *filename, char type);

// prints error message and exits program
// if an empty string is passed, prints standard message
void raise_error(char *error);

// reads a CSV file and returns the full content as a string
char *read_csv(char *filename);

// prints a unique code identifying the binary file
void binarioNaTela(char *filename);

// reads a string input surrounded by double quotes (saves it to str)
void scan_quote_string(char *str) ;

#endif