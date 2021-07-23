/**
 * AUTHOR: Lucas Viana Vilela
 * NO.USP: 10748409
 */

#ifndef UTIL_H
#define UTIL_H

#include <stdio.h>
#include "../structs/vehicle.h"
#include "../structs/line.h"

// compares two arrays char-by-char
// return-pattern the same as strcmp()
int cmp_string_field(char *str1, int str1_length, char *str2, int str2_length);

// prints a register's string-type field's title (description) and value
void print_string_field(char *key, int key_length, char *value, int value_length);

// prints a register's int-type field's title (description) and value
void print_int_field(char *key, int key_length, int value);

// copies a vector of chars into another (no '\0' necessary)
void copy_char_vector(char *a, char *b, int length);

// receives a filename and filetype ('b' for .bin and 'c' for .csv)
// and returns a string that has the file's full relative path
char *get_filepath(char *filename, char type);

// prints error message and exits program
// if an empty string is passed, prints standard message
void raise_error(char *error);

// prints a unique code identifying the binary file
void binarioNaTela(char *filename);

// reads a string input surrounded by double quotes (saves it to str)
void scan_quote_string(char *str);

// converts a vehicle prefix to an int
int convertePrefixo(char* str);

// receives a date in format YYYY-MM-DD and returns a
// formatted date string (the returned string needs to be freed)
char *format_date(char *date);

// receives a card status character and returns a
// formatted card status string (no need to free it)
char *format_card(char card_status);

// prints a vehicle's data
void print_vehicle(vehicle_header header, vehicle_register data, int newline);

// prints a line's data
void print_line(line_header header, line_register data, int newline);

// compares two line data registers (intended for qsort)
int cmp_lines(const void *a, const void *b);

// compares two vehicle data registers (intended for qsort)
int cmp_vehicles(const void *a, const void *b);

#endif
