#include<stdio.h>
#include<string.h>
int main(){
    char str[]="Learn,C,Programming";
    char* token=strtok(str,",");
    while(token!=NULL){
        printf("Token: %s\n",token);
        token=strtok(NULL,",");
    }
    return 0;
}