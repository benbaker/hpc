all: thread_fib

thread_fib: thread_fib.o MyFibThreader.o
	g++ thread_fib.o MyFibThreader.o -o thread_fib -lboost_thread-mt

thread_fib.o:
	g++ -c thread_fib.cpp

MyFibThreader.o: MyFibThreader.cpp
	g++ -c MyFibThreader.cpp

clean:
	rm thread_fib *.o
