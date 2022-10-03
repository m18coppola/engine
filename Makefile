# -*-Makefile-*-

CC = gcc
FLAGS=`sdl2-config --libs --cflags` -W -Wall -g
LIBS= -lGLEW -lGL -lm -lreadline -lpthread

all: main

main: main.o window.o events.o cmd.o
	$(CC) main.o window.o events.o cmd.o $(LIBS) $(FLAGS) -o app.bin

main.o: main.c
	$(CC) main.c $(LIBS) $(FLAGS) -c -o main.o

events.o: events.c
	$(CC) events.c $(LIBS) $(FLAGS) -c -o events.o

window.o: window.c
	$(CC) window.c $(LIBS) $(FLAGS) -c -o window.o

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
	

clean:
	rm *.o
	rm ./app.bin
