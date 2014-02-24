#include "Splitter.hpp"

// -----------------------------------------------------------------------------
// Boiler plate code from Dr Scott. Name space just for simplicity.
// I did not make this.
// -----------------------------------------------------------------------------

namespace Ben {

Splitter::Splitter() { ; } 
Splitter::~Splitter() { ; } 

pid_t Splitter::spawn()
{
	const pid_t pid = fork(); 
	// std::cout << "Splitter::spawn, adding (" << pid << ") to process group, ";	
	_processGroup.insert(pid);
	std::cout << "Splitter::spawn, processGroup size is " << _processGroup.size() << std::endl;	
	return pid;
}

void Splitter::reap_all()
{
	do
	{
		std::cout << "Splitter::reap_all, processGroup size = " << _processGroup.size() << std::endl;		
		pid_t reapedPid = this->reap();
		// std::cout << "Splitter::reap_all, reaped pid (" << reapedPid << ")" << std::endl;		
		_processGroup.erase(reapedPid);
		sleep(1); // just to slow things down
	}
	while (!_processGroup.empty());

	return;
}

pid_t Splitter::reap(pid_t pid)
{
	if (pid > 0)
		return this->reap_this(pid);
	else
		return this->reap_this(-1); // see code
}

pid_t Splitter::reap_this(pid_t pid)
{
	// std::cout << "Splitter::reap_this(" << pid << ")" << std::endl;
	
	// ============================== 
	// All this code is boiler-plate	
	// ============================== 

	int status;	// catch the status of the child
	pid_t returnPid = 0;

	do  // in reality, mulptiple signals or exit status could come from the child
	{

		pid_t w = waitpid(pid, &status, WUNTRACED | WCONTINUED);
		if (w == -1)
		{
			std::cerr << "Splitter::reap_this(" << pid << "), Error waiting for child process" << std::endl;
			break;
		}
		else	// we caught a valid
			returnPid = w;
		
		if (WIFEXITED(status))
		{
			if (status > 0)
			{
				std::cerr << "SSplitter::reap_this(" << pid << "), Child process exited with non-zero status of " << WEXITSTATUS(status) << std::endl;
				continue;
			}
			else
			{
				// std::cout << "Splitter::reap_this(" << pid << "), Child process exited with status of " << WEXITSTATUS(status) << std::endl;
				continue;
			}
		}
		else if (WIFSIGNALED(status))
		{
			std::cout << "Splitter::reap_this(" << pid << "), Child process killed by signal (" << WTERMSIG(status) << ")" << std::endl;
			continue;			
		}
		else if (WIFSTOPPED(status))
		{
			std::cout << "Splitter::reap_this(" << pid << "), Child process stopped by signal (" << WSTOPSIG(status) << ")" << std::endl;
			continue;			
		}
		else if (WIFCONTINUED(status))
		{
			std::cout << "Splitter::reap_this(" << pid << "), Child process continued" << std::endl;
			continue;
		}
	}
	while (!WIFEXITED(status) && !WIFSIGNALED(status));
	
	return returnPid;
}

}; // END of Ben Namespace


