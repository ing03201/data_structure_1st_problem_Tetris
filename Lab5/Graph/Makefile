# Set compiler to use
CC=g++
CFLAGS=-g -I. -fpermissive -Wno-conversion-null -Wno-write-strings
LDFLAGS=
DEPS=
DEBUG=0

all:: DFSMain.exe BFSMain.exe

DFSMain.exe: DFSMain.o DFS.o Graph.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

BFSMain.exe: BFSMain.o BFS.o Graph.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean: 
	rm -f *.exe *.o *~ *.log *.stackdump
