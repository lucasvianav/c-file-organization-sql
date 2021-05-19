#ifndef IO_H
#define IO_H

#include "./structs.h"

// reads a .csv file and returns the content as a string
char *read_csv(char *filename);

// receives a filename, parses the vehicle-content and writes it to the file
void write_vehicle_bin(char *filename, char *content);

// receives a filename, parses the line-content and writes it to the file
void write_line_bin(char *filename, char *content);

#endif