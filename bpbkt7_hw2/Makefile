OPT = -std=c++0x
TEST = ./hw
OTHER_FLAGS=$(ENV_HPC_OPTS) 
GOOD = -O -funroll-loops -march=native -fstrict-aliasing

all: hw
	@$(TEST)

# $@ matches target; $< matches first dependent
#	g++ $@ -o $< -std=c++0x

hw: hw.o 
	g++ -o hw hw.o ${OPT} ${GOOD}

hw.o: hw.cpp rvec.o
	g++ -c $< ${OPT} ${GOOD}

rvec.o: rvec.cpp
	g++ -c $< ${OPT} ${GOOD}

clean:
	@for dir in $(SUBDIRS); do \
		$(MAKE) -C $$dir clean; \
	done
	@rm -rf *.o cachegrind* hw2 2>/dev/null || true



