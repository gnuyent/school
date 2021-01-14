# Manan Katwala | CS 320 | Fall 2020 | cssc2604

CXX=g++
CXXFLAGS=-g -Wall
CXXIGNOREWRITESTR=-Wno-write-strings
CXXOUTFLAG=-o p5

main : iadrv.cpp intArray.o
	$(CXX) $(CXXFLAGS) $(CXXOUTFLAG) iadrv.cpp intArray.o

nw: iadrv.cpp intArray.o
	$(CXX) $(CXXFLAGS) $(CXXOUTFLAG) $(CXXIGNOREWRITESTR) iadrv.cpp intArray.o

intArray.o : intArray.cpp intArray.h
	$(CXX) $(CXXFLAGS) -c intArray.cpp

clean :
	rm *.o

