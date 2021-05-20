#include <stdio.h>
#include <stdlib.h>
#include "./vehicle-io.c"
#include "./line-io.c"
#include "./util.c"

int main(){
    int function;
    char auxString1[50], auxString2[50]; //, auxString3[50];
    char *content;
    
    scanf("%d %s", &function, auxString1);

    switch(function){
        case 1:
            scanf("%s", auxString2);
            
            content = read_csv(auxString1);
            write_vehicle_bin(auxString2, content);
            
            binarioNaTela(auxString2);
            free(content);

            break;
            
        case 2:
            scanf("%s", auxString2);

            content = read_csv(auxString1);
            write_line_bin(auxString2, content);

            binarioNaTela(auxString2);
            free(content);

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