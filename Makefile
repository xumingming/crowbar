TARGET = crowbar
CC = gcc
OBJS = \
	lex.yy.o\
	y.tab.o

CFLAGS = -c -g -Wall -Wswitch-enum -ansi -DDEBUG
INCLUDES = \

$(TARGET):$(OBJS)
	cd ./memory; $(MAKE);
	cd ./debug; $(MAKE);
	$(CC) $(OBJS) -o $@ -lm

clean:
	rm -rf *.o lex.yy.c y.tab.c y.tab.h *~

y.tab.h:
	bison --yacc -dv crowbar.y
y.tab.c: crowbar.y
	bison --yacc -dv crowbar.y
lex.yy.c: crowbar.l crowbar.y y.tab.h
	flex crowbar.l
