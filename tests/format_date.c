#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *format_date(char *date){
    char formatted[70];

    // day
    formatted[0] = date[8];
    formatted[1] = date[9];
    formatted[2] = '\0';
    
    strcat(formatted, " de ");

    char month_str[3];
    month_str[0] = date[5];
    month_str[1] = date[6];
    month_str[2] = '\0';
    
    char year_str[10] = " de ";
    year_str[4] = date[0];
    year_str[5] = date[1];
    year_str[6] = date[2];
    year_str[7] = date[3];
    year_str[8] = '\n';
    year_str[9] = '\0';

    int month = atoi(month_str);

    switch (month) {
        case 1:
            strcat(formatted, "janeiro");
            break;

        case 2:
            strcat(formatted, "fevereiro");
            break;

        case 3:
            strcat(formatted, "março");
            break;

        case 4:
            strcat(formatted, "abril");
            break;

        case 5:
            strcat(formatted, "maio");
            break;

        case 6:
            strcat(formatted, "junho");
            break;

        case 7:
            strcat(formatted, "julho");
            break;

        case 8:
            strcat(formatted, "agosto");
            break;

        case 9:
            strcat(formatted, "setembro");
            break;

        case 10:
            strcat(formatted, "outubro");
            break;

        case 11:
            strcat(formatted, "novembro");
            break;

        case 12:
            strcat(formatted, "dezembro");
            break;

        default:
            break;
    }

    strcat(formatted, year_str);

    return strdup(formatted);
}

int main(){
    char *date = "2021-05-25";
    char *formatted = format_date(date);
    printf("%s", formatted);
    free(formatted);
    return 0;
}