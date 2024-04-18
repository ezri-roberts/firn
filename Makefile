CC = gcc
OPT = -O1
#CFLAGS = -Wall -Wextra -std=c99 -I include/
CFLAGS = -Wall -Wextra -std=c99 -I lib/termbox2/
LIBS = -L lib/termbox2/ -ltermbox2

# ifeq ($(OS), Windows_NT)
# 	MKDIR = @mkdir
# 	RM = @rmdir /s /q
# 	LIBS = -L lib/ -lraylib_win -lgdi32 -lwinmm
# 	BIN = out.exe
# else # Linux
# 	MKDIR = @mkdir -p
# 	RM = @rm -r
# 	LIBS = -L lib/ -lraylib_linux -lGL -lm -lpthread -ldl -lrt -lX11
# 	BIN = out
# endif

BIN_DIR = bin
OBJ_DIR = obj
BIN = out

OBJS = main.o dir.o list.o

all: create_dir termbox2 $(BIN_DIR)/$(BIN)

create_dir:
	@mkdir -p bin
	@mkdir -p obj

# Create executable.
$(BIN_DIR)/$(BIN): $(addprefix $(OBJ_DIR)/,$(OBJS))
	$(CC) $(LIBS) -o $@ $^ $(LIBS)

# Create object files.
$(addprefix $(OBJ_DIR),/%.o):src/%.c
	$(CC) $(CFLAGS) -c -o $@ $^

termbox2:
	$(MAKE) -C ./lib/termbox2/ libtermbox2.a

clean:
	@rm -r bin
	@rm -r obj
	$(MAKE) -C ./lib/termbox2/ clean
