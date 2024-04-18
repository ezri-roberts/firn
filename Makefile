CC = gcc
OPT = -O1
CFLAGS = -Wall -Wextra -std=c99
BIN_DIR = bin
OBJ_DIR = obj
BIN = out

OBJS = main.o dir.o list.o menu.o

all: create_dir $(BIN_DIR)/$(BIN)

create_dir:
	@mkdir -p bin
	@mkdir -p obj

# Create executable.
$(BIN_DIR)/$(BIN): $(addprefix $(OBJ_DIR)/,$(OBJS))
	$(CC) -o $@ $^

# Create object files.
$(addprefix $(OBJ_DIR),/%.o):src/%.c
	$(CC) $(CFLAGS) -c -o $@ $^

clean:
	@rm -r bin
	@rm -r obj
