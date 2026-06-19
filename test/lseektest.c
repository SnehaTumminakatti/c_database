#include<stdio.h>
#include <fcntl.h>
#include <unistd.h>
int main(){
    int fd = open("example.txt",O_RDONLY);
    if(fd==-1){
        return 1;
    }

    char buffer[10];
    lseek(fd,7,SEEK_SET);
    read(fd,buffer,4);
    buffer[4]='\0';
    printf("Skipped 'Hello ,' :%s\n",buffer);
    lseek(fd,2,SEEK_CUR);
    read(fd,buffer,3);
    buffer[2]='\0';
    printf("Advanced further :%s \n",buffer);
    close(fd);
    return 0;
}