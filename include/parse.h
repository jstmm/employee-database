#ifndef PARSE_H
#define PARSE_H

#define HEADER_MAGIC 0x4A756A75

struct dbheader_t {
    unsigned int magic;
    unsigned short version;
    unsigned short employee_count;
    unsigned int file_size;
};

struct employee_t {
    char first_name[32];
    char last_name[32];
    char address[64];
    unsigned int hours;
    bool is_manager;
};

int create_db_header(int fd, struct dbheader_t**);
int validate_db_header(int fd, struct dbheader_t**);
int read_employees(int fd, struct dbheader_t*, struct employee_t**);
int add_employee(struct dbheader_t*, struct employee_t*, char*);
void output_file(int fd, struct dbheader_t*, struct employee_t*);
void list_employees(struct dbheader_t*, struct employee_t*);
int remove_employee(struct dbheader_t*, struct employee_t*, char);

#endif
