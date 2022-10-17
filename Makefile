# -*-Makefile-*-

CC = gcc
FLAGS=`sdl2-config --cflags` -W -Wall -g
LIBS= -lGLEW -lGL -lm -lreadline `sdl2-config --libs`
SRCS = $(wildcard *.c)
OBJS = $(patsubst %.c, %.o, $(SRCS))

all: clear_screen main

main: $(OBJS)
	$(CC) $(LIBS) $(FLAGS) $^ -o app.bin

%.o: %.c
	$(CC) $(LIBS) $(FLAGS) -c $< -o $@

run: main
	$(info )
	$(info ___STARTING EXECUTION___)
	$(info ========================)
	$(info )
	./app.bin

memcheck: main logs
	$(info )
	$(info ___STARTING EXECUTION___)
	$(info ____CHECKMEM ENABLED____)
	$(info ========================)
	$(info )
	valgrind --tool=memcheck --leak-check=full --log-file="logs/memcheck.log" ./app.bin

cachecheck: main logs
	$(info )
	$(info ___STARTING EXECUTION___)
	$(info ___CHECKCACHE ENABLED___)
	$(info ========================)
	$(info )
	valgrind --tool=cachegrind --log-file="logs/cachecheck.log" ./app.bin

logs::
	mkdir -p logs

clear_screen::
	clear

clean::
	rm -f *.o
	rm -f ./app.bin

install: main
	cp app.bin ~/.local/bin/rinse
