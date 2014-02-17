OPT = -std=c++0x
CLEAN = rm -rf *.o hw0
TEST = ./hw0 /content/cs/hpc/data/sp14_1k.csv

all: hw0
	@$(TEST)

# $@ matches the target; $< matches the first dependent
#	g++ $@ -o $< -std=c++0x

hw0: hw0.o
	g++ hw0.o -o hw0 -std=c++0x

hw0.o: hw0.cpp
	g++ -c $< -std=c++0x
  
clean:
	$(CLEAN)


