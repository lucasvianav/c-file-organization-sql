#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "./structs.c"
#include "IO.h"

char* finaliza_string(char *string, int n){
    char *string2 = (char *) malloc(sizeof(char)*(n+1)); 
    strcpy(string2, string);
    //string2[n] = '\0';
    strcpy(&string2[n], "\0");

    if(strcmp(string2,"")==0){
        free(string2);
        string2 = (char *) malloc(sizeof(char)*(19));
        strcpy(string2, "campo com valor nulo");
    }

    return string2;
    //free(string2);
}

//0123456789
//2002-12-18
//29 de maio de 2009
/*char* data(char *string){
    char *string2 = (char *) malloc(sizeof(char)*(23));

    strcpy(&string2[0], string[8]);
    strcpy(&string2[1], string[9]);
    strcpy(&string2[2], " ");
    strcpy(&string2[3], "d");
    strcpy(&string2[4], "e");
    strcpy(&string2[5], " ");
    if((strcmp(&string[5],"0") == 0) && (strcmp(&string[6],"1") == 0)){
        strcpy(&string2[6], "j");
        strcpy(&string2[7], "a");
        strcpy(&string2[8], "n");
        strcpy(&string2[9], "e");
        strcpy(&string2[10], "i");
        strcpy(&string2[11], "r");
        strcpy(&string2[12], "o");
        strcpy(&string2[13], " ");
        strcpy(&string2[14], "d");
        strcpy(&string2[15], "e");
        strcpy(&string2[16], " ");
        strcpy(&string2[17], string[0]);
        strcpy(&string2[18], string[1]);
        strcpy(&string2[19], string[2]);
        strcpy(&string2[20], string[3]);
    }
    if((strcmp(&string[5],"0") == 0) && (strcmp(&string[6],"2") == 0)){
        strcpy(&string2[6], "f");
        strcpy(&string2[7], "e");
        strcpy(&string2[8], "v");
        strcpy(&string2[9], "e");
        strcpy(&string2[10], "r");
        strcpy(&string2[11], "e");
        strcpy(&string2[12], "i");
        strcpy(&string2[11], "r");
        strcpy(&string2[12], "o");
        strcpy(&string2[13], " ");
        strcpy(&string2[14], "d");
        strcpy(&string2[15], "e");
        strcpy(&string2[16], " ");
        strcpy(&string2[17], string[0]);
        strcpy(&string2[18], string[1]);
        strcpy(&string2[19], string[2]);
        strcpy(&string2[20], string[3]);
    }

    strcpy(string2, string);
    strcpy(&string2[n], "\0");
    return string2;
    //free(string2);
}*/

void impressao(char *filename){
    FILE *arquivo;
    vehicle_header header;
    vehicle_register dado;

    arquivo = fopen(filename, "rb"); 

    if(arquivo == NULL){
        printf("Falha no processamento do arquivo.");
    }

    fread(&header.status, sizeof(char), 1, arquivo);
    if(strcmp(&header.status, "0")==0){
        printf("Falha no processamento do arquivo.");
    }
    else{
        fread(&header.byteProxReg, sizeof(long long), 1, arquivo);
        fread(&header.nroRegistros, sizeof(int), 1, arquivo);
        if(header.nroRegistros == 0){
            printf("Registro inexistente.");
        }
        else{
            fread(&header.nroRegRemovidos, sizeof(int), 1, arquivo);
            fread(header.descrevePrefixo, sizeof(char), 18, arquivo);
            fread(header.descreveData, sizeof(char), 35, arquivo);
            fread(header.descreveLugares, sizeof(char), 42, arquivo);
            fread(header.descreveLinha, sizeof(char), 26, arquivo);
            fread(header.descreveModelo, sizeof(char), 17, arquivo);
            fread(header.descreveCategoria, sizeof(char), 20, arquivo);

            //corrigir campo nulo e data
            //: 29 de maio de 2009
            //: campo com valor nulo
            //for(int i = 0; i<header.nroRegistros; i++){
            for(int i = 0; i<2; i++){
                fread(&dado.removido, sizeof(char), 1, arquivo);
                fread(&dado.tamanhoRegistro, sizeof(int), 1, arquivo);
                fread(dado.prefixo, sizeof(char), 5, arquivo);
                fread(dado.data, sizeof(char), 10, arquivo);
                fread(&dado.quantidadeLugares, sizeof(int), 1, arquivo);
                fread(&dado.codLinha, sizeof(int), 1, arquivo);
                fread(&dado.tamanhoModelo, sizeof(int), 1, arquivo);
                dado.modelo = (char *) malloc(sizeof(char)*dado.tamanhoModelo);
                fread(dado.modelo, sizeof(char), dado.tamanhoModelo, arquivo);
                fread(&dado.tamanhoCategoria, sizeof(int), 1, arquivo);
                dado.categoria = (char *) malloc(sizeof(char)*dado.tamanhoCategoria);
                fread(dado.categoria, sizeof(char), dado.tamanhoCategoria, arquivo);

                printf("%s: %s\n", finaliza_string(header.descrevePrefixo, sizeof(header.descrevePrefixo)), finaliza_string(dado.prefixo, sizeof(dado.prefixo)));
                printf("%s: %s\n", finaliza_string(header.descreveModelo, sizeof(header.descreveModelo)), finaliza_string(dado.modelo, sizeof(dado.modelo)));
                printf("%s: %s\n", finaliza_string(header.descreveCategoria, sizeof(header.descreveCategoria)), finaliza_string(dado.categoria, sizeof(dado.categoria)));
                printf("%s: %s\n", finaliza_string(header.descreveData, sizeof(header.descreveData)), finaliza_string(dado.data, sizeof(dado.data)));
                printf("%s: %d\n", finaliza_string(header.descreveLugares, sizeof(header.descreveLugares)), dado.quantidadeLugares);
                
                printf("\n");

                free(dado.modelo);
                free(dado.categoria);
            }
        }
    }

    fclose(arquivo);
}

char* cartao(char *string){
    char *string2;
    if(strcmp(&string[0], "S")==0){
        string2 = (char *) malloc(sizeof(char)*(52)); 
        strcpy(string2, "PAGAMENTO SOMENTE COM CARTAO SEM PRESENCA DE COBRADOR");
    }
    else if(strcmp(&string[0], "N")==0){
        string2 = (char *) malloc(sizeof(char)*(29)); 
        strcpy(string2, "PAGAMENTO EM CARTAO E DINHEIRO");
    }
    else if(strcmp(&string[0], "F")==0){
        string2 = (char *) malloc(sizeof(char)*(45));
        strcpy(string2, "PAGAMENTO EM CARTAO SOMENTE NO FINAL DE SEMANA"); 
    }

    return string2;
    //free(string2);
}

void impressao2(char *filename){
    FILE *arquivo;
    line_header header;
    line_register dado;

    arquivo = fopen(filename, "rb"); 

    if(arquivo == NULL){
        printf("Falha no processamento do arquivo.");
    }

    fread(&header.status, sizeof(char), 1, arquivo);
    if(strcmp(&header.status, "0")==0){
        printf("Falha no processamento do arquivo.");
    }
    else{
        fread(&header.byteProxReg, sizeof(long long), 1, arquivo);
        fread(&header.nroRegistros, sizeof(int), 1, arquivo);
        if(header.nroRegistros == 0){
            printf("Registro inexistente.");
        }
        else{
            fread(&header.nroRegRemovidos, sizeof(int), 1, arquivo);
            fread(header.descreveCodigo, sizeof(char), 15, arquivo);
            fread(header.descreveCartao, sizeof(char), 13, arquivo);
            fread(header.descreveNome, sizeof(char), 13, arquivo);
            fread(header.descreveCor, sizeof(char), 24, arquivo);

            //corrigir campo nulo
            //: campo com valor nulo
            //for(int i = 0; i<header.nroRegistros; i++){
            for(int i = 0; i<2; i++){
                fread(&dado.removido, sizeof(char), 1, arquivo);
                fread(&dado.tamanhoRegistro, sizeof(int), 1, arquivo);
                fread(&dado.codLinha, sizeof(int), 1, arquivo);
                fread(&dado.aceitaCartao, sizeof(char), 1, arquivo);
                fread(&dado.tamanhoNome, sizeof(int), 1, arquivo);
                dado.nomeLinha = (char *) malloc(sizeof(char)*dado.tamanhoNome);
                fread(dado.nomeLinha, sizeof(char), dado.tamanhoNome, arquivo);
                fread(&dado.tamanhoCor, sizeof(int), 1, arquivo);
                dado.corLinha = (char *) malloc(sizeof(char)*dado.tamanhoCor);
                fread(dado.corLinha, sizeof(char), dado.tamanhoCor, arquivo);

                printf("%s: %d\n", finaliza_string(header.descreveCodigo, sizeof(header.descreveCodigo)), dado.codLinha);
                printf("%s: %s\n", finaliza_string(header.descreveNome, sizeof(header.descreveNome)), finaliza_string(dado.nomeLinha, sizeof(dado.nomeLinha)));
                printf("%s: %s\n", finaliza_string(header.descreveCor, sizeof(header.descreveCor)), finaliza_string(dado.corLinha, sizeof(dado.corLinha)));
                printf("%s: %s\n", finaliza_string(header.descreveCartao, sizeof(header.descreveCartao)), cartao(&dado.aceitaCartao));
                
                printf("\n");

                free(dado.nomeLinha);
                free(dado.corLinha);
            }
        }
    }

    fclose(arquivo);
}

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

void busca(char *filename, char *campo, char *valor){
    FILE *arquivo;
    vehicle_header header;
    vehicle_register dado;

    arquivo = fopen(filename, "rb"); 

    if(arquivo == NULL){
        printf("Falha no processamento do arquivo.");
    }

    fread(&header.status, sizeof(char), 1, arquivo);
    if(strcmp(&header.status, "0")==0){
        printf("Falha no processamento do arquivo.");
    }
    else{
        fread(&header.byteProxReg, sizeof(long long), 1, arquivo);
        fread(&header.nroRegistros, sizeof(int), 1, arquivo);
        if(header.nroRegistros == 0){
            printf("Registro inexistente.");
        }
        else{
            fread(&header.nroRegRemovidos, sizeof(int), 1, arquivo);
            fread(header.descrevePrefixo, sizeof(char), 18, arquivo);
            fread(header.descreveData, sizeof(char), 35, arquivo);
            fread(header.descreveLugares, sizeof(char), 42, arquivo);
            fread(header.descreveLinha, sizeof(char), 26, arquivo);
            fread(header.descreveModelo, sizeof(char), 17, arquivo);
            fread(header.descreveCategoria, sizeof(char), 20, arquivo);

            //corrigir campo nulo e data
            //: 29 de maio de 2009
            //: campo com valor nulo
            //for(int i = 0; i<header.nroRegistros; i++){
            for(int i = 0; i<2; i++){
                fread(&dado.removido, sizeof(char), 1, arquivo);
                fread(&dado.tamanhoRegistro, sizeof(int), 1, arquivo);
                fread(dado.prefixo, sizeof(char), 5, arquivo);
                fread(dado.data, sizeof(char), 10, arquivo);
                fread(&dado.quantidadeLugares, sizeof(int), 1, arquivo);
                fread(&dado.codLinha, sizeof(int), 1, arquivo);
                fread(&dado.tamanhoModelo, sizeof(int), 1, arquivo);
                dado.modelo = (char *) malloc(sizeof(char)*dado.tamanhoModelo);
                fread(dado.modelo, sizeof(char), dado.tamanhoModelo, arquivo);
                fread(&dado.tamanhoCategoria, sizeof(int), 1, arquivo);
                dado.categoria = (char *) malloc(sizeof(char)*dado.tamanhoCategoria);
                fread(dado.categoria, sizeof(char), dado.tamanhoCategoria, arquivo);

                
                //prefixo, data, quantidadeLugares, modelo, categoria
                /*if(strcmp(campo, header.descrevePrefixo) == 0){
                    if (strcmp(dado.prefixo, valor)== 0){
                        printf("%s: %s\n", finaliza_string(header.descrevePrefixo, sizeof(header.descrevePrefixo)), finaliza_string(dado.prefixo, sizeof(dado.prefixo)));
                        printf("%s: %s\n", finaliza_string(header.descreveModelo, sizeof(header.descreveModelo)), finaliza_string(dado.modelo, sizeof(dado.modelo)));
                        printf("%s: %s\n", finaliza_string(header.descreveCategoria, sizeof(header.descreveCategoria)), finaliza_string(dado.categoria, sizeof(dado.categoria)));
                        printf("%s: %s\n", finaliza_string(header.descreveData, sizeof(header.descreveData)), finaliza_string(dado.data, sizeof(dado.data)));
                        printf("%s: %d\n", finaliza_string(header.descreveLugares, sizeof(header.descreveLugares)), dado.quantidadeLugares);
                        
                        printf("\n");
                    }
                }*/

                free(dado.modelo);
                free(dado.categoria);
            }
        }
    }

    fclose(arquivo);
}

void busca2(char *filename, char *campo, char *valor){
    FILE *arquivo;
    line_header header;
    line_register dado;

    arquivo = fopen(filename, "rb"); 

    if(arquivo == NULL){
        printf("Falha no processamento do arquivo.");
    }

    fread(&header.status, sizeof(char), 1, arquivo);
    if(strcmp(&header.status, "0")==0){
        printf("Falha no processamento do arquivo.");
    }
    else{
        fread(&header.byteProxReg, sizeof(long long), 1, arquivo);
        fread(&header.nroRegistros, sizeof(int), 1, arquivo);
        if(header.nroRegistros == 0){
            printf("Registro inexistente.");
        }
        else{
            fread(&header.nroRegRemovidos, sizeof(int), 1, arquivo);
            fread(header.descreveCodigo, sizeof(char), 15, arquivo);
            fread(header.descreveCartao, sizeof(char), 13, arquivo);
            fread(header.descreveNome, sizeof(char), 13, arquivo);
            fread(header.descreveCor, sizeof(char), 24, arquivo);

            //corrigir campo nulo
            //: campo com valor nulo
            //for(int i = 0; i<header.nroRegistros; i++){
            for(int i = 0; i<2; i++){
                fread(&dado.removido, sizeof(char), 1, arquivo);
                fread(&dado.tamanhoRegistro, sizeof(int), 1, arquivo);
                fread(&dado.codLinha, sizeof(int), 1, arquivo);
                fread(&dado.aceitaCartao, sizeof(char), 1, arquivo);
                fread(&dado.tamanhoNome, sizeof(int), 1, arquivo);
                dado.nomeLinha = (char *) malloc(sizeof(char)*dado.tamanhoNome);
                fread(dado.nomeLinha, sizeof(char), dado.tamanhoNome, arquivo);
                fread(&dado.tamanhoCor, sizeof(int), 1, arquivo);
                dado.corLinha = (char *) malloc(sizeof(char)*dado.tamanhoCor);
                fread(dado.corLinha, sizeof(char), dado.tamanhoCor, arquivo);

                // codLinha, aceitaCartao, nomeLinha, corLinha
                /*if(strcmp(campo, header.descreveCodigo) == 0){
                    if (strcmp(dado.codLinha, valor)== 0){
                        printf("%s: %d\n", finaliza_string(header.descreveCodigo, sizeof(header.descreveCodigo)), dado.codLinha);
                        printf("%s: %s\n", finaliza_string(header.descreveNome, sizeof(header.descreveNome)), finaliza_string(dado.nomeLinha, sizeof(dado.nomeLinha)));
                        printf("%s: %s\n", finaliza_string(header.descreveCor, sizeof(header.descreveCor)), finaliza_string(dado.corLinha, sizeof(dado.corLinha)));
                        printf("%s: %s\n", finaliza_string(header.descreveCartao, sizeof(header.descreveCartao)), cartao(&dado.aceitaCartao));
                        
                        printf("\n");
                    }
                }*/

                free(dado.nomeLinha);
                free(dado.corLinha);
            }
        }
    }

    fclose(arquivo);
}