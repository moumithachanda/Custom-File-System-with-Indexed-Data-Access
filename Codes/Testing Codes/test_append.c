#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <table_file.h>
#include <unistd.h>

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

    printf("Enter string by string (type 'exit' to finish):\n");
    char input[fid.rec_size];
    while (1) {
        printf("> ");
        scanf(" %s", input);

        if (strcmp(input, "exit") == 0) {
			input[0]='\0';
			write_table(fid, input, 1);
            break;
        }

        if (write_table(fid, input, strlen(input)) == -1) {			//not adding null terminator
            fprintf(stderr, "Error: Failed to write record.\n");
        } else {
            printf("Record appended successfully.\n");
        }
    }

    close(fid.fd);
    return 0;
}
