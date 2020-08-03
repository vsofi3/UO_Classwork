#
# CIS330: Lab 3
#
# Makefile to build libraries and test applications.
#

CC=gcc
AR=ar

CFLAGS=-g -O2 -I./include -L./lib
LIBS=-llab3 -lm

all: lab3

obj:
	mkdir -p obj

lib:
	mkdir -p lib

obj/stats.o: src/stats.c include/stats.h obj
	$(CC) $(CFLAGS) -c -o $@ $<

obj/ohno.o: src/ohno.c include/ohno.h obj
	$(CC) $(CFLAGS) -c -o $@ $<

lib/liblab3.a: obj/stats.o obj/ohno.o lib
	$(AR) rc lib/liblab3.a obj/stats.o obj/ohno.o

lab3: src/lab3.c lib/liblab3.a
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)
lib/liblab3.so: obj/stats.o obj/ohno.o lib
	$(CC) -shared -o lib/liblab3.so obj/stats.o obj/ohno.o
lab3_dyn: src/lab3.c lib/liblab3.so
	$(CC) $(CFLAGS) -o $@ $< $(LIBS)
clean:
	rm lab3 lib/liblab3.a obj/ohno.o obj/stats.o
	rmdir obj
	rmdir lib
