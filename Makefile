TARGET = bin/dbview
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

run: clean default
		./$(TARGET)

default: $(TARGET)

clean:
		rm -f obj/*.o
		rm -f bin/*

$(TARGET): $(OBJ)
		gcc -o $@ $?

obj/%.o : src/%.c
		gcc -g -o $@ -c $< -Iinclude
