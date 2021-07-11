/**
 * AUTHOR: Lucas Viana Vilela
 * NO.USP: 10748409
 */

#ifndef BTREE_VEHICLES_H
#define BTREE_VEHICLES_H

// creates a btree index file from a vehicle data file
// (the parameters are both the file's filenames)
void create_vehicle_btree(char *vehiclesFilename, char *btreeFilename);

// searches for a vehicle with received prefix on the btree
// and, if found, prints the data register's info
// (the filename parameters are both the file's)
void search_vehicle_btree(char *vehiclesFilename, char *btreeFilename, char *prefix);

#endif
