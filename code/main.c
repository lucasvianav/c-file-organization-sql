/** AUTHORS:
 * Lucas Viana Vilela 10748409
 * Stéfane Tame Monteiro Oliveira 10829970
*/

#include <stdio.h>
#include <stdlib.h>
#include "headers/vehicles.h"
#include "headers/lines.h"
#include "headers/util.h"
#include "headers/btree-lines.h"
#include "headers/btree-vehicles.h"

int main(){
    // function --> functionality to execute (1, 2, 3, 4, 5, 6, 7, 8)
    // n --> number of inputs to be read in functions 7 and 8
    int function, n;

    // auxiliar variables that'll be used to read input from the stdin
    char auxString1[50], auxString2[50], auxString3[50];

    // variable that'll be used to read the .csv files
    char *content;

    scanf("%d %s", &function, auxString1);

    switch(function){
        // functionality 1
        case 1:
            // reads binary file name
            scanf("%s", auxString2);

            // reads .csv file
            content = read_csv(auxString1);

            // parses .csv content and writes to binary
            write_vehicle_bin(auxString2, content);

            binarioNaTela(auxString2);
            free(content);

            break;

        // functionality 2
        case 2:
            // reads binary file name
            scanf("%s", auxString2);

            // reads .csv file
            content = read_csv(auxString1);

            // parses .csv content and writes to binary
            write_line_bin(auxString2, content);

            binarioNaTela(auxString2);
            free(content);

            break;

        // functionality 3
        case 3:
            print_vehicle_bin(auxString1);
            break;

        // functionality 4
        case 4:
            print_line_bin(auxString1);
            break;

        // functionality 5
        case 5:
            // reads query key-value pair
            scanf("%s", auxString2);
            scan_quote_string(auxString3);

            // searches for the query in the database and prints results
            search_vehicle_bin(auxString1, auxString2, auxString3);

            break;

        // functionality 6
        case 6:
            // reads query key-value pair
            scanf("%s", auxString2);
            scan_quote_string(auxString3);

            // searches for the query in the database and prints results
            search_line_bin(auxString1, auxString2, auxString3);

            break;

        // functionality 7
        case 7:
            // reads number of inputs to be read from stdin
            scanf("%d", &n);

            // reads all inputs and appends their data to the specified binary file
            append_vehicle_bin(auxString1, n);
            binarioNaTela(auxString1);

            break;

        // functionality 8
        case 8:
            // reads number of inputs to be read from stdin
            scanf("%d", &n);

            // reads all inputs and appends their data to the specified binary file
            append_line_bin(auxString1, n);
            binarioNaTela(auxString1);

            break;

        // functionality 9
        case 9:
            // reads btree file name
            scanf("%s", auxString2);

            // creates the btree index file from the data file
            create_vehicle_btree(auxString1, auxString2);

            binarioNaTela(auxString2);

            break;

        // functionality 10
        case 10:
            // reads btree file name
            scanf("%s", auxString2);

            // creates the btree index file from the data file
            create_line_btree(auxString1, auxString2);

            binarioNaTela(auxString2);

            break;

        // functionality 11
        case 11:
            // reads btree file name
            scanf("%s", auxString2);

            // reads and ignores "prefixo" input
            // and then reads the queried prefix
            scanf("%s", auxString3);
            scan_quote_string(auxString3);

            // searches for the query in the database and prints results
            search_vehicle_btree(auxString1, auxString2, auxString3);

            break;

        // functionality 12
        case 12:
            // reads btree file name
            scanf("%s", auxString2);

            // reads and ignores "codLinha" input
            // and then reads the queried code
            scanf("%s", auxString3);
            scan_quote_string(auxString3);

            // searches for the query in the database and prints results
            search_line_btree(auxString1, auxString2, atoi(auxString3));

            break;

        // functionality 13
        case 13:
            // reads btree file name
            scanf("%s", auxString2);

            // reads number of inputs to be read from stdin
            scanf("%d", &n);

            // reads all inputs and appends their data to the
            // specified vehicle file as well as the btree index
            append_vehicle_bin_btree(auxString1, auxString2, n);
            binarioNaTela(auxString1);

            break;

        // functionality 14
        case 14:
            // reads btree file name
            scanf("%s", auxString2);

            // reads number of inputs to be read from stdin
            scanf("%d", &n);

            // reads all inputs and appends their data to the
            // specified vehicle file as well as the btree index
            append_line_bin_btree(auxString1, auxString2, n);
            binarioNaTela(auxString1);

            break;

        // if an invalid funcionality was passed
        default: raise_error("");
    }

    return 0;
}
