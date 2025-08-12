#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include "table_file.h"

struct int_pair{
    int fd;
    int rec_size;
}
;

int create_table_file(char *filename, int num_rows,int num_columns,int record_size){ // Time complexity = O(1)
 
    size_t file_size = num_rows*num_columns*record_size;
    int fd = open(filename,O_WRONLY|O_CREAT|O_TRUNC,0644);
    if(fd==-1){
        fprintf(stderr,"Error in creating file %s\n",filename,strerror(errno));
        return -1;
    }
    int status = ftruncate(fd,file_size);
    if(status==-1){
        fprintf(stderr,"Error in creating file %s\n",filename,strerror(errno));
        if(close(fd)==-1){
            fprintf(stderr,"Error in closing file %s\n",filename,strerror(errno));
            return -1;
        }
        return -1;
    }
    if(close(fd)==-1){
        fprintf(stderr,"Error in closing file %s\n",filename,strerror(errno));
        return -1;
    }
    char*index_suffix = "_index.txt";
    size_t index_file_len = strlen(filename)+strlen(index_suffix)+1;
    char* index_filename = malloc(index_file_len);
    if(index_filename==NULL){
        fprintf(stderr,"Unable to create index file of %s\n",filename,strerror(errno));
        return -1;
    }
    snprintf(index_filename,index_file_len,"%s%s",filename,index_suffix);

    int index_fd = open(index_filename,O_WRONLY|O_CREAT,0644);
    if(index_fd==-1){
        fprintf(stderr,"Error in creating file %s\n",index_filename,strerror(errno));
        return -1;
    }
    char buffer[50];
    snprintf(buffer,sizeof(buffer),"%d %d %d\n",num_rows,num_columns,record_size);
    ssize_t bytes_written = write(index_fd,buffer,strlen(buffer));
    if(bytes_written==-1){
        fprintf(stderr,"Error in writing to file %s\n",index_filename,strerror(errno));
        close(index_fd);
        return -1;
    }

    if(close(index_fd)==-1){
        fprintf(stderr,"Error in closing file %s\n",index_filename,strerror(errno));
        return -1;
    }
    fprintf(stdout,"Successfully created files %s & %s\n",filename,index_filename);
    return 0;
    

}

int delete_table_file(char*filename){ //Time complexity = O(1)
    char*index_suffix = "_index.txt";
    size_t index_file_len = strlen(filename)+strlen(index_suffix)+1;
    char* index_filename = malloc(index_file_len);
    if(index_filename==NULL){
        fprintf(stderr,"Unable to create for index file of %s\n",filename,strerror(errno));
        return -1;
    }
    snprintf(index_filename,index_file_len,"%s%s",filename,index_suffix);
    if(remove(filename)!=0){
        fprintf(stderr,"Unable to delete file %s\n",filename,strerror(errno));
        return -1;
    }
    if(remove(index_filename)!=0){
        fprintf(stderr,"Unable to delete index file of %s\n",filename,strerror(errno));
        return -1;
    }
    printf("successfully deleted files %s & %s\n",filename,index_filename);
    return 0;
}

struct int_pair open_table_file(char* filename,int rindex,int cindex){ //Time complexity = O(1)
    struct int_pair error_pair;
    error_pair.fd = -1;
    error_pair.rec_size = 0;
    int fd = open(filename,O_RDWR);
    if(fd==-1){
        fprintf(stderr,"Unable to open table file %s\n",filename,strerror(errno));
        return error_pair;
    }
    char*index_suffix = "_index.txt";
    size_t index_file_len = strlen(filename)+strlen(index_suffix)+1;
    char* index_filename = malloc(index_file_len);
    if(index_filename==NULL){
        fprintf(stderr,"Unable to create for index file of %s\n",filename,strerror(errno));
        close(fd);
        return error_pair;
    }
    snprintf(index_filename,index_file_len,"%s%s",filename,index_suffix);
    int index_fd = open(index_filename,O_RDONLY,0644);
    if(index_fd==-1){
        fprintf(stderr,"Error in opening file %s\n",index_filename,strerror(errno));
        return error_pair;
    }
    char buffer[50];
    ssize_t bytes_read = read(index_fd,buffer,sizeof(buffer)-1);
    if(bytes_read==-1){
        fprintf(stderr,"Error in reading from file %s\n",index_filename,strerror(errno));
        close(fd);
        close(index_fd);
        return error_pair;
    }
    buffer[bytes_read] = '\0';

    if(close(index_fd)==-1){
        fprintf(stderr,"Error in closing file %s\n",index_filename,strerror(errno));
        close(fd);
        return error_pair;
    }
    int num_rows,num_cols,rec_size;
    if(sscanf(buffer,"%d %d %d",&num_rows,&num_cols,&rec_size)!=3){
        fprintf(stderr, "Error: File does not contain three numbers in the expected format\n");
        close(fd);
        return error_pair;
    }
    printf("numbers got from index file %d %d %d\n",num_rows,num_cols,rec_size);
    if(rindex>=num_rows||cindex>=num_cols){
        fprintf(stderr, "Given row and column indices exceed the file size\n");
        close(fd);
        return error_pair;
    }
    off_t byte_offset = ((rindex*num_cols)+cindex)*rec_size;
    if(lseek(fd,byte_offset,SEEK_SET)==-1){
        fprintf(stderr,"Error in opening file %s\n",index_filename,strerror(errno));
        close(fd);
        return error_pair;
    }
    struct int_pair myp;
    myp.fd = fd;
    myp.rec_size = rec_size;
    return myp;


}