typedef struct {
    // list of rows in which each row is a list of strings
    // each string representing a value from the .csv file
    char ***list;

    // number of rows in csv_data->list
    int length;

    // number of values in a row (number of csv_data->list elements' elements)
    int row_length;

} csv_data;
