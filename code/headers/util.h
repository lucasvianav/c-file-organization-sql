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
void scan_quote_string(char *str);

// converts a vehicle prefix to an int
int convertePrefixo(char* str);

// receives a date in format YYYY-MM-DD and returns a
// formatted date string (the returned string needs to be freed)
char *format_date(char *date);

// receives a card status character and returns a
// formatted card status string (no need to free it)
char *format_card(char card_status);

// opens a binary file, checks if it is
// consistent and returns a pointer to it
FILE *open_validate_binary(char *filename,  char *mode);

// prints a vehicle's data
void print_vehicle(vehicle_header header, vehicle_register data, int newline);

// prints a line's data
void print_line(line_header header, line_register data, int newline);

// reads all data from the
// vehicle's header but the status
vehicle_header read_vehicle_header(FILE *file);

// reads all data from the
// line's header but the status
line_header read_line_header(FILE *file);

// compares two registers (ideal for qsort)
int cmp_registers(const void *a, const void *b);

// reads a vehicle data register from
// the received file and returns it
vehicle_register fread_vehicle_register(FILE *file);

#endif
