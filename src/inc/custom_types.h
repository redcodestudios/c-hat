#ifndef CUSTOM_TYPES_H
#define CUSTOM_TYPES_H

#include <stdlib.h>

typedef struct{
    char** elements;
    int length;
}str_array_t;


void add_str(int index, char* str, str_array_t* a);
str_array_t new_str_array();

#endif