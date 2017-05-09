UNAME := $(shell uname -s)
CC = g++
OBJ = MazeRunner.o Rules.o
LDFLAGS = -lGL -lGLU -lglut

# Mac OS
ifeq ($(UNAME), Darwin)
	LDFLAGS = -framework OpenGL -framework glut
endif

MazeRunner : $(OBJ)
	$(CC) -o MazeRunner $(OBJ) $(LDFLAGS)

MazeRunner.o : MazeRunner.cpp MazeRunner.h Rules.h
	$(CC) -c -g MazeRunner.cpp

Rules.o : Rules.cpp Rules.h MazeRunner.h
	$(CC) -c -g Rules.cpp

clean :
	rm $(OBJ)
