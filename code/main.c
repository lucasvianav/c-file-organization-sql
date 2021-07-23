/**
 * AUTHOR: Lucas Viana Vilela
 * NO.USP: 10748409
 */

#include <stdio.h>
#include <stdlib.h>
#include "headers/vehicles.h"
#include "headers/lines.h"
#include "headers/util.h"
#include "headers/file-handling.h"
#include "headers/btree-lines.h"
#include "headers/btree-vehicles.h"
#include "headers/join.h"

int main(){
    // functionality to execute (1, 2, 3, 4, 5, 6, ..., 14)
    int function;

    // number of inputs to be read in functions 7, 8, 13 and 14
    int n;

    // auxiliar variables that'll be used to read input from the stdin
    char auxString1[50], auxString2[50], auxString3[50];

    // variable that'll be used to read the .csv files
    char *content;

    scanf("%d %s", &function, auxString1);

    switch(function){
        // functionality 1
        // create vehicles binary from csv
        case 1:
            // reads binary file name
            scanf("%s", auxString2);

            // reads .csv file
            content = fread_csv(auxString1);

            // parses .csv content and writes to binary
            write_vehicle_bin(auxString2, content);

            binarioNaTela(auxString2);
            free(content);

            break;

        // functionality 2
        // create lines binary from csv
        case 2:
            // reads binary file name
            scanf("%s", auxString2);

            // reads .csv file
            content = fread_csv(auxString1);

            // parses .csv content and writes to binary
            write_line_bin(auxString2, content);

            binarioNaTela(auxString2);
            free(content);

            break;

        // functionality 3
        // prints all vehicle registers from binary
        case 3:
            print_vehicle_bin(auxString1);
            break;

        // functionality 4
        // prints all line registers from binary
        case 4:
            print_line_bin(auxString1);
            break;

        // functionality 5
        // searches for a vehicle in the binary
        case 5:
            // reads query key-value pair
            scanf("%s", auxString2);
            scan_quote_string(auxString3);

            // searches for the query in the database and prints results
            search_vehicle_bin(auxString1, auxString2, auxString3);

            break;

        // functionality 6
        // searches for a line in the binary
        case 6:
            // reads query key-value pair
            scanf("%s", auxString2);
            scan_quote_string(auxString3);

            // searches for the query in the database and prints results
            search_line_bin(auxString1, auxString2, auxString3);

            break;

        // functionality 7
        // appends to a vehicle binary from stdin
        case 7:
            // reads number of inputs to be read from stdin
            scanf("%d", &n);

            // reads all inputs and appends their data to the specified binary file
            append_vehicle_bin(auxString1, n);
            binarioNaTela(auxString1);

            break;

        // functionality 8
        // appends to a line binary from stdin
        case 8:
            // reads number of inputs to be read from stdin
            scanf("%d", &n);

            // reads all inputs and appends their data to the specified binary file
            append_line_bin(auxString1, n);
            binarioNaTela(auxString1);

            break;

        // functionality 9
        // creates a btree index from a vehicle binary
        case 9:
            // reads btree file name
            scanf("%s", auxString2);

            // creates the btree index file from the data file
            create_vehicle_btree(auxString1, auxString2);

            binarioNaTela(auxString2);

            break;

        // functionality 10
        // creates a btree index from a line binary
        case 10:
            // reads btree file name
            scanf("%s", auxString2);

            // creates the btree index file from the data file
            create_line_btree(auxString1, auxString2);

            binarioNaTela(auxString2);

            break;

        // functionality 11
        // searches for a vehicle using it's btree index
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
        // searches for a line using it's btree index
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
        // appends to a vehicle binary and
        // it's btree index from stdin
        case 13:
            // reads btree file name
            scanf("%s", auxString2);

            // reads number of inputs to be read from stdin
            scanf("%d", &n);

            // reads all inputs and appends their data to the
            // specified vehicle file as well as the btree index
            append_vehicle_bin_btree(auxString1, auxString2, n);
            binarioNaTela(auxString2);

            break;

        // functionality 14
        // appends to a line binary and
        // it's btree index from stdin
        case 14:
            // reads btree file name
            scanf("%s", auxString2);

            // reads number of inputs to be read from stdin
            scanf("%d", &n);

            // reads all inputs and appends their data to the
            // specified vehicle file as well as the btree index
            append_line_bin_btree(auxString1, auxString2, n);
            binarioNaTela(auxString2);

            break;

        // functionality 15
        // joins vehicles and lines
        // data using nested loops
        case 15:
            // reads line binary's filename
            scanf("%s", auxString2);

            // reads and ignores "nomeCampoVeiculo"
            // and "nomeCampoLinha" input
            scanf("%s %s", auxString3, auxString3);

            nested_loop_join(auxString1, auxString2);

            break;

        // functionality 16
        // joins vehicles and lines
        // data using a single loop
        case 16:
            // reads line binary's filename
            scanf("%s", auxString2);

            // reads and ignores "nomeCampoVeiculo"
            // and "nomeCampoLinha" input
            scanf("%s %s", auxString3, auxString3);

            // reads the line btree index's filename
            scanf("%s", auxString3);

            single_loop_join(auxString1, auxString2, auxString3);

            break;

        // functionality 17
        // sorts a vehicles binary's data
        case 17:
            // reads the new sorted binary's filename
            scanf("%s", auxString2);

            // reads and ignores the "campoOrdenacao" input
            scanf("%s", auxString3);

            sort_vehicles_bin(auxString1, auxString2);
            binarioNaTela(auxString2);

            break;

        // functionality 18
        // sorts a line binary's data
        case 18:
            // reads the new sorted binary's filename
            scanf("%s", auxString2);

            // reads and ignores the "campoOrdenacao" input
            scanf("%s", auxString3);

            sort_lines_bin(auxString1, auxString2);
            binarioNaTela(auxString2);

            break;

        // functionality 19
        // joins vehicles and lines data
        // using sorted intercalation
        case 19:
            // reads the lines binary's filename
            scanf("%s", auxString2);

            // reads and ignores the "nomeCampoVeiculo"
            // and the "nomeCampoLinha" inputs
            scanf("%s %s", auxString3, auxString3);

            sorted_interpolation_join(auxString1, auxString2);

            break;

        // if an invalid funcionality was passed
        default: raise_error("");
    }

    return 0;
}
