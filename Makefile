#Makefile

CC = gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS = -fsanitize=address -g -Wall -Wextra -std=c99
LDFLAGS = -fsanitize=address
LDLIBS = -lSDL -lSDL_image -lm `pkg-config --libs sdl`

# SRC contain all the file (.c) we must built
SRC = src/main.c src/Segmentation/segmentation.c src/Segmentation/rlsa.c src/Tools/tools.c src/Black_White/Black_White.c src/Reconstruction/reconstruction.c
OBJ = $(SRC:.c=.o)
DEP = ${SRC:.c=.d}
EXEC = src/main

all: $(EXEC)

$(EXEC): $(OBJ)

clean:
	$(RM) $(OBJ) $(DEP)

properclean: clean
	 rm $(EXEC) 

-include $(DEP)
