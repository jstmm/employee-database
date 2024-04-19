TARGET = bin/dbview
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

default: create_folders  $(TARGET)

testdb: default
		rm -f ./test.db
		./$(TARGET) -f ./test.db -n
		./$(TARGET) -f ./test.db -a "Joe,Blogg,4 Main Street,5,false"
		./$(TARGET) -f ./test.db -a "Jill,Jones,6 Main Street,50,true"
		./$(TARGET) -f ./test.db -a "Martin,Smith,8 Main Street,500,false"

clean:
		rm -f obj/*.o
		rm -f bin/*
		rm -f *.db

$(TARGET): $(OBJ)
		gcc -o $@ $?

obj/%.o : src/%.c
		gcc -c $<  -o $@ -Iinclude -g

create_folders:
	mkdir -p bin
	mkdir -p obj
