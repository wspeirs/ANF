GPP   = /usr/bin/g++
GCC   = /usr/bin/gcc
FLAGS = -std=c++11 -W -Wall -g -Wctor-dtor-privacy -Wold-style-cast -Woverloaded-virtual
LIBS  =
EXEC  = anf

all: main.o
	$(GPP) $(FLAGS) $(LIBS) *.o -o $(EXEC)

main.o: main.cpp anf.h
	$(GPP) -c $(FLAGS) main.cpp

clean:
	rm *~ *.o $(EXEC)
