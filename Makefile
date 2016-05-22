OS := $(shell uname)

CC=g++
CFLAGS=-Wall -g -Wno-deprecated #-Wextra -Werror
IFLAGS=-Iinclude
OPATH=obj/
ifeq ($(OS),Darwin)
	GL=-framework OpenGL -framework GLUT
else
	GL=-lglut -lGL -lGLU
endif

vpath %.cpp src/
vpath %.o obj/
vpath %.h include/


main: main.o Chip8.o |bin
	$(CC) $(CFLAGS) -o main $(OPATH)main.o $(OPATH)Chip8.o $(GL)
	mv main bin/

main.o: main.cpp Chip8.cpp Chip8.h
Chip8.o: Chip8.cpp Chip8.h

%.o : |obj
	$(CC) $(CFLAGS) -c $< $(IFLAGS) $(GL)
	mv $@ $(OPATH)


bin :
	mkdir bin

obj:
	mkdir obj

clean: |obj
	rm -r obj
