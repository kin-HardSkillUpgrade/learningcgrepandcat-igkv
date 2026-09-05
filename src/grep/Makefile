CC = gcc 
FLAGS = -Wall -Werror -Wextra -std=c11 
SRC = ggrep.c ggrep_lib.c
OBJ = $(patsubst %.c, %.o, $(SRC))


all: ggrep clean_o 

ggrep:$(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o ggrep


%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf ggrep

clean_o:
	rm -rf $(OBJ)


rebuild: clean all

.PHONY: all clean clean_o rebuild test