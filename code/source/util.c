/** AUTHORS:
 * Lucas Viana Vilela 10748409
 * Stéfane Tame Monteiro Oliveira 10829970
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../headers/util.h"

// compares two arrays char-by-char
// return-pattern the same as strcmp()
int cmp_string_field(char *str1, int str1_length, char *str2, int str2_length){
    int i;
    for(i = 0; i < str1_length && i < str2_length; i++){ if(str1[i] != str2[i]){ return 1; } }
    return ( i == str1_length && i == str2_length ) ? 0 : 1;
}

// prints a register's string-type field's title (description) and value
void print_string_field(char *key, int key_length, char *value, int value_length){
    // prints that field's title
    // (for-loop is necessary because the array does not contain '\0')
    for(int i=0; i < key_length; i++){ printf("%c", key[i]); }
    printf(": ");

    // prints the current value if not empty and custom message otherwise
    // (for-loop is necessary because array does not contain '\0')
    if(value_length == 0){ printf("campo com valor nulo"); }
    else{ for(int i=0; i < value_length; i++){ printf("%c", value[i]); } }

    // prints newline
    printf("\n");
}

// prints a register's int-type field's title (description) and value
void print_int_field(char *key, int key_length, int value){
    // prints that field's title
    // (for-loop is necessary because the array does not contain '\0')
    for(int i=0; i < key_length; i++){ printf("%c", key[i]); }

    // prints the current value if not empty and custom message otherwise
    if(value == -1){ printf(": campo com valor nulo\n"); }
    else{ printf(": %d\n", value); }
}

// receives a filename and filetype ('b' for .bin and 'c' for .csv)
// and returns a string that has the file's full relative path
char *get_filepath(char *filename, char type){
    char *basepath = type == 'b' ? "./binaries/" : "./data/";

    // string that has the file's full relative (inside the respective directory)
    char *filepath = (char *)malloc((strlen(basepath) + strlen(filename) + 1) * sizeof(char));

    // sets filepath's value
    strcpy(filepath, basepath);
    strcat(filepath, filename);

    return filepath;
}

// prints error message and exits program
void raise_error(char *error){
    printf("%s\n", error[0] == '\0' ? "Falha no processamento do arquivo." : error);
    exit(0);
}

// reads a CSV file and returns the content as a string
char *read_csv(char *filename){
    // string that has the .csv filepath (inside the "data" directory)
    char *filepath = get_filepath(filename, 'c');

    // opens the file in reading mode
    FILE *fp = fopen(filepath, "r");

    // if the files does not exist, raises error and exists program
    if(!fp){ raise_error(""); }

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

    // closes file and frees allocated data
    fclose(fp);
    free(filepath);

    return content;
}

// prints a unique code identifying the binary file
void binarioNaTela(char *filename){
    // string that has the .bin filepath (inside the "binaries" directory)
    char *filepath = get_filepath(filename, 'b');

    unsigned long i, cs;
    unsigned char *mb;
    size_t fl;
    FILE *fs;
    if(filepath == NULL || !(fs = fopen(filepath, "rb"))) {
        fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
        return;
    }
    fseek(fs, 0, SEEK_END);
    fl = ftell(fs);
    fseek(fs, 0, SEEK_SET);
    mb = (unsigned char *) malloc(fl);
    fread(mb, 1, fl, fs);

    cs = 0;
    for(i = 0; i < fl; i++) {
        cs += (unsigned long) mb[i];
    }
    printf("%lf\n", (cs / (double) 100));
    free(mb);
    free(filepath);
    fclose(fs);
}

// reads a string input surrounded by double quotes (saves it to str)
void scan_quote_string(char *str) {
    /*
     *	Use essa função para ler um campo string delimitado entre aspas (").
     *	Chame ela na hora que for ler tal campo. Por exemplo:
     *
     *	A entrada está da seguinte forma:
     *		nomeDoCampo "MARIA DA SILVA"
     *
     *	Para ler isso para as strings já alocadas str1 e str2 do seu programa, você faz:
     *		scanf("%s", str1); // Vai salvar nomeDoCampo em str1
     *		scan_quote_string(str2); // Vai salvar MARIA DA SILVA em str2 (sem as aspas)
     *
     */

    char R;

    while((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...

    if(R == 'N' || R == 'n') { // campo NULO
        getchar(); getchar(); getchar(); // ignorar o "ULO" de NULO.
        strcpy(str, ""); // copia string vazia
    } else if(R == '\"') {
        if(scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
            strcpy(str, "");
        }
        getchar(); // ignorar aspas fechando
    } else if(R != EOF){ // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então, pois deve ser algum inteiro ou algo assim...
        str[0] = R;
        scanf("%s", &str[1]);
    } else { // EOF
        strcpy(str, "");
    }
}

// converts a vehicle prefix to an int
int convertePrefixo(char* str) {
    /* O registro que tem essa string como chave foi removido */
    if(str[0] == '*')
        return -1;

    /* Começamos com o primeiro digito na ordem de 36^0 = 1 */
    int power = 1;

    /* Faz a conversão char por char para chegar ao resultado */
    int result = 0;
    for(int i = 0; i < 5; i++) {

        /*
           Interpreta o char atual como se fosse um digito
           em base 36. Os digitos da base 36 são:
           0, 1, 2, 3, 4, 5, 6, 7, 8, 9, A, B, C, D,
           E, F, G, H, I, J, K, L, M, N, O, P, Q, R,
           S, T, U, V, W, X, Y, Z
           */
        int cur_digit;
        /* Checa pelos digitos normais e os converte para números */
        if(str[i] >= '0' && str[i] <= '9')
            cur_digit = str[i] - '0';
        /* Checa pelas letras e as converte para números */
        else if(str[i] >= 'A' && str[i] <= 'Z')
            cur_digit = 10 + str[i] - 'A';

        /*
           Multiplica o digito atual pelo ordem da posição atual
           e adiciona no resultado
           Primeira posição:   36^0 = 1
           Segunda posição:    36^1 = 36
           Terceira posição:   36^2 = 1.296
           Quarta posição:     36^3 = 46.656
           Quinta posição:     36^4 = 1.679.616
           */
        result += cur_digit * power;

        /* Aumenta a ordem atual */
        power *= 36;

    }

    return result;
}
