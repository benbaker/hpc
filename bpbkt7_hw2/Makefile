OPT = -std=c++0x
TEST = ./hw
OTHER_FLAGS=$(ENV_HPC_OPTS) 
GOOD = -O -funroll-loops -march=native -fstrict-aliasing

OFLAGS = -Wall -c $(OPT)
CFLAGS = -Wall $(OPT)


all: hw
	@$(TEST)

# $@ matches target; $< matches first dependent
#	g++ $@ -o $< -std=c++0x

hw: hw.o stopwatch.o
	g++ -o hw hw.o stopwatch.o ${OPT} ${GOOD}

hw.o: hw.cpp rvec.o 
	g++ -c $< ${OPT} ${GOOD}

stopwatch.o: stopwatch.cpp
	g++ -c $< ${OPT} ${GOOD}

rvec.o: rvec.cpp
	g++ -c $< ${OPT} ${GOOD}

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
	@rm -rf *.o cachegrind* hw 2>/dev/null || true

