#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

// Function to convert a string to lowercase
void str_to_lower(char* str)
{
    while (*str) {
        *str = tolower(*str);
        str++;
    }
}

// Function to parse "true" or "false" string to boolean
bool parse_boolean(const char* str)
{
    char lowerStr[10]; // Assuming maximum length of the string is 9 characters
    strcpy(lowerStr, str);
    str_to_lower(lowerStr);

    if (strcmp(lowerStr, "true") == 0) {
        return true;
    } else if (strcmp(lowerStr, "false") == 0) {
        return false;
    } else {
        // Handle invalid input
        printf("Invalid input\n");
        return false; // Or handle error in your desired way
    }
}
