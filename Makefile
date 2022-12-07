##########################################################
##                                                      ##
##    Implementace překladače jazyka IFJ22              ##
##                                                      ##
##    Autoři: xlukas18, xmedri01, xpoliv07, xschie03    ##
##                                                      ##
##    Implementace Makefile: xschie03                   ##
##    Datum: 9. 10. 2022                                ##
##                                                      ##
##    Licence: GNU GPL v3, nebo pozdější                ##
##                                                      ##
##########################################################

CC=gcc
CFLAGS=--std=gnu99 -pedantic -Wall -Wextra -Wformat -g -fPIC -O3

HDRS := $(wildcard *.h)
SRCS := $(wildcard *.c)
OBJS := $(SRCS:c=o)

all: compiler

clean:
	rm -rf *.o compiler memory.dbg

fmt:
	clang-format -i *.c *.h --style=WebKit

compiler: $(HDRS) $(SRCS) $(OBJS)
	$(CC) $(CFLAGS) -o compiler *.o

$(OBJS): %.o: %.c %.h Makefile
	$(CC) -c $< $(CFLAGS)

zip: $(HDRS) $(SRCS) Makefile dokumentace.pdf rozdeleni rozsireni
	zip xpoliv07 $^
