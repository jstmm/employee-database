.POSIX:
.SUFFIXES:

SHELL = /bin/sh
TARGET = bin/employee_database
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

default: create_folders $(TARGET)

test: default
	rm -f ./resources/test.db
	./$(TARGET) -f ./resources/test.db -n
	./$(TARGET) -f ./resources/test.db -a "Joe,Blogg,4 Main Street,5,false"
	./$(TARGET) -f ./resources/test.db -a "Jill,Jones,6 Main Street,50,true"
	./$(TARGET) -f ./resources/test.db -a "Martin,Smith,8 Main Street,500,false"

clean:
	rm -f $(TARGET)
	rm -f obj/*.o

$(TARGET): $(OBJ)
	gcc -o $@ $?

obj/%.o : src/%.c
	gcc -c $< -o $@ -Iinclude

.PHONY: create_folders
create_folders:
	mkdir -p bin obj resources
