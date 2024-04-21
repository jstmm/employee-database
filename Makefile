.POSIX:
.SUFFIXES:

TARGET = ./bin/employee_database
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))
DB_TEST = ./resources/test.db

default: create_folders $(TARGET)

test: default
	rm -f $(DB_TEST)
	$(TARGET) -f $(DB_TEST) -n
	$(TARGET) -f $(DB_TEST) -a "Joe,Blogg,4 Main Street,5,false"
	$(TARGET) -f $(DB_TEST) -a "Martin,Jones,6 Main Street,50,true"
	$(TARGET) -f $(DB_TEST) -a "Jill,Jones,6 Main Street,50,true"
	$(TARGET) -f $(DB_TEST) -a "Steve,Martin,8 Main Street,500,false"
	$(TARGET) -f $(DB_TEST) -s martin
	$(TARGET) -f $(DB_TEST) -s jones
	$(TARGET) -f $(DB_TEST) -s john

clean:
	rm -f $(TARGET)
	rm -f obj/*.o

$(TARGET): $(OBJ)
	gcc -o $@ $?

obj/%.o : src/%.c
	gcc -c $< -o $@ -Iinclude -g

.PHONY: create_folders
create_folders:
	mkdir -p bin obj resources
