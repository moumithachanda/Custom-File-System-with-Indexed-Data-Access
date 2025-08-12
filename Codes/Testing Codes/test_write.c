#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <table_file.h>

int main() {
    char filename[100], row_name[100], column_name[100], input[100];
    printf("Enter the file name: ");
    scanf("%99s", filename);
    printf("Enter the row name: ");
    scanf("%99s", row_name);
    printf("Enter the column name: ");
    scanf("%99s", column_name);

    printf("Enter the string to write: ");
    scanf(" %99[^\n]", input);

    struct File_ID fid = open_string_table_file(filename, row_name, column_name);
    if (fid.fd == -1) {
        fprintf(stderr, "Failed to open file.\n");
        return 1;
    }

    size_t size = strlen(input) + 1; // Include null terminator
    if (write_table(fid, input, size) == -1) {
        fprintf(stderr, "Failed to write to the record.\n");
    } else {
        printf("Successfully wrote to the record.\n");
    }

    close(fid.fd);
    return 0;
}