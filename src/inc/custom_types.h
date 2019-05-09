#ifndef CUSTOM_TYPES_H
#define CUSTOM_TYPES_H

#include <stdlib.h>

typedef struct{
    char** elements;
    int length;
}str_array;

typedef struct{
    char* value;
    int length;
}username_t;

void add_str(int index, char* str, str_array* a);
str_array new_str_array();

#endif