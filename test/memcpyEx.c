#include<stdio.h>
#include<string.h>
int main(){
    int source_arr[]={10,20,30,40,50};
    int dest_arr[5];

    size_t bytes_to_copy=sizeof(source_arr);

    memcpy(dest_arr,source_arr,bytes_to_copy);

    for(int i=0;i<5;i++){
        printf("%d\t",source_arr[i]);
    }
}