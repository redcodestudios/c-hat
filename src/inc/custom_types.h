#include <stdlib.h>

typedef struct{
    char** elements;
    int length;
}str_array;

void add_str(int index, char* str, str_array* a);
str_array new_str_array();