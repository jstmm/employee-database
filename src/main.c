#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "file.h"
#include "parse.h"
#include "utility.h"

void print_usage(char* argv[])
{
    printf("Usage: %s -n -f <database file>\n", argv[0]);
    printf("\t -n  -  create new database file\n");
    printf("\t -f  -  (required) path to database file\n");
    printf("\t -l  -  list all employees\n");
    printf("\t -r  -  remove employee\n");
    printf("\t -s  -  search employee\n");
    return;
}

int main(int argc, char* argv[])
{
    // Create a new database (-f <file name>)
    bool newfile = false;
    char* filepath = NULL;

    // Add a new employee (-a <string>)
    // Input format: <first name,last name,address,number of hours,is manager>
    char* addstring = NULL;

    // Remove an employee (-r <index from list>)
    char* rm_employee = NULL;

    // Search for an employee by name (-s <employee first or last name>)
    char* sr_employee = NULL;

    // List all employees (-l)
    bool list = false;

    int fd = -1;
    struct dbheader_t* hdr = NULL;
    struct employee_t* empl = NULL;

    int c = 0;
    while ((c = getopt(argc, argv, "nf:a:lr:s:")) != -1) {
        switch (c) {
        case 'n':
            newfile = true;
            break;
        case 'f':
            filepath = optarg;
            break;
        case 'a':
            addstring = optarg;
            break;
        case 'l':
            list = true;
            break;
        case 'r':
            rm_employee = optarg;
            break;
        case 's':
            sr_employee = optarg;
            break;
        case '?':
            printf("Unknown option -%c\n", c);
            break;
        default:
            return -1;
        }
    }

    if (filepath == NULL) {
        printf("Filepath is a required argument\n");
        print_usage(argv);
        return 0;
    }

    if (newfile) {
        fd = create_db_file(filepath);
        if (fd == STATUS_ERROR) {
            printf("Unable to create database file\n");
            return -1;
        }
        if (create_db_header(fd, &hdr) == STATUS_ERROR) {
            printf("Failed to create database header\n");
            return -1;
        }
    } else {
        fd = open_db_file(filepath);
        if (fd == STATUS_ERROR) {
            printf("Unable to open database file\n");
            return -1;
        }
        if (validate_db_header(fd, &hdr) == STATUS_ERROR) {
            printf("Failed to validate database header\n");
            return -1;
        }
    }

    if (read_employees(fd, hdr, &empl) != STATUS_SUCCESS) {
        printf("Failed to read employees");
        return -1;
    }

    if (addstring) {
        hdr->employee_count++;
        empl = realloc(empl, hdr->employee_count * (sizeof(struct employee_t)));
        add_employee(hdr, empl, addstring);
    }

    if (rm_employee) {
        remove_employee(hdr, empl, rm_employee);
    }

    if (sr_employee) {
        search_employee(hdr, empl, sr_employee);
    }

    if (list) {
        list_employees(hdr, empl);
    }

    save_to_file(fd, hdr, empl);
    close(fd);

    return 0;
}
