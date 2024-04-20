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
int read_employees(int fd, const struct dbheader_t*, struct employee_t**);
int add_employee(const struct dbheader_t*, struct employee_t*, char*);
int remove_employee(struct dbheader_t*, struct employee_t*, const char* employee_to_remove_index);
void list_employees(const struct dbheader_t*, struct employee_t*);
void save_to_file(int fd, struct dbheader_t*, struct employee_t*);

#endif
