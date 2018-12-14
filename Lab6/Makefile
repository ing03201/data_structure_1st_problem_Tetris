# Set compiler to use
CC=g++
CFLAGS=-g -I. -fpermissive
LDFLAGS=
DEPS=
DEBUG=0

all:: LBTree.exe ELBTree.exe Map.exe 

LBTree.exe: LBTree.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

ELBTree.exe: ELBTree.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

Map.exe: Map.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean: 
	rm -f *.exe *.o *~ *.log *.stackdump
