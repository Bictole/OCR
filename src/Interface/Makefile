################################################################################
# Variables (you might need to change these)
################################################################################

# The directory for intermediate files
TMP=artifacts/

# The directory for the end product :
BIN=bin/

# The executable to be produced
EXEC=$(BIN)test

# The source files
SRC= $(wildcard *.c)

# The intermediate files
OBJ= $(addprefix $(TMP),$(SRC:.c=.o))

# The dependency files
DEP= $(addprefix $(TMP),$(SRC:.c=.d))

################################################################################
# Compiler options (probably a bad idea to change these)
################################################################################

# Compiler :
CC = gcc

# Preprocessor options :
CPPFLAGS = -MMD

# Compiler flags and options :
CFLAGS = -Wall -Wextra -Werror -std=c99 -O2

# Linker options :
LDFLAGS =

# Libraries and path for linker :
LDLIBS = -lm -lSDL2-2.0 -lSDL2_image

################################################################################
# Rules (you should not need to change these unless you are adding new targets)
################################################################################

# Rule to build program :
all: $(EXEC)

# Rule to build the Neural Network Test binary
$(EXEC): $(OBJ)
	@mkdir bin 2>/dev/null || true
	$(CC) -o $@ $^ $(LDFLAGS) $(LDLIBS)

# Rule to build .o files from .c files
$(TMP)%.o: %.c
	@mkdir artifacts 2>/dev/null || true
	$(CC) -o $@ -c $< $(CFLAGS)

# Clean up intermediate files
.PHONY: clean-all # Ensures clean is always fully executed
clean:
	rm -rf $(OBJ) $(DEP)

# Clean up all files (to rebuild the project entirely)
clean-all: clean
	rm -rf $(EXEC)

-include $(DEP)
