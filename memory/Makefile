CC=gcc
CFLAGS = -c -g -DDEBUG -Wall -ansi -pedantic -Wlong-long -Wcomment
OBJS = memory.o storage.o
TARGETS = test testp

testp : ${OBJS} main.o
	$(CC) -o $@ main.o ${OBJS}
.c.o:
	$(CC) $(CFLAGS) -I.. $*.c
main.o: main.c ../MEM.h

memory.o: memory.c memory.h ../MEM.h
storage.o: storage.c memory.h ../MEM.h
test: *_test.c ${OBJS} ../MEM.h
	${CC} -I.. -o $@ *_test.c ${OBJS}
	./$@

clean:
	rm -f *.o
	rm -f ${TARGETS}
