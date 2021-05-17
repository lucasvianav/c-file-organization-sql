#include <stdio.h>
#include <stdlib.h>

int main(void) {
    int funcionalidade, valor;
    char *string1, *string2;

    scanf("%d", &funcionalidade);

    if(funcionalidade == 1){
        scanf("%s %s", string1, string2);

        /*Saída caso o programa seja executado com sucesso:Listar  o  arquivode  saídaveiculo.binno  formato  binário  usando  a função fornecida binarioNaTela.
        
        Mensagem de saída caso algum erro seja encontrado:Falha no processamentodo arquivo.*/

    }
    else if(funcionalidade == 2){
        scanf("%s %s", string1, string2);

        /*Saída caso o programa seja executado com sucesso:Listar  o  arquivo  de  saída veiculo.binno  formato  binário  usando  a função fornecida binarioNaTela.
        
        Mensagem de saída caso algum erro seja encontrado:Falha no processamentodo arquivo.*/
    }
    else if(funcionalidade == 3){
        scanf("%s", string1);

        /*Saída caso o programa seja executado com sucesso:Para cada registro, devem ser exibidos seus campos da seguinte forma. Cada  campo  deve  ser  exibido  em  uma  linha  diferente.  Primeiro,  deve ser  colocado  o  valor  do  metadado  para  aquele  campo,  e  depois  o  seu valor.  Campos  com  valores  nulos  devem  ser  representados  por  'campo com  valor  nulo'. A  data  deve  ser  exibida  na  forma  corrente  de escrita. Depois de cada registro, pule uma linha em branco.A ordem de  exibição  doscampos  dosregistrosé  ilustrada  no exemplo  de execução.
        
        Mensagem de saída caso não existam registros:Registro inexistente.
        
        Mensagem de saída caso algum erro seja encontrado:Falha no processamento do arquivo.*/
    }
    else if(funcionalidade == 4){
        scanf("%s", string1);

        /*Saída caso o programa seja executado com sucesso:Para cada registro, devem ser exibidos seus campos da seguinte forma. Cada  campo  deve  ser  exibido  em  uma  linha  diferente.  Primeiro,  deve ser  colocado  o  valor  do  metadado  para  aquele  campo,  e  depois  o  seu valor.  Campos  com  valores  nulos  devem  ser  representados  por  'campo com valor nulo'. Com relação ao campo aceita cartão, seu valor deve ser  exibido  como PAGAMENTO  SOMENTE  COM  CARTAO  SEM  PRESENCA  DECOBRADOR(ao invés de S) ou PAGAMENTO EM CARTAO E DINHEIRO(ao invés de N)ou PAGAMENTO EM CARTAO SOMENTE NO FINAL DE SEMANA (ao invés de F). Depois  de  cada  registro,  pule  uma  linha  em  branco.A  ordem  de exibição doscampos dosregistrosé ilustrada no exemplo de execução.
        
        Mensagem de saída caso não existam registros:Registro inexistente.
        
        Mensagem de saída caso algum erro seja encontrado:Falha no processamento do arquivo.*/
    }
    else if(funcionalidade == 5){
        scanf("%s %s %d", string1, string2, &valor);

        /*Saída caso o programa seja executado com sucesso:Para cada registro, devem ser exibidos seus campos da seguinte forma. Cada  campo  deve  ser  exibido  em  uma  linha  diferente.  Primeiro,  deve ser  colocado  o  valor  do  metadado  para  aquele  campo,  e  depois  o  seu valor. Campos com valores nulos devem ser representados por campo com valor nulo'. A  data  deve  ser  exibida  na  forma  corrente  de  escrita. Depois  de  cada  registro,  pule  uma  linha  em  branco.A  ordem  de exibição doscampos dosregistrosé ilustrada no exemplo de execução.
        
        Mensagem  de  saída  caso  não  seja  encontrado  o  registro  que  contém  o  valor  do campo ou o campo pertence a um registro que esteja removido:Registro inexistente.
        
        Mensagem de saída caso algum erro seja encontrado:Falha no processamento do arquivo.*/
    }
    else if(funcionalidade == 6){
        scanf("%s %s %d", string1, string2, &valor);

        /*Saída caso o programa seja executado com sucesso:Para cada registro, devem ser exibidos seus campos da seguinte forma. Cada  campo  deve  ser  exibido  em  uma  linha  diferente.  Primeiro,  deve ser  colocado  o  valor  do metadado  para  aquele  campo,  e  depois  o  seu valor.  Campos  com  valores  nulos  devem  ser  representados  por  'campo com valor nulo'. Com relação ao campo aceita cartão, seu valor deve ser  exibido  como PAGAMENTO  SOMENTE  COM  CARTAO  SEM  PRESENCA  DE COBRADOR (ao invés de S) ou PAGAMENTO EM CARTAO E DINHEIRO (ao invés de N) ou PAGAMENTO EM CARTAO SOMENTE NO FINAL DE SEMANA (ao invés de F).Depois  de  cada  registro,  pule  uma  linha  em  branco.A  ordem  de exibição doscampos dosregistrosé ilustrada no exemplo de execução.
        
        Mensagem  de  saída  caso  não  seja  encontrado  o  registro  que  contém  o  valor  do campo ou o campo pertence a um registro que esteja removido:Registro inexistente.
        
        Mensagem de saída caso algum erro seja encontrado:Falha no processamento do arquivo.*/
    }
    else if(funcionalidade == 7){
        scanf("%s %d", string1, &valor);

        //receber mais infos usando scanf e while

        /*Saída caso o programa seja executado com sucesso:Listar o arquivo binário veiculo.bin usando a função binarioNaTela.
        
        Mensagem de saída caso algum erro seja encontrado:Falha no processamento do arquivo.*/
    }
    else if(funcionalidade == 8){
        scanf("%s %s %d", string1, string2, &valor);

        //receber mais infos usando scanf e while

        /*Saída caso o programa seja executado com sucesso:Listar o arquivo binário linha.bin usando a função binarioNaTela.
        
        Mensagem de saída caso algum erro seja encontrado:Falha no processamento do arquivo.*/
    }

    return 0;
}