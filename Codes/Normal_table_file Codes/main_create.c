#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <table_file.h>

int main(int argc,char*argv[]){
    char*filename = argv[1];
    int r = atoi(argv[2]);
    int c = atoi(argv[3]);
    int rec = atoi(argv[4]);
    //fprintf(stdout,"%d %d %d %s\n",nr,nc,rs,filename);
    int status = create_table_file(filename,r,c,rec);
    return 0;
}