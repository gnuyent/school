#Program 2 makefile

p2 : p2.o llist.o
		gcc -g -Wall -o p2 p2.o llist.o

p2.o : p2.c llist.h
		gcc -g -Wall -c p2.c

llist.o : llist.c llist.h
		gcc -g -Wall -c llist.c

clean :
		rm p2.o llist.o

cleanall :
		rm p2.o llist.o p2
