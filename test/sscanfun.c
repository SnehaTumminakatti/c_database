#include<stdio.h>
int main(){
 
    const char *data="John 28 75.45";
    char name[50];
    int age;
    float weight;

    int items_read=sscanf(data,"%49s %d %f",name,&age,&weight);
    printf("Items Read %d %49s %d %f",items_read,name,&age,&weight);
}
// sscanf converts the string separated by space andstores it into specific variables