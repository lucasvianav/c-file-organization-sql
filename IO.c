#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// reads a CSV file and returns the content as a string
char *readCSV(char *filename){
    char *basepath = "";

    // string that has the .csv filepath (inside the "data" directory)
    char *filepath = (char *)malloc((strlen(basepath) + strlen(filename) + 1) * sizeof(char));

    // sets filepath's value
    strcpy(filepath, basepath);
    strcat(filepath, filename);
    
    printf("\n\n\n%s\n\n\n", filepath);

    // opens the file in reading mode
    FILE *fp = fopen(filename, "r");

    if(fp == NULL){
        printf("alo!\n\n\n");
    }

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

    // deletes the header row (first register)
    if(content){ strsep(&content, "\n"); }

    fclose(fp);
    
    return content;
}