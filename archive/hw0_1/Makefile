OPT = -std=c++0x
CLEAN = rm -rf *.o hw0
TEST = clear; ./hw0 

all: hw0
	@$(TEST)

# $@ matches the target; $< matches the first dependent
#	g++ $@ -o $< -std=c++0x

hw0: CSVparser.o
	g++ CSVparser.o -o hw0 -std=c++0x

CSVparser.o: CSVparser.cpp CSVparser.hpp
	g++ -c $< -std=c++0x

clean:
	$(CLEAN)