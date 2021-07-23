/**
 * AUTHOR: Lucas Viana Vilela
 * NO.USP: 10748409
 */

#ifndef FILE_HANDLING_H
#define FILE_HANDLING_H

#include <stdio.h>
#include "../structs/vehicle.h"
#include "../structs/line.h"

// reads a CSV file and returns the content as a string
char *fread_csv(char *filename);

// opens a binary file, checks if it is
// consistent and returns a pointer to it
FILE *fopen_validate_binary(char *filename,  char *mode);

// reads all data from the
// vehicle's header but the status
vehicle_header fread_vehicle_header(FILE *file);

// reads all data from the
// line's header but the status
line_header fread_line_header(FILE *file);

// reads a vehicle data register from
// the received file and returns it
vehicle_register fread_vehicle_register(FILE *file);

// reads a line data register from
// the received file and returns it
line_register fread_line_register(FILE *file);

// writes the passed consistency
// to the passed file's header
// '0' means inconsistent
// '1' means consistent
void set_consistency(char consistency, FILE *file);

// creates a binary file with the received filename
FILE *fcreate_binary(char *filename);

// deletes a binary file with the received filename
void fremove_binary(char *filename);

#endif
