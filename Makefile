#!/bin/bash
all:
	gcc -o program graphics.c main.c thread_functions.c helper_functions.c -lpthread -lncurses
debug:
	gcc -g program_debug graphics.c main.c thread_functions.c helper_functions.c -lpthread -lncurses
