#include <stdio.h>

struct Student {
    int id;
    float marks;
    char name[50];
};

#define size_of_attributes(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

int main() {
    
    printf("Size of id: %zu\n", size_of_attributes(struct Student, id));       // 4
    printf("Size of marks: %zu\n", size_of_attributes(struct Student, marks)); // 4
    printf("Size of name: %zu\n", size_of_attributes(struct Student, name));   // 50
    struct Student* s = NULL;
    printf("Size of name: %s\n", s->name);   // 50

    return 0;
}
