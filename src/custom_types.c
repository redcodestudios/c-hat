#include "inc/custom_types.h"

/* Add a string in the given index at a given string array */
void add_str(int index, char* str, str_array *a){
    int new_len = a->length + 1;
    a->elements = realloc(a->elements, sizeof(char**) * new_len);
    a->elements[index] = str;
    a->length = new_len;
}

/* Initialize a str_array */
str_array new_str_array(){
    str_array sta = {NULL, 0};
    return sta;
}