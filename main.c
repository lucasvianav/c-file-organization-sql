#include <stdio.h>
#include <stdlib.h>
#include "./code/source/io.c"
#include "./code/source/util.c"

int main(){
    int function;
    char auxString1[50], auxString2[50];
    
    scanf("%d", &function);

    switch(function){
        case 1:
            scanf("%s %s", auxString1, auxString2);
            
            char *content = read_csv(auxString1);
            write_vehicle_bin(auxString2, content);
            
            binarioNaTela(auxString2);
            free(content);

            break;
            
        case 2:
            break;

        case 3:
            break;

        case 4:
            break;

        case 5:
            break;

        case 6:
            break;

        case 7:
            break;

        case 8:
            break;

        default:
            break;
    }
    
    return 0;
}