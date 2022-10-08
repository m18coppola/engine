# -*-Makefile-*-

CC = gcc
FLAGS=`sdl2-config --libs --cflags` -W -Wall -g
LIBS= -lGLEW -lGL -lm -lreadline -lpthread
OBJS = events.o render.o cmd.o

all: clear_screen main

main: main.o $(OBJS)
	$(CC) main.o $(OBJS) $(LIBS) $(FLAGS) -o app.bin

main.o: main.c
	$(CC) main.c $(LIBS) $(FLAGS) -c -o main.o

events.o: events.c
	$(CC) events.c $(LIBS) $(FLAGS) -c -o events.o

render.o: render.c
	$(CC) render.c $(LIBS) $(FLAGS) -c -o render.o

cmd.o: cmd.c
	$(CC) cmd.c $(LIBS) $(FLAGS) -c -o cmd.o

run: main
	$(info )
	$(info ___STARTING EXECUTION___)
	$(info ========================)
	$(info )
	./app.bin

checkmem: main
	$(info )
	$(info ___STARTING EXECUTION___)
	$(info ____CHECKMEM ENABLED____)
	$(info ========================)
	$(info )
	valgrind --leak-check=full --track-origins=yes ./app.bin

checkcache: main
	$(info )
	$(info ___STARTING EXECUTION___)
	$(info ___CHECKCACHE ENABLED___)
	$(info ========================)
	$(info )
	valgrind --tool=cachegrind ./app.bin
	
clear_screen::
	clear

clean:
	rm -f *.o
	rm -f ./app.bin

install: main
	cp app.bin ~/.local/bin/rinse
