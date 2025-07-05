#include<stdio.h>
#include<stddef.h>

struct User{
    int id;
    char name[32];
    char email[255];
};

#define SIZE_OF_MEMBER(struct_type,member) ((size_t)&(((struct_type *)0x1)->member) -(size_t)(struct_type *)0x1)
int main(){
    printf("Size of is: %zu\n",SIZE_OF_MEMBER(struct User,id));
    printf("Size of is: %zu\n",SIZE_OF_MEMBER(struct User,name));
    printf("Size of is: %zu\n",SIZE_OF_MEMBER(struct User,email));
    return 0;
}