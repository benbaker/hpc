OPT = -std=c++0x
TEST = ./hw /content/cs/hpc/data/sp14_1k.csv 1 25
OTHER_FLAGS=$(ENV_HPC_OPTS) 
GOOD = -O -funroll-loops -march=native -fstrict-aliasing

OFLAGS = -Wall -c $(OPT)
CFLAGS = -Wall $(OPT)

all: hw
	@$(TEST)

# $@ matches target; $< matches first dependent
#	g++ $@ -o $< -std=c++0x

hw: hw.o stopwatch.o forker.o Splitter.o
	g++ -o hw hw.o stopwatch.o forker.o Splitter.o ${OPT} ${GOOD} ${CFLAGS}

hw.o: hw.cpp rvec.o 
	g++ -c $< ${OPT} ${GOOD}

stopwatch.o: stopwatch.cpp
	g++ -c $< ${OPT} ${GOOD}

rvec.o: rvec.cpp
	g++ -c $< ${OPT} ${GOOD}

forker.o: forker.cpp forker.hpp
	gcc -c $< ${OPT} ${GOOD}

Splitter.o: Splitter.hpp Splitter.cpp
	g++ -c Splitter.cpp

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
	@rm -rf *.o cachegrind* hw 2>/dev/null || true
