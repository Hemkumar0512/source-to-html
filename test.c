/* this is multi-line comment */

#include <stdio.h>
#include "my_header.h"

int main(void)
{
    int integer = 10;
    int returnValue = 20;
    int static_count = 30;

    int decimal = 123;
    int hexadecimal = 0x2A;
    float fraction = 3.14;
    float exponent = 1.5e3;
    char letter = 'A';
    char newline = '\n';

    char *message = "Hello World";
    char *quote = "He said \"Hello\"";
    char *path = "C:\\Temp\\file.txt";

// This is a single-line comment
/* This is a
   multi-line comment */

    int count=0;
    count++;
    if (count==1)
    {
        count = count->value;
    }

    return 0;
}
