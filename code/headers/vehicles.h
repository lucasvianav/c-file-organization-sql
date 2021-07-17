/**
 * AUTHOR: Lucas Viana Vilela
 * NO.USP: 10748409
 */

#ifndef VEHICLES_H
#define VEHICLES_H

// receives a filename, parses the content and writes it to the file
void write_vehicle_bin(char *filename, char *content);

// receives a filename, reads "no_inputs" vehicles and appends it to the file
void append_vehicle_bin(char *filename, int no_inputs);

// receives a filename, reads "no_inputs" vehicles and appends it
// to the file also inserts it's reference on the passed btree
void append_vehicle_bin_btree(char *vehiclesFilename, char *btreeFilename, int no_inputs);

// receives a filename, reads and parses all of the
// binary file's registers and prints the parsed data
void print_vehicle_bin(char *filename);

// receives a filename as well as a query (field key-value pair),
// parses all of the binary file's registers and prints every
// register that matches the query's parsed data
void search_vehicle_bin(char *filename, char *key, char *value);

// sorts and removes logically deleted register from
// the original binary then writes it to a new files
void sort_vehicles_bin(char *originalFilename, char *sortedFilename);

#endif
