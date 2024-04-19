# Employee Database

Simple CLI application to store information about employees into a file.

- Create new files
- Add/Remove/List employees

## Build

```console
$ make
$ make testdb # OR this to add a test database
```

## Commands

```console
# To create a new database 'employees.db'
$ ./bin/dbview -f employees.db -n

# To add employee - the format is FirstName,LastName,Address,NbOfHours,IsManager
$ ./bin/dbview -f employees.db -a "Joe,Blogg,4 Main Street,5,false"

# To remove employees
$ ./bin/dbview -f employees.db -r <index from list>

# To list employees
$ ./bin/dbview -f employees.db -l
```
