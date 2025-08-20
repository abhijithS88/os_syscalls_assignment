#include <unistd.h> // write
#include <sys/stat.h> // mkdir,stat
#include <sys/types.h> // for mode_t
#include <cstring> // for using c-strings
#include <iostream> // for debugging
#include <stdio.h>
#include <errno.h> // for errno
#include <fcntl.h> // for open
#include <string.h>
#include <algorithm> // for min
#include <stdlib.h> // for strtoll
using namespace std;

// int mkdir(const char *pathname, mode_t mode);
// int stat(const char *pathname, struct stat *buf);
// void perror(const char *s);
// int open(const char *pathname, int flags);
// int open(const char *pathname, int flags, mode_t mode);
// S_IRUSR  // User read
// S_IWUSR  // User write
// S_IXUSR  // User execute
// S_IRGRP  // Group read
// S_IWGRP  // Group write
// S_IXGRP  // Group execute
// S_IROTH  // Others read
// S_IWOTH  // Others write
// S_IXOTH  // Others execute

void printPermissions(struct stat* st ,const char* file){
    // char vec[3][10] = {"User","Group","Others"};
    write(1,"User has read permissions on ",30);
    write(1,file,strlen(file));
    if(st->st_mode & S_IRUSR){
        write(1,": Yes\n",6);
    }
    else write(1,": No\n",5);
    write(1,"User has write permissions on ",31);
    write(1,file,strlen(file));
    if(st->st_mode & S_IWUSR){
        write(1,": Yes\n",6);
    }
    else write(1,": No\n",5);
    write(1,"User has execute permissions on ",33);
    write(1,file,strlen(file));
    if(st->st_mode & S_IXUSR){
        write(1,": Yes\n",6);
    }
    else write(1,": No\n",5);
    write(1,"Group has read permissions on ",31);
    write(1,file,strlen(file));
    if(st->st_mode & S_IRGRP){
        write(1,": Yes\n",6);
    }
    else write(1,": No\n",5);
    write(1,"Group has write permissions on ",32);
    write(1,file,strlen(file));
    if(st->st_mode & S_IWGRP){
        write(1,": Yes\n",6);
    }
    else write(1,": No\n",5);
    write(1,"Group has execute permissions on ",34);
    write(1,file,strlen(file));
    if(st->st_mode & S_IXGRP){
        write(1,": Yes\n",6);
    }
    else write(1,": No\n",5);
    write(1,"Others has read permissions on ",32);
    write(1,file,strlen(file));
    if(st->st_mode & S_IRGRP){
        write(1,": Yes\n",6);
    }
    else write(1,": No\n",5);
    write(1,"Others has write permissions on ",33);
    write(1,file,strlen(file));
    if(st->st_mode & S_IWGRP){
        write(1,": Yes\n",6);
    }
    else write(1,": No\n",5);
    write(1,"Others has execute permissions on ",35);
    write(1,file,strlen(file));
    if(st->st_mode & S_IXGRP){
        write(1,": Yes\n",6);
    }
    else write(1,": No\n",5);
}

bool checkFile(char* file){
    int cnt=0;
    while(file[cnt]!='\0'){
        cnt++;
    }
    if(cnt<4){
        return false;
    }
    if(file[cnt-1]!='t' or file[cnt-2]!='x' or file[cnt-3]!='t' or file[cnt-4]!='.'){
        return false;
    }
    return true;
}

bool checkDirectory(char* file){
    struct stat st; 
    if(stat(file,&st)==-1){
        perror("stat sys call error for directory");
        return false;
    }
    return true;
}

bool checkFileSizes(char* file1, char* file2){
    struct stat st1,st2;
    if(stat(file1,&st1)==-1 or stat(file2,&st2)==-1){
        perror("stat sys call error for input and output files");
        return false;
    }
    return st1.st_size == st2.st_size;
}

void flag0(char* output, char* input, char* block, bool &flag){
    int fd1 = open(output,O_RDONLY);
    int fd2 = open(input,O_RDONLY);
    if(fd1==-1){
        perror("open sys call failed for output file");
        return;
    }
    if(fd2==-1){
        perror("open sys call failed for input file");
        return;
    }
    struct stat st;
    if(stat(input,&st)==-1){
        perror("stat sys call error for input file");
        close(fd1);
        close(fd2);
        return;
    }
    char *endptr;
    long long int block_size = strtoll(block,&endptr,10);
    if(block_size <= 0 or *endptr!='\0'){
        write(1,"block size should be natural number\n",36);
        close(fd1);
        close(fd2);
        return;
    }
    block_size = min(block_size,(long long int)st.st_size);
    char* buffer1 = (char*)malloc(block_size);
    char* buffer2 = (char*)malloc(block_size);
    if(!buffer1 or !buffer2){
        perror("memory allocation failed");
        close(fd1);
        close(fd2);
        return;
    }
    long long rem = st.st_size;
    ssize_t n1,n2;
    while(rem>0){
        n1 = read(fd1,buffer1,min(block_size,rem));
        n2 = read(fd2,buffer2,min(block_size,rem));
        if(n1<0 or n2<0 or n1!=n2){
            perror("error in reading files");
            free(buffer1);
            free(buffer2);
            close(fd1);
            close(fd2);
            return;
        }
        if(n1==0 and n2==0)break;
        int flag=1;
        for(int i=0;i<n1;i++){
            if(buffer1[i]!=buffer2[n1-i-1]){
                flag=0;
            }
        }
        if(!flag){
            // write(1,"input and output files are not ok for given flag and constraints\n",66);
            free(buffer1);
            free(buffer2);
            close(fd1);
            close(fd2);
            return;
        }
        rem-=block_size;
    }
    // write(1,"input and output files are ok for given flag and constraints\n",62);
    flag = true;
    free(buffer1);
    free(buffer2);
    close(fd1);
    close(fd2);
}

void flag1(char* output, char* input, bool &flag){
    int fd1 = open(output,O_RDONLY);
    int fd2 = open(input,O_RDONLY);
    if(fd1==-1){
        perror("open sys call failed for output file");
        return;
    }
    if(fd2==-1){
        perror("open sys call failed for input file");
        return;
    }
    struct stat st;
    if(stat(input,&st)==-1){
        perror("stat sys call error for input file");
        close(fd1);
        close(fd2);
        return;
    }
    long long block_size = min(65536ll,(long long int)st.st_size);
    char* buffer1 = (char*)malloc(block_size);
    char* buffer2 = (char*)malloc(block_size);
    if(!buffer1 or !buffer2){
        perror("memory allocation failed");
        close(fd1);
        close(fd2);
        return;
    }
    long long rem = st.st_size;
    while(rem>0){
        size_t buffer_size = min((long long)65536,(long long)rem); 
        off_t offset = rem - buffer_size;
        if(lseek(fd1,offset,SEEK_SET)==-1){
            perror("lseek sys call failed");
            free(buffer1);
            free(buffer2);
            close(fd1);
            close(fd2);
            return;
        }
        ssize_t n1 = read(fd1,buffer1,buffer_size);
        ssize_t n2 = read(fd2,buffer2,buffer_size);
        if(n1<0 or n2<0 or n1!=n2){
            perror("error in reading files");
            free(buffer1);
            free(buffer2);
            close(fd1);
            close(fd2);
            return;
        }
        if(n1==0 and n2==0)break;
        int flag=1;
        for(int i=0;i<n1;i++){
            if(buffer1[i]!=buffer2[n1-i-1]){
                flag=0;
            }
        }
        if(!flag){
            // write(1,"input and output files are not ok for given flag and constraints\n",66);
            free(buffer1);
            free(buffer2);
            close(fd1);
            close(fd2);
            return;
        }
        rem-=buffer_size;
    }
    // write(1,"input and output files are ok for given flag and constraints\n",62);
    flag = true;
    free(buffer1);
    free(buffer2);
    close(fd1);
    close(fd2);
}

void flag2(char* output, char* input, char* start_index, char* end_index, bool &flag){
    int fd1 = open(output,O_RDONLY);
    int fd2 = open(input,O_RDONLY);
    if(fd1==-1){
        perror("open sys call failed for output file");
        return;
    }
    if(fd2==-1){
        perror("open sys call failed for input file");
        return;
    }
    struct stat st;
    if(stat(input,&st)==-1){
        perror("stat sys call error for input file");
        close(fd1);
        close(fd2);
        return;
    }
    long long tot = st.st_size;
    char *endptr;
    long long int start_ind = strtoll(start_index,&endptr,10);
    if(start_ind < 0 or *endptr!='\0'){
        write(1,"start index should be number >=0\n",34);
        close(fd1);
        close(fd2);
        return;
    }
    char *endptr1;
    long long int end_ind = strtoll(end_index,&endptr1,10);
    if(end_ind < 0 or end_ind>=tot or *endptr1!='\0'){
        write(1,"end index should be number >=0 and <tot_size\n",46);
        close(fd1);
        close(fd2);
        return;
    }
    if(start_ind>end_ind){
        write(1,"start index shouldnt be greater than end index\n",48);
        close(fd1);
        close(fd2);
        return;
    }
    long long block_size = min(65536ll,(long long int)st.st_size);
    char* buffer1 = (char*)malloc(block_size);
    char* buffer2 = (char*)malloc(block_size);
    if(!buffer1 or !buffer2){
        perror("memory allocation failed");
        close(fd1);
        close(fd2);
        return;
    }
    off_t rem = start_ind;
    while(rem>0){
        ssize_t buffer_size = min(65536ll, (long long)rem);
        off_t offset = rem - buffer_size;
        if(lseek(fd1,offset,SEEK_SET)==-1){
            perror("lseek sys call failed");
            free(buffer1);
            free(buffer2);
            close(fd1);
            close(fd2);
            return;
        }
        ssize_t n1 = read(fd2,buffer2,buffer_size);
        ssize_t n2 = read(fd1,buffer1,buffer_size);
        if(n1<0 or n2<0 or n1!=n2){
            perror("error in reading files");
            free(buffer1);
            free(buffer2);
            close(fd1);
            close(fd2);
            return;
        }
        if(n1==0 and n2==0)break;
        int flag=1;
        for(int i=0;i<n1;i++){
            if(buffer1[i]!=buffer2[n1-i-1]){
                flag=0;
            }
        }
        if(!flag){
            // write(1,"input and output files are not ok for given flag and constraints\n",66);
            free(buffer1);
            free(buffer2);
            close(fd1);
            close(fd2);
            return;
        }
        rem-=buffer_size;
    }
    rem = end_ind-start_ind+1;
    off_t offset = start_ind;
    while(rem>0){
        ssize_t buffer_size = min(65536ll, (long long)rem);
        if(lseek(fd1,offset,SEEK_SET)==-1){
            perror("lseek sys call failed");
            free(buffer1);
            free(buffer2);
            close(fd1);
            close(fd2);
            return;
        }
        ssize_t n1 = read(fd2,buffer2,buffer_size);
        ssize_t n2 = read(fd1,buffer1,buffer_size);
        if(n1<0 or n2<0 or n1!=n2){
            perror("error in reading files");
            free(buffer1);
            free(buffer2);
            close(fd1);
            close(fd2);
            return;
        }
        if(n1==0 and n2==0)break;
        int flag=1;
        for(int i=0;i<n1;i++){
            if(buffer1[i]!=buffer2[i]){
                flag=0;
            }
        }
        if(!flag){
            // write(1,"input and output files are not ok for given flag and constraints\n",66);
            free(buffer1);
            free(buffer2);
            close(fd1);
            close(fd2);
            return;
        }
        rem-=buffer_size;
        offset+=buffer_size;
    }
    rem = st.st_size-end_ind-1;
    offset = st.st_size;
    while(rem>0){
        ssize_t buffer_size = min(65536ll, (long long)rem);
        offset-=buffer_size;
        if(lseek(fd1,offset,SEEK_SET)==-1){
            perror("lseek sys call failed");
            free(buffer1);
            free(buffer1);
            close(fd1);
            close(fd2);
            return;
        }
        ssize_t n1 = read(fd2,buffer2,buffer_size);
        ssize_t n2 = read(fd1,buffer1,buffer_size);
        if(n1<0 or n2<0 or n1!=n2){
            perror("error in reading files");
            free(buffer1);
            free(buffer2);
            close(fd1);
            close(fd2);
            return;
        }
        if(n1==0 and n2==0)break;
        int flag=1;
        for(int i=0;i<n1;i++){
            if(buffer1[i]!=buffer2[n1-i-1]){
                flag=0;
            }
        }
        if(!flag){
            // write(1,"input and output files are not ok for given flag and constraints\n",66);
            free(buffer1);
            free(buffer2);
            close(fd1);
            close(fd2);
            return;
        }
        rem-=buffer_size;
    }
    // write(1,"input and output files are ok for given flag and constraints\n",62);
    free(buffer1);
    free(buffer2);
    close(fd1);
    close(fd2);
    flag = true;
    return;
}

int main(int argc, char* argv[]){
    if(argc<=4 or argc>=8){
        write(1,"No Flag Found!\n", 16);
        return 0;
    }
    if(!checkFile(argv[1]) or !checkFile(argv[2])){
        write(1,"old and new files should be .txt files!\n", 41);
        return 0;
    }
    if(!checkDirectory(argv[3])){
        write(1,"directory is not present!\n", 27);
        return 0;
    }
    bool flag_size = true;
    if(!checkFileSizes(argv[1],argv[2])){
        flag_size = false;
    }
    bool flag = false; 
    if(argc==5){
        if(strcmp(argv[4],"1")==0 and flag_size){
            flag1(argv[1],argv[2],flag);
        }
    }
    if(argc==6){
        if(strcmp(argv[4],"0")==0 and flag_size){
            flag0(argv[1],argv[2],argv[5],flag);
        }
    }
    if(argc==7){
        if(strcmp(argv[4],"2")==0 and flag_size){
            flag2(argv[1],argv[2],argv[5],argv[6],flag);
        }
    }
    if(!checkDirectory(argv[3])){
        write(1,"directory is not present!\n", 27);
        return 0;
    }
    else{
        write(1,"Directory is created: Yes\n", 27);
    }
    if(flag){
        write(1,"Whether file contents are correctly processed: Yes\n",52);
    }
    else{
        write(1,"Whether file contents are correctly processed: No\n",51);
    }
    if(checkFileSizes(argv[1],argv[2])){
        write(1,"Both Files Sizes are Same: Yes\n", 32);
    }
    else{
        write(1,"Both Files Sizes are Same: No\n", 31);
    }
    struct stat st1;
    struct stat st2;
    struct stat st3;
    if(stat(argv[1],&st1)==-1){
        perror("stat sys call error for output file");
    }
    if(stat(argv[2],&st2)==-1){
        perror("stat sys call error for input file");
    }
    if(stat(argv[3],&st3)==-1){
        perror("stat sys call error for directory");
    }
    mode_t m1 = st1.st_mode, m2 = st2.st_mode, m3 = st3.st_mode;
    printPermissions(&st1,"newfile");
    printPermissions(&st2,"oldfile");
    printPermissions(&st3,"directory");
    return 0;
}