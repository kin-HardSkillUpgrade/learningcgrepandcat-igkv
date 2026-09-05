CC = gcc
FLAGS = -Wall -Werror -Wextra -std=c11 
SRC = ccat.c ccat_lib.c
OBJ = $(patsubst %.c, %.o, $(SRC))

all: ccat clean_o 

 
ccat: $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o ccat

%.o: %.c
	$(CC) $(FLAGS) -c $< -o $@

clean:
	rm -rf ccat


clean_o:
	rm -rf $(OBJ)


rebuild: clean all

.PHONY: all clean clean_o rebuild test