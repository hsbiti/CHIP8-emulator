CC=g++
CFLAGS=-Wall -g #-Wextra -Werror
IFLAGS=-Iinclude
OPATH=obj/

vpath %.cpp src/
vpath %.o obj/
vpath %.h include/


main: main.o |bin
	$(CC) $(CFLAGS) -o main $(OPATH)main.o
	mv main bin/

main.o: main.cpp Chip8.cpp Chip8.h
Chip8.o: Chip8.cpp Chip8.h

%.o : |obj
	$(CC) $(CFLAGS) -c $< $(IFLAGS)
	mv $@ $(OPATH)


bin :
	mkdir bin

obj:
	mkdir obj

clean: |obj
	rm -r obj