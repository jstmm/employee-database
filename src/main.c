#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>

#include "../include/file.h"
#include "../include/parse.h"
#include "../include/common.h"

void print_usage(char* argv[]) {
    printf("Usage: %s -n -f <database file>\n", argv[0]);
    printf("\t -n  -  create new database file\n");
    printf("\t -f  -  (required) path to database file\n");
    return;
}

int main(int argc, char* argv[]) {
    char *filepath = NULL;
    char *addstring = NULL;
    bool newfile = false;
    int c = 0;

    int fd = -1;
    struct dbheader_t* hdr = NULL;
    struct employee_t* empl = NULL;

    while ((c = getopt(argc, argv, "nf:a:")) != -1) {
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
        empl = realloc(empl, hdr->employee_count*(sizeof(struct employee_t)));
        add_employee(hdr, empl, addstring);
    }

    output_file(fd, hdr, empl);

    close(fd);
    return 0;
}
