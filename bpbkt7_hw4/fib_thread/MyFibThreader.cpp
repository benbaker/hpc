#include "MyFibThreader.hpp"

#include <stdexcept>
#include <iostream>

#include <boost/thread/thread.hpp>

scottgs::MyFibThreader::MyFibThreader(unsigned int N) : _N(N)
{
	std::cout << "Constructing MyFibThreader with N = (" << N << ")" << std::endl; 
	if (N < 2)
	{
		std::stringstream msg;
		msg << "Fibonacci sequence can only be calculated for numbers greater than or equal to 2, you gave me : " << N;
		throw std::logic_error(msg.str());
	}
	_sequence = new unsigned long[N+1]; // sequence max index is now N
	_mutex = new boost::mutex();
	
	// The base case of the sequence
	_sequence[0] = 0;
	_sequence[1] = 1;
	_index = 2;
}

scottgs::MyFibThreader::~MyFibThreader()
{
	std::cout << "Destructing MyFibThreader" << std::endl; 
	
	delete _sequence;
	delete _mutex;
}

unsigned long scottgs::MyFibThreader::getAnswer() const
{ 
	return _sequence[_N];
}

void scottgs::MyFibThreader::operator()()
{
	// Who am I?
	const boost::thread::id id = boost::this_thread::get_id(); 
	
	std::cout << "Entered functor for thread (" << id << ")" << std::endl; 
	
	int threadIndex;
	while (true)
	{
		// NOTE: below, all but the sleep is within MUTEX
		// This is a toy example, in real work you only mutex
		// the critical index manipulation
		
		// do MUTEX segment
		{
			boost::mutex::scoped_lock lock(*(_mutex));
			threadIndex = _index;
		
			if (threadIndex > _N)
				break; // kill the threaded work.
		
			++_index; // NOTE: If all threads were independent, I could release the mutex here...
	
			if (threadIndex <= _N)
			{
				std::cout << "Thread (" << id << "), index=" << threadIndex;
				_sequence[threadIndex] = _sequence[threadIndex - 1] + _sequence[threadIndex-2];
				std::cout << " as (" << _sequence[threadIndex] << ")" << std::endl; 
			}
		} // mutex released

		// Could yeild or sleep, yeild is better is real-world
		// because you recapture unused time-slice
		boost::this_thread::sleep(boost::posix_time::milliseconds(500));
	}	
}

