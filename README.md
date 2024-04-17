# Employee Database

To build:

```bash
$ make # This adds create a test database with sample data
$ ./bin/dbview -f <filename> -n
```

To run:

```bash
$ ./bin/dbview -f <filename>
```

To add employee:

```bash
$ ./bin/dbview -f <filename> -a "Joe,Blogg,4 Main Street,5,false"

# Format is FirstName,LastName,Address,NbOfHours,IsManager
```

To list employees:

```bash
$ ./bin/dbview -f <filename> -l
```
