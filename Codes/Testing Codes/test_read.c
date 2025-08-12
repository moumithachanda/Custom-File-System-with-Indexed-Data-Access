#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <table_file.h>

int main() {
    char filename[100], row_name[100], column_name[100];
    printf("Enter the file name: ");
    scanf("%99s", filename);
    printf("Enter the row name: ");
    scanf("%99s", row_name);
    printf("Enter the column name: ");
    scanf("%99s", column_name);

    struct File_ID fid = open_string_table_file(filename, row_name, column_name);
    if (fid.fd == -1) {
        fprintf(stderr, "Failed to open file.\n");
        return 1;
    }

    void *record = read_table(fid);
    if (record) {
        printf("Record: %s\n", (char *)record);
        free(record);
    } else {
        fprintf(stderr, "Failed to read the record.\n");
    }

    close(fid.fd);
    return 0;
}
