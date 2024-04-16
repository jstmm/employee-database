#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#include "../include/file.h"
#include "../include/common.h"

int create_db_file(char* filepath) {
    int fd = open(filepath, O_RDONLY);
    if (fd != -1) {
        close(fd);
        printf("File already exists\n");
        return STATUS_ERROR;
    }

    fd = open(filepath, O_RDWR | O_CREAT, 0644);
    if (fd == -1) {
        perror("open");
        return STATUS_ERROR;
    }

    return fd;
}

int open_db_file(char* filepath) {
    int fd = open(filepath, O_RDWR, 0644);
    if (fd == -1) {
        perror("open");
        return STATUS_ERROR;
    }

    return fd;
}
