#include <stdio.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

#include "file.h"
#include "parse.h"

typedef struct {
    int id;
    char first_name[64];
    char last_name[64];
    float income;
    bool is_manager;
} Employee;

int main(int argv, char* argc[]) {
    // int fd = open_file_rw(argc[1]);
    int fd = open_file_rw("employees.db");

    struct stat db_stat = {0};
    if (fstat(fd, &db_stat) < 0) {
        perror("fstat");
        close(fd);
        return -1;
    }

    // Add header if database is new
    if (db_stat.st_size == 0) {
        DatabaseHeader header = {0};
        write(fd, &header, sizeof(header));
    }

    DatabaseHeader header = {0};

    if (pread(fd, &header, sizeof(header), 0) != sizeof(header)) {
        perror("read");
        close(fd);
        return -1;
    }

    // Add a new employee
    int new_offset = sizeof(header) + (sizeof(Employee) * header.employees);

    Employee new_employee = {
        ++header.employees,
        "Toto",
        "Titi",
        100000.00,
        false
    };

    pwrite(fd, &new_employee, sizeof(new_employee), new_offset);
    printf("[!] New employee added\n");

    // Update metadata 
    header.version += 1;
    header.file_size = lseek(fd, 0, SEEK_END);
    pwrite(fd, &header, sizeof(header), 0);
    printf("[!] Header updated\n");

    printf("-- Header --\n");
    printf("DB Version: %u\n", header.version);
    printf("DB Number of Employees: %u\n", header.employees);
    printf("File length (recorded): %u Bytes\n", header.file_size);
    
    for (int i = 0; i < header.employees; i++) {
        Employee employee = {};
        pread(fd, &employee, sizeof(Employee), sizeof(header) + (i * sizeof(Employee)));
        printf("#%d - %s\n", employee.id, employee.first_name);
    }

    close(fd);
    return 0;
}
