#ifndef PARSE_H
#define PARSE_H

typedef struct {
    u_int32_t employees;
    u_int32_t version;
    u_int32_t file_size;
} DatabaseHeader;

int parse_file_header(int fd, int* num_employees_out);

#endif
