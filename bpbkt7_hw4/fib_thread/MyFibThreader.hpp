#ifndef _MYFIBTHREADER_HPP
#define _MYFIBTHREADER_HPP

#include <boost/thread/mutex.hpp>
#include <boost/utility.hpp> // for noncopyable

namespace scottgs {

class MyFibThreader : boost::noncopyable
{
public:
	MyFibThreader(unsigned int N);
	~MyFibThreader();
	unsigned long getAnswer() const;
	void operator()();
private:
	
	// Size to compute
	unsigned int _N;

	// sequence memory
	unsigned long * _sequence;
	
	// index we are currently working on
	int _index;
	
	// mutex
	boost::mutex * _mutex;
};

}; // end of scottgs namespace

#endif // _MYFIBTHREADER_HPP

