#include <stdio.h>
#include <stdlib.h>
#include "IO.h"
//#include "./IO.c"
//#include "./util.c"

int main(){
    int function;
    char auxString1[50], auxString2[50], str[50];
    FILE *arq;
    
    scanf("%d", &function);

    switch(function){
        case 1:
            break;
            
        case 2:
            break;

        case 3:
            //scanf("%s", auxString1);
            
            impressao("veiculo.bin");

            break;

        case 4:
            //scanf("%s", auxString1);

            impressao2("linha.bin");

            break;

        case 5:
            scanf("%s %s", auxString1, auxString2);
            scan_quote_string(str);

            busca(auxString1, auxString2, str);

            break;

        case 6:
            scanf("%s %s", auxString1, auxString2);
            scan_quote_string(str);

            busca2(auxString1, auxString2, str);

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