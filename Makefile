GPP   = /usr/bin/g++
GCC   = /usr/bin/gcc
FLAGS = -std=c++11 -W -Wall -g -O3 -Wctor-dtor-privacy -Wold-style-cast -Woverloaded-virtual
LIBS  =
EXEC  = anf

all: anf.o main.o
	$(GPP) $(FLAGS) $(LIBS) *.o -o $(EXEC)

main.o: main.cpp anf.h
	$(GPP) -c $(FLAGS) main.cpp

anf.o: anf.cpp anf.h
	$(GPP) -c $(FLAGS) anf.cpp


clean:
	rm *~ *.o $(EXEC)
