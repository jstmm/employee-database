#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "file.h"

int open_file_rw(char* filename) {
    int file_descriptor = open(filename, O_RDWR | O_CREAT, 0644);

    if (file_descriptor == -1) {
        perror("open");
    }

    return file_descriptor;
}
