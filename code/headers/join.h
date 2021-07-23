/**
 * AUTHOR: Lucas Viana Vilela
 * NO.USP: 10748409
 */

#include <stdio.h>

#ifndef JOIN_H
#define JOIN_H

// joins a vehicle binary file's data with a line binary file's data
// uses two nested loops to perform the join operations
void nested_loop_join(char *vehiclesFilename, char *linesFilename);

// joins a vehicle binary file's data with a line binary file's data
// uses a single loop with a btree index for the lines file
void single_loop_join(char *vehiclesFilename, char *linesFilename, char* linesBtreeFilename);

// joins a vehicle binary file's data with a line binary file's data
// uses a single loop with sort-interpolate strategy
void sorted_interpolation_join(char *vehiclesFilename, char *linesFilename);

#endif
