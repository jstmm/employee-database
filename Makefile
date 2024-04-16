TARGET = bin/dbview
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

run: clean default
		./$(TARGET) -f ./test.db -n
		./$(TARGET) -f ./test.db -a "Joe,Blogg,4 Main Street,5,false"

default: create_folders $(TARGET)

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
