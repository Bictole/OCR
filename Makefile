#Makefile


CC = gcc

CPPFLAGS= `pkg-config --cflags sdl` -MMD
CFLAGS = -fsanitize=address -g -Wall -Wextra -std=c99
LDFLAGS = -fsanitize=address
LDLIBS = -lSDL -lSDL_image -lm `pkg-config --libs sdl`

# SRC contain all the file (.c) we must built
SRC = main.c Segmentation/segmentation.c Segmentation/rlsa.c Tools/tools.c Black_White/Black_White.c Reconstruction/reconstruction.c
OBJ = $(SRC:.c=.o)
DEP = ${SRC:.c=.d}
EXEC = main

all: $(EXEC)

$(EXEC): $(OBJ)

clean:
	$(RM) $(OBJ) $(DEP)

properclean: clean
	 rm $(EXEC) 

-include $(DEP)
