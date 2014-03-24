#include <iostream>

#include <boost/thread.hpp>
#include <boost/lexical_cast.hpp>

#include "MyFibThreader.hpp"

int main (int argc, char * argv[])
{
	if (argc < 3)
	{
		std::cout << "Usage" << argv[0] << " <threads> <N> " << std::endl;
		exit(1);
	}
	
	unsigned int threads = boost::lexical_cast<unsigned int>(argv[1]);
	unsigned int N = boost::lexical_cast<unsigned int>(argv[2]);
	
	std::cout << "Computing Fibonacci sequence to " << N << " using " << threads << " threads." << std::endl;
	
	scottgs::MyFibThreader parallelizeMe(N); // my noncopyable object	

	// Create a thread group	
	boost::thread_group tg;
	
	// For the number of threads requested
	// Create a thread passing in the reference to the callable object
	for (int t = 0; t < threads ; ++t)
		tg.create_thread( boost::ref(parallelizeMe) );
		
	// Block until all threads are completed
	// The print answer
	std::cout << "About to block and wait on all my threads." << std::endl;
	tg.join_all();
		
	std::cout << "Computed Fibonacci(" << N <<") = " << parallelizeMe.getAnswer() << std::endl;
	std::cout << "CHECK : Fib(10)  = 55" << std::endl;
	std::cout << "CHECK : Fib(25)  = 75025" << std::endl;
	std::cout << "CHECK : Fib(50)  = 12586269025" << std::endl;
	std::cout << "CHECK : Fib(100) = 354224848179261915075" << std::endl;

}
