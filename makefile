# Name: <Richard, Ifebueme>
# NSID: <lns842>
# Student Number: <11371236>
# Course: CMPT214 - <02> 2025

CC = gcc
CFLAGS = -Wall -std=c99 -pedantic -g
LDFLAGS = 

# FOR PERSONAL USE
BRANCHNAME = $(shell git rev-parse --abbrev-ref HEAD 2>/dev/null || echo "False Repo")
MYFILES ?= git.log main.c makefile
###########################################

.PHONY: all clean

all: main

clean:
		@rm -f main *.o

main: main.o sha1.o
		@$(CC) main.o -o main sha1.o $(LDFLAGS)

main.o: main.c
		@$(CC) $(CFLAGS) -c main.c

sha1.o: sha1.c sha1.h
		@$(CC) $(CFLAGS) -c sha1.c

###########################################
# FOR PERSONAL USE
submission:
		@if [ "$(BRANCHNAME)" == "False Repo" ]; then echo "This is not a git repository"; exit; fi
		@git log main..$(BRANCHNAME) > git.log
		@tar -cvf $(BRANCHNAME).tar $(MYFILES)
		@rm *.log
