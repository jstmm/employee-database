#include <arpa/inet.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <unistd.h>

#include "parse.h"
#include "utility.h"

int create_db_header(int fd, struct dbheader_t** header_out)
{
    struct dbheader_t* hdr = calloc(1, sizeof(struct dbheader_t));

    if (hdr == NULL) {
        printf("Malloc failed to create db header\n");
        return STATUS_ERROR;
    }

    hdr->magic = HEADER_MAGIC;
    hdr->version = 0x1;
    hdr->employee_count = 0;
    hdr->file_size = sizeof(struct dbheader_t);
    *header_out = hdr;

    return STATUS_SUCCESS;
}

int validate_db_header(int fd, struct dbheader_t** header_out)
{
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

    struct stat dbstat = { 0 };
    fstat(fd, &dbstat);
    if (hdr->file_size != dbstat.st_size) {
        printf("Corrupted database\n");
        free(hdr);
        return -1;
    }

    *header_out = hdr;

    return STATUS_SUCCESS;
}

int read_employees(int fd, const struct dbheader_t* header, struct employee_t** employee_out)
{
    if (fd < 0) {
        printf("Got a bad FD from the user\n");
        return STATUS_ERROR;
    }

    int count = header->employee_count;
    struct employee_t* employees = calloc(count, sizeof(struct employee_t));

    if (employees == NULL) {
        printf("Malloc failed\n");
        return STATUS_ERROR;
    }

    read(fd, employees, count * sizeof(struct employee_t));

    for (int i = 0; i < count; i++) {
        employees[i].hours = ntohl(employees[i].hours);
    }

    *employee_out = employees;

    return STATUS_SUCCESS;
}

int add_employee(const struct dbheader_t* header, struct employee_t* employees, char* addstring)
{
    char* first_name = strtok(addstring, ",");
    char* last_name = strtok(NULL, ",");
    char* address = strtok(NULL, ",");
    char* hours = strtok(NULL, ",");
    char* is_manager = strtok(NULL, ",");

    strncpy(employees[header->employee_count - 1].first_name, first_name, sizeof(employees[header->employee_count - 1].first_name));
    strncpy(employees[header->employee_count - 1].last_name, last_name, sizeof(employees[header->employee_count - 1].last_name));
    strncpy(employees[header->employee_count - 1].address, address, sizeof(employees[header->employee_count - 1].address));
    employees[header->employee_count - 1].hours = atoi(hours);
    employees[header->employee_count - 1].is_manager = parse_boolean(is_manager);

    printf("User addded : %s %s %s %s %s\n", first_name, last_name, address, hours, is_manager);

    return STATUS_SUCCESS;
}

int remove_employee(struct dbheader_t* header, struct employee_t* employees, const char* employee_index)
{
    const int employee_i = atoi(employee_index);

    if (header->employee_count == 0) {
        printf("Database empty - nothing to delete\n");
        return STATUS_ERROR;
    }

    if (employee_i < 0 || employee_i >= header->employee_count) {
        printf("Out of bound reference\n");
        return STATUS_ERROR;
    }

    header->employee_count--;
    memmove(employees + employee_i, employees + employee_i + 1, (header->employee_count - employee_i) * sizeof(struct employee_t));
    employees = realloc(employees, header->employee_count * (sizeof(struct employee_t)));

    return STATUS_SUCCESS;
}

int search_employee(struct dbheader_t* header, struct employee_t* employees, const char* search)
{
    bool record_found = false;

    for (int i = 0; i < header->employee_count; i++) {
        char* fn = toLowerCase(employees[i].first_name);
        char* ln = toLowerCase(employees[i].last_name);
        char* search_lc = toLowerCase(search);
        if (strcmp(fn, search_lc) == 0 || strcmp(ln, search_lc) == 0) {
            printf("Employee %d\n", i);
            printf("\tName: %s %s\n", employees[i].first_name, employees[i].last_name);
            printf("\tAddress: %s\n", employees[i].address);
            printf("\tHours: %d\n", employees[i].hours);
            printf("\tIs Manager: %d\n", employees[i].is_manager);
            record_found = true;
        }
        free(fn);
        free(ln);
        free(search_lc);
    }

    if (!record_found) {
        printf("No record found for \"%s\"\n", search);
    }
    return STATUS_SUCCESS;
}

void list_employees(const struct dbheader_t* header, struct employee_t* employees)
{
    for (int i = 0; i < header->employee_count; i++) {
        printf("Employee %d\n", i);
        printf("\tName: %s %s\n", employees[i].first_name, employees[i].last_name);
        printf("\tAddress: %s\n", employees[i].address);
        printf("\tHours: %d\n", employees[i].hours);
        printf("\tIs Manager: %d\n", employees[i].is_manager);
    }
}

void save_to_file(int fd, struct dbheader_t* header, struct employee_t* employees)
{
    if (fd < 0) {
        printf("Got a bad FD from the user\n");
    }

    int realcount = header->employee_count;
    int realsize = sizeof(struct dbheader_t) + (sizeof(struct employee_t) * realcount);

    header->magic = htonl(header->magic);
    header->version = htons(header->version);
    header->employee_count = htons(header->employee_count);
    header->file_size = htonl(realsize);

    for (int i = 0; i < realcount; i++) {
        employees[i].hours = htonl(employees[i].hours);
    }

    lseek(fd, 0, SEEK_SET);
    write(fd, header, sizeof(struct dbheader_t));

    for (int i = 0; i < realcount; i++) {
        write(fd, &employees[i], sizeof(struct employee_t));
    }

    ftruncate(fd, realsize);
}
