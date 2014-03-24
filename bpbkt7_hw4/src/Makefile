# set this to what I want
OTHER_FLAGS= $(-std=c++0x -O3 -lpthread)

# Standard all target
all: hw1

# HW 1
hw1: hw1.o MatrixMultiply.o
	g++ -o hw1 hw1.o MatrixMultiply.o -lpthread 

hw1.o: hw1.cpp 
	g++ ${OTHER_FLAGS} -c hw1.cpp

matrixMultiply.o : MatrixMultiply.hpp MatrixMultiply.cpp
	g++ -lpthread -c MatrixMultiply.cpp -pthread

clean:
	rm -f *.o hw1


