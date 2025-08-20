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

void printProcessing(int percent){
    char c[16]="Processing: ";
    if((percent/100)%10)c[12]=(char)('0'+(percent/100)%10);
    else c[12]=' ';
    c[13]=(char)('0'+(percent/10)%10);
    c[14]=(char)('0'+(percent/1)%10);
    c[15]='%';
    write(STDOUT_FILENO, "\r", 1);
    write(STDOUT_FILENO, c, 16);
}

void createDirectory(){
    const char* Directory = "Assignment1";
    if(mkdir(Directory, 0700)==-1){
        if(errno==EEXIST){
            write(1,"Directory Already Exists\n",25);
        }
        else {
            perror("mkdir failed");
            return;
        }
    }
    else{
        write(1,"Directory Created Successfully\n",31);
    }
}

void changeMode(char* file){
    if (chmod(file, 0644) == -1) {
        perror("chmod failed");
    }
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

void flag0(char* file, char* block){
    createDirectory();
    char s[250]="Assignment1/0_"; 
    strcat(s,file);
    int fd2 = open(file,O_RDONLY);
    if(fd2==-1){
        perror("open sys call error for input file");
        return;
    }
    int fd1 = open(s,O_CREAT|O_WRONLY|O_TRUNC,0600);
    if(fd1==-1){
        perror("open sys call error for new file");
        close(fd2);
        return;
    }
    struct stat st;
    if(stat(file,&st)==0){
        mode_t mode = st.st_mode & 0777;
        if(mode != 0644){
            perror("Input file does not have 0644 permissions");
            close(fd1);
            close(fd2);
            return;
        }
    }
    else{
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
    // reading 
    char* buffer = (char*)malloc(block_size);
    if(!buffer){
        perror("memory allocation failed");
        close(fd1);
        close(fd2);
        return;
    }
    ssize_t n;
    long long done = 0, tot = st.st_size;
    while(1){
        n = read(fd2,buffer,block_size);
        if(n<0){
            perror("error in input file");
            free(buffer);
            close(fd1);
            close(fd2);
            return;
        }
        if(n==0)break;
        long long size = min((long long)n,block_size);
        for(int i=0;i<n/2;i++){
            swap(buffer[i],buffer[n-i-1]);
        }
        if(write(fd1,buffer,n)!=n){
            perror("error in output file");
            free(buffer);
            close(fd1);
            close(fd2);
            return;
        }
        done+=n;
        int percent = (done*100)/tot;
        printProcessing(percent);
    }
    printProcessing(100);
    write(STDOUT_FILENO, "\n", 1);
    free(buffer);
    close(fd1);
    close(fd2);
}

void flag1(char* file){
    createDirectory();
    char s[250]="Assignment1/1_"; 
    strcat(s,file);
    int fd2 = open(file,O_RDONLY);
    if(fd2==-1){
        perror("open sys call error for input file");
        return;
    }
    int fd1 = open(s,O_CREAT|O_WRONLY|O_TRUNC,0600);
    if(fd1==-1){
        perror("open sys call error for new file");
        close(fd2);
        return;
    }
    struct stat st;
    if(stat(file,&st)==0){
        mode_t mode = st.st_mode & 0777;
        if(mode != 0644){
            perror("Input file does not have 0644 permissions");
            close(fd1);
            close(fd2);
            return;
        }
    }
    else{
        perror("stat sys call error for input file");
        close(fd1);
        close(fd2);
        return;
    }
    long long tot = st.st_size;
    long long done = 0;
    char* buffer = (char*)malloc(min(65536ll,tot));
    if(!buffer){
        perror("memory allocation failed");
        close(fd1);
        close(fd2);
        return;
    }
    off_t  rem = st.st_size;
    while(rem>0){
        ssize_t buffer_size = min((long long)65536,(long long)rem); 
        off_t offset = rem - buffer_size;
        if(lseek(fd2,offset,SEEK_SET)==-1){
            perror("lseek sys call failed");
            free(buffer);
            close(fd1);
            close(fd2);
            return;
        }
        ssize_t n = read(fd2,buffer,buffer_size);
        if(n<0){
            perror("error in input file");
            free(buffer);
            close(fd1);
            close(fd2);
            return;
        }
        long long size = min((long long)n,(long long)buffer_size);
        for(int i=0;i<n/2;i++){
            swap(buffer[i],buffer[n-i-1]);
        }
        if(write(fd1,buffer,n)!=n){
            perror("error in output file");
            free(buffer);
            close(fd1);
            close(fd2);
            return;
        }
        done+=n;
        int percent = (done*100)/tot;
        printProcessing(percent);
        rem-=buffer_size;
    }
    printProcessing(100);
    write(STDOUT_FILENO, "\n", 1);
    free(buffer);
    close(fd1);
    close(fd2);
}

void flag2(char* file, char* start_index, char* end_index){
    createDirectory();
    char s[250]="Assignment1/2_"; 
    strcat(s,file);
    int fd2 = open(file,O_RDONLY);
    if(fd2==-1){
        perror("open sys call error for input file");
        return;
    }
    int fd1 = open(s,O_CREAT|O_WRONLY|O_TRUNC,0600);
    if(fd1==-1){
        perror("open sys call error for new file");
        close(fd2);
        return;
    }
    struct stat st;
    if(stat(file,&st)==0){
        mode_t mode = st.st_mode & 0777;
        if(mode != 0644){
            perror("Input file does not have 0644 permissions");
            close(fd1);
            close(fd2);
            return;
        }
    }
    else{
        perror("stat sys call error for input file");
        close(fd1);
        close(fd2);
        return;
    }
    long long tot = st.st_size;
    long long done = 0;
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
    char* buffer = (char*)malloc(min(65536ll,tot));
    if(!buffer){
        perror("memory allocation failed");
        close(fd1);
        close(fd2);
        return;
    }
    off_t rem = start_ind;
    while(rem>0){
        ssize_t buffer_size = min(65536ll, (long long)rem);
        off_t offset = rem - buffer_size;
        if(lseek(fd2,offset,SEEK_SET)==-1){
            perror("lseek sys call failed");
            free(buffer);
            close(fd1);
            close(fd2);
            return;
        }
        ssize_t n = read(fd2,buffer,buffer_size);
        if(n<0){
            perror("error in input file");
            free(buffer);
            close(fd1);
            close(fd2);
            return;
        }
        long long size = min((long long)n,(long long)buffer_size);
        for(int i=0;i<n/2;i++){
            swap(buffer[i],buffer[n-i-1]);
        }
        if(write(fd1,buffer,n)!=n){
            perror("error in output file");
            free(buffer);
            close(fd1);
            close(fd2);
            return;
        }
        done+=n;
        int percent = (done*100)/tot;
        printProcessing(percent);
        rem-=buffer_size;
    }
    rem = end_ind-start_ind+1;
    off_t offset = start_ind;
    while(rem>0){
        ssize_t buffer_size = min(65536ll, (long long)rem);
        if(lseek(fd2,offset,SEEK_SET)==-1){
            perror("lseek sys call failed");
            free(buffer);
            close(fd1);
            close(fd2);
            return;
        }
        ssize_t n = read(fd2,buffer,buffer_size);
        if(n<0){
            perror("error in input file");
            free(buffer);
            close(fd1);
            close(fd2);
            return;
        }
        if(write(fd1,buffer,n)!=n){
            perror("error in output file");
            free(buffer);
            close(fd1);
            close(fd2);
            return;
        }
        done+=n;
        int percent = (done*100)/tot;
        printProcessing(percent);
        rem-=buffer_size;
        offset+=buffer_size;
    }
    rem = st.st_size-end_ind-1;
    offset = st.st_size;
    while(rem>0){
        ssize_t buffer_size = min(65536ll, (long long)rem);
        offset-=buffer_size;
        if(lseek(fd2,offset,SEEK_SET)==-1){
            perror("lseek sys call failed");
            free(buffer);
            close(fd1);
            close(fd2);
            return;
        }
        ssize_t n = read(fd2,buffer,buffer_size);
        if(n<0){
            perror("error in input file");
            free(buffer);
            close(fd1);
            close(fd2);
            return;
        }
        long long size = min((long long)n,(long long)buffer_size);
        for(int i=0;i<n/2;i++){
            swap(buffer[i],buffer[n-i-1]);
        }
        if(write(fd1,buffer,n)!=n){
            perror("error in output file");
            free(buffer);
            close(fd1);
            close(fd2);
            return;
        }
        done+=n;
        int percent = (done*100)/tot;
        printProcessing(percent);
        rem-=buffer_size;
    }
    printProcessing(100);
    write(STDOUT_FILENO, "\n", 1);
    free(buffer);
    close(fd1);
    close(fd2);
}

int main(int argc, char *argv[]){
    if(argc<=2 or argc>=6){
        write(1,"No Flag Found!\n", 15);
        return 0;
    }
    if(!checkFile(argv[1])){
        write(1,".txt file input needed!\n", 25);
        return 0;
    }
    changeMode(argv[1]);
    if(argc==3){
        if(strcmp(argv[2],"1")==0){
            flag1(argv[1]);
        }
        else{
            write(1,"No Flag Found!\n", 15);
        }
    }
    if(argc==4){
        if(strcmp(argv[2],"0")==0){
            flag0(argv[1],argv[3]);
        }
        else{
            write(1,"No Flag Found!\n", 15);
        }
    }
    if(argc==5){
        if(strcmp(argv[2],"2")==0){
            flag2(argv[1],argv[3],argv[4]);
        }
        else{
            write(1,"No Flag Found!\n", 15);
        }
    }
    return 0;
}