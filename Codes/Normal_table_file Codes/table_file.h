#ifndef TABLE_FILE_H    // Include guard
#define TABLE_FILE_H

#include <stddef.h>    // For size_t

int create_table_file(char *filename, int num_rows, int num_columns, int record_size);

int delete_table_file(char*filename);

int open_table_file(char*filename, int row_index, int column_index);

#endif // TABLE_FILE_H
