#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctype.h>

#include "../include/parse.h"
#include "../include/common.h"

// Function to convert a string to lowercase
void str_to_lower(char *str) {
    while (*str) {
        *str = tolower(*str);
        str++;
    }
}

// Function to parse "true" or "false" string to boolean
bool parse_boolean(const char *str) {
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

int create_db_header(int fd, struct dbheader_t** headerOut) {
    struct dbheader_t* hdr = calloc(1, sizeof(struct dbheader_t));
    if (hdr == NULL) {
        printf("Malloc failed to create db header\n");
        return STATUS_ERROR;
    }
    hdr->magic = HEADER_MAGIC;
    hdr->version = 0x1;
    hdr->employee_count = 0;
    hdr->file_size = sizeof(struct dbheader_t);

    *headerOut = hdr;

    return STATUS_SUCCESS;
}

int validate_db_header(int fd, struct dbheader_t** headerOut) {
    if (fd < 0) {
        printf("Got a bad FD from the user\n");
        return STATUS_ERROR;
    }

    struct dbheader_t* hdr = calloc(1, sizeof(struct dbheader_t));
    if (hdr == NULL) {
        printf("Malloc failed create a db header\n");
        return STATUS_ERROR;
    }
    
    if (read(fd, hdr, sizeof(struct dbheader_t)) != sizeof(struct dbheader_t)) {
        perror("read");
        free(hdr);
        return STATUS_ERROR;
    }

    hdr->magic = ntohl(hdr->magic);
    hdr->version = ntohs(hdr->version);
    hdr->employee_count = ntohs(hdr->employee_count);
    hdr->file_size = ntohl(hdr->file_size);

    if (hdr->magic != HEADER_MAGIC) {
        printf("Improper header magic\n");
        free(hdr);
        return -1;
    }
    
    if (hdr->version != 1) {
        printf("Improper header vesion\n");
        free(hdr);
        return -1;
    }

    struct stat dbstat = {0};
    fstat(fd, &dbstat);
    if (hdr->file_size != dbstat.st_size) {
        printf("Corrupted database\n");
        free(hdr);
        return -1;
    }

    *headerOut = hdr;

    return STATUS_SUCCESS;
}

int read_employees(int fd, struct dbheader_t* hdr, struct employee_t** employeeOut) {
    if (fd < 0) {
        printf("Got a bad FD from the user\n");
        return STATUS_ERROR;
    }

    int count = hdr->employee_count;

    struct employee_t* empl = calloc(count, sizeof(struct employee_t));
    if (empl == NULL) {
        printf("Malloc failed\n");
        return STATUS_ERROR;
    }

    read(fd, empl, count*sizeof(struct employee_t));

    for (int i = 0; i < count; i++) {
        empl[i].hours = ntohl(empl[i].hours);
    }

    *employeeOut = empl;

    return STATUS_SUCCESS;
}

int add_employee(struct dbheader_t* hdr, struct employee_t* empl, char* addstring) {
    printf("%s\n", addstring);

    char* first_name = strtok(addstring, ",");
    char* last_name = strtok(NULL, ",");
    char* address = strtok(NULL, ",");
    char* hours = strtok(NULL, ",");
    char* is_manager = strtok(NULL, ",");
    printf("%s %s %s %s %s\n", first_name, last_name, address, hours, is_manager);

    strncpy(empl[hdr->employee_count - 1].first_name, first_name, sizeof(empl[hdr->employee_count - 1].first_name));
    strncpy(empl[hdr->employee_count - 1].last_name, last_name, sizeof(empl[hdr->employee_count - 1].last_name));
    strncpy(empl[hdr->employee_count - 1].address, address, sizeof(empl[hdr->employee_count - 1].address));
    empl[hdr->employee_count - 1].hours = atoi(hours);
    empl[hdr->employee_count - 1].is_manager = parse_boolean(is_manager);

    return STATUS_SUCCESS;
}

void output_file(int fd, struct dbheader_t* hdr, struct employee_t* empl) {
    if (fd < 0) {
        printf("Got a bad FD from the user\n");
    }

    int realcount = hdr->employee_count;

    hdr->magic = htonl(hdr->magic);
    hdr->version = htons(hdr->version);
    hdr->employee_count = htons(hdr->employee_count);
    hdr->file_size = htonl(sizeof(struct dbheader_t) + (sizeof(struct employee_t) * realcount));

    lseek(fd, 0, SEEK_SET);
    
    write(fd, hdr, sizeof(struct dbheader_t));

    for (int i = 0; i < realcount; i++) {
        empl[i].hours = htonl(empl[i].hours);
        write(fd, &empl[i], sizeof(struct employee_t));
    }
}

void list_employees(struct dbheader_t* hdr, struct employee_t* empl) {
    for (int i = 0; i < hdr->employee_count; i++) {
        printf("Employee %d\n", i);
        printf("\tName: %s %s\n", empl[i].first_name, empl[i].last_name);
        printf("\tAddress: %s\n", empl[i].address);
        printf("\tHours: %d\n", empl[i].hours);
        printf("\tIs Manager: %d\n", empl[i].is_manager);
    }
}

int remove_employee(struct dbheader_t* hdr, struct employee_t* empl, char rm_employee) {
    printf("%c\n", rm_employee);
    return STATUS_SUCCESS;
}
