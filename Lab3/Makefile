# Set compiler to use
CC=g++
CFLAGS=-g -I. -fpermissive
LDFLAGS=
DEPS=
DEBUG=0

all:: Calc.exe mobum.exe

Calc.exe: Calc.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

mobum.exe: mobum.o
	$(CC) -o $@ $^ $(CFLAGS) $(LDFLAGS)

%.o: %.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

%.o: %.cpp $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

clean: 
	rm -f *.exe *.o *~ *.log *.stackdump
