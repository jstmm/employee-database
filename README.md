# Employee Database

Simple CLI application to store information about employees into a file.

- Create a new database file
- Add and remove employee records
- Search or list all employee records

## Build

```console
$ make      # OR
$ make test # to create a test database
```

## Commands

```console
# To create a new database 'storage.db'
$ ./bin/employee_database -f storage.db -n

# To add employee - the format is FirstName,LastName,Address,NbOfHours,IsManager
$ ./bin/employee_database -f storage.db -a "Joe,Blogg,4 Main Street,5,false"

# To remove employees
$ ./bin/employee_database -f storage.db -r <index from list>

# To search employees by first or last name
$ ./bin/employee_database -f storage.db -s Blogg

# To list employees
$ ./bin/employee_database -f storage.db -l
```
