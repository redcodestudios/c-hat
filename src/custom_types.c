#include "inc/custom_types.h"

/* Add a string in the given index at a given string array */
void add_str(int index, char* str, str_array *a){
    int length = a->length + 1;
    a->elements = realloc(a->elements, sizeof(char**) * length);
    a->elements[index] = str;
    a->length = length;
}

/* Initialize a str_array */
str_array new_str_array(){
    str_array sta = {NULL, 0};
    return sta;
}