#include <string.h>
#include<stdio.h>

struct Student {
    int id;
    float marks;
};

int main(){
    struct Student s1 = {1, 98.5};
    struct Student s2;

    memcpy(&s2, &s1, sizeof(struct Student)); 
    printf("%d %d",s1.id,s2.id);
    // printf("")
    s2.id=3;
    printf("%d %d",s1.id,s2.id);

    // s2 is now a copy of s1
}


