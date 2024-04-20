.POSIX:
.SUFFIXES:

SHELL = /bin/sh
TARGET = bin/employee_database

all: create_folders $(TARGET)

testdb: all
	rm -f ./resources/test.db
	./$(TARGET) -f ./resources/test.db -n
	./$(TARGET) -f ./resources/test.db -a "Joe,Blogg,4 Main Street,5,false"
	./$(TARGET) -f ./resources/test.db -a "Jill,Jones,6 Main Street,50,true"
	./$(TARGET) -f ./resources/test.db -a "Martin,Smith,8 Main Street,500,false"

clean:
	rm -f bin/*
	rm -f obj/*.o

create_folders:
	mkdir -p bin
	mkdir -p obj
	mkdir -p resources

$(TARGET): $(patsubst src/%.c, obj/%.o, $(wildcard src/*.c))
	gcc -o $@ $?

obj/%.o : src/%.c
	gcc -c $<  -o $@ -Iinclude
