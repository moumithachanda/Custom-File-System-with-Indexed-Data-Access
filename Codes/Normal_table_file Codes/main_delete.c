#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <table_file.h>

int main(int argc,char*argv[]){
    char*filename = argv[1];
    //fprintf(stdout,"%d %d %d %s\n",nr,nc,rs,filename);
    int status = delete_table_file(filename);
    return 0;
}