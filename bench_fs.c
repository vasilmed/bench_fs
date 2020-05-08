#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include "error.h"
#include <stdlib.h>
#include <getopt.h>
#include <dirent.h>


extern char **environ;
extern char *optarg;


void 
get_random_string(char *src,int size)
{
    srand(clock());
    if(src == NULL)
        err_exit("No allocate string array");
    char *dict = "0123456789abcdefghijklmnoprstqwxyzABCDEFGHIJKLMNOPRSTQWXYZ";
    int dict_len = strlen(dict);
    int i;
    for(i = 0; i < size; i++){
        src[i] = dict[rand()%dict_len];
    }
    src[i] = 0;
}

char* 
str_cat(char* src1,char* src2)
{
    int src1_len = strlen(src1);
    int src2_len = strlen(src2);
    char* tmp_str = malloc(src1_len+src2_len+1);
    int i;
    for(i = 0; i < src1_len;i++) {
        tmp_str[i] = src1[i];
    }
    for(i = 0; i < src2_len; i++) {
        tmp_str[i+src1_len] = src2[i];
    }
    tmp_str[src1_len+i] = 0;
    return tmp_str;
}

void
create_rand_files(char *path,int n)
{   
    char *rand_name = malloc(sizeof(char)*11);
    char *tmp_name;
    int i;
    for(i = 0; i < n;i++) {
        get_random_string(rand_name,10);
        tmp_name = str_cat(path,rand_name);
        if (i < 10)
            printf("%s\n",tmp_name);
        int fd = open(tmp_name,O_RDWR|O_CREAT|O_TRUNC|O_SYNC,S_IRUSR|S_IWUSR);
        write(fd,"a",1);
        close(fd);
        free(tmp_name);
    }
    
    free(rand_name);
    
}

int 
funccmp( const void * val1, const void * val2 )
{
    return strcmp((char*)val1,(char*)val2);
}

int
delete_files(char *path,int nf_num)
{
    if(!nf_num)
        return -1;
    int i = 0;
    char **files = malloc(sizeof(char*)*nf_num);
    DIR* dir = opendir(path);
    if(dir == NULL)
        perror("Error oprn dir");
    struct dirent* entry;
    char buffer[100];
    while((entry = readdir(dir))) {
        if(strcmp(entry->d_name,".") == 0 || strcmp(entry->d_name,"..") == 0 || strcmp(entry->d_name,"lost+found") == 0)
            continue;
        char* tmp = malloc(strlen(entry->d_name)+1);
        if (tmp == NULL)
            perror("Malloc tmp filename");
        strcpy(tmp,entry->d_name);
        files[i++] = tmp;
    }
    closedir(dir);
    qsort(files,nf_num,sizeof(char*),funccmp);
    for(i = 0; i < nf_num; i++) {
        char *tmp_file = str_cat(path,files[i]);
        unlink(tmp_file);
        free(files[i]);
        free(tmp_file);
    }
    free(files); 
    return 0;
}

void
test_filesystem(char *fs_name,int nf_num)
{
    char buffer[100];
    sprintf(buffer,"/tmp/%s_temp/",fs_name);
    long start,end;
    printf("Starting test %s....\n",fs_name);
    printf("Create file test...\n");
    start = clock();
    create_rand_files(buffer,nf_num);
    end = clock() - start;
    printf("End create file test, time: %i\n\n",end);
    printf("delete order files test...\n");
    start = clock();
    delete_files(buffer,nf_num);
    end = clock() - start;
    printf("End delete order files test, time: %i\n\n",end);
}

int
main(int argc,char **argv)
{
    
    char* nf = getenv("NF");
    int nf_num;
    int res = 0;
    int i;
    while((res = getopt(argc,argv,"h:")) != -1) {
        switch(res) {
            case 'h':
                printf("Please. set NF value using  'export NF=n' , where n is count of files in each fs.\n For example: NF=n ./program <fs_type> <fs_type> ...\n");
                printf("<fs_type> can be like: btrfs, ext4,xfs\n");
                exit(0);
        }
    }
    if(nf)
        nf_num = atoi(nf);
    else {
        printf("Please type -h for more info\n");
        exit(0);
    }
    for(i = 1; i < argc; i++) {
        test_filesystem(argv[i],nf_num);
    }
    exit(0);

}


