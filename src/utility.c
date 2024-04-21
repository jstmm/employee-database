#include <ctype.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char* toLowerCase(const char* str)
{
    size_t len = strlen(str);
    char* lower = (char*)malloc(len + 1);

    if (lower == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(1);
    }

    for (size_t i = 0; i < len; i++) {
        lower[i] = tolower(str[i]);
    }
    lower[len] = '\0';
    return lower;
}

bool parse_boolean(const char* str)
{
    bool result = false;
    char* lowerStr = toLowerCase(str);

    if (strcmp(lowerStr, "true") == 0) {
        result = true;
    } else if (strcmp(lowerStr, "false") == 0) {
        result = false;
    } else {
        printf("Invalid input\n");
    }
    free(lowerStr);
    return result;
}
