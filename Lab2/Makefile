# Set compiler to use

# Variables
CC=g++
CFLAGS=-g -I. -fpermissive
LDFLAGS=
DEPS=Scores.h GameEntry.h SLinkedList.h DLinkedList.h
DEBUG=0

#output program
all:: List.exe

#Target : Dependency list /n Command list
List.exe: List.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean: 
	rm -f *.exe *.o *~ *.log *.stackdump
