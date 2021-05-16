OBJ_DIR = obj
SRC_DIR = src

EXEC = bonobo
CFLAGS = -Wall -g -Iinclude

SRC= $(wildcard $(SRC_DIR)/*.c)
_OBJS= $(SRC:.c=.o)

OBJS = $(patsubst $(SRC_DIR)/%, $(OBJ_DIR)/%,$(_OBJS))

all: clear $(EXEC)

clean: 
	@rm -r $(EXEC)* $(OBJ_DIR)/*

clear:
	@clear
	@printf "\n\x1B[1;33mStarted Compilation\n\033[0m"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	gcc $(CFLAGS) -c $< -o $@

$(EXEC): $(OBJS)
	gcc $^ -o $@
	@printf "\n\x1B[1;32mCompilation finished successfully !\n\033[0m"