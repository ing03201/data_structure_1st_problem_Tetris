# Set compiler to use
CC=g++
CFLAGS=-g -I. -fpermissive
LDFLAGS=
DEPS=Scores.h GameEntry.h SLinkedList.h DLinkedList.h
DEBUG=0

all:: List.exe

List.exe: List.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean: 
	rm -f *.exe *.o *~ *.log *.stackdump
