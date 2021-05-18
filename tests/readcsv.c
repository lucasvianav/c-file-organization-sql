#include <stdio.h>
#include "../IO.c"

int main(){
    csv_data *data = readCSV("veiculo.csv");
    print_csv_data(data);
    free_csv_data(&data);
    
    return 0;
}