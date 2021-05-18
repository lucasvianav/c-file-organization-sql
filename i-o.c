#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./strucs.c"

// reads a CSV file and returns a pointer to a "csv_data" variable
csv_data *readCSV(char *filename, char type){
    char *basepath = "./data/";

    // string that has the .csv filepath (inside the "data" directory)
    char *filepath = (char *)malloc((strlen(basepath) + strlen(filename) + 1) * sizeof(char));

    // sets filepath's value
    strcpy(filepath, basepath);
    strcat(filepath, filename);

    // opens the file in reading mode
    FILE *fp = fopen(filename, "r");

    // string in which all of the file's content will be stored
    // it's initial value is 0 bytes because each char will be read
    // individually
    int contentSize = 0;
    char *content = (char *)malloc(0);

    // temporary/auxiliar variable to read individual chars
    char tmp;

    // loop to read each of the file's characters
    while(fread(&tmp, sizeof(char), 1, fp)){
        content = (char *)realloc(content, ++contentSize * sizeof(char));
        content[contentSize-1] = tmp;
    }

    // terminates the string 
    content = (char *)realloc(content, ++contentSize * sizeof(char));
    content[contentSize-1] = '\0';

    // temporary/auxiliar variable to read each .csv row
    char *tmp_row, *tmp_value; 

    // deletes the header row (first register)
    if(content){ strsep(&content, "\n"); }

    // list of rows that'll be returned
    csv_data *data = (csv_data *)malloc(sizeof(csv_data));
    data->length = 0;

    // loops through every row (every register)
    while(strcmp(content, "")){
        // gets next row
        tmp_row = strsep(&content, "\n");
        
        data->list = (char ***)realloc(data->list, ++(data->length) * sizeof(char **));
        data->row_length = 0;
        
        tmp_value = strsep(&tmp_row, ",");
        while(tmp_value != NULL){
            data->list[data->length-1] = (char **)realloc(data->list[data->length-1], ++(data->row_length) * sizeof(char *));
            data->list[data->length-1][data->row_length-1] = tmp_value;

            tmp_value = strsep(&tmp_row, ",");
        }
    }

    fclose(fp);
    
    return data;
}

// completely deallocates a "csv_data" variable
void free_csv_data(csv_data **data){
    // deallocates each row
    for(int i = 0; i < (*data)->length; i++){ 
        free((*data)->list[i]); 
        (*data)->list[i] = NULL;
    }
    
    // deallocates the list
    free((*data)->list);
    (*data)->list = NULL;

    // deallocates the variable itself
    free(*data);
    *data = NULL;
    
    // there's no need to deallocate the rows' values (strings) 'cause 
    // they weren't dynamically allocated in the first place (strsep)
}