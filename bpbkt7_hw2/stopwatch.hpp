#include <chrono>
#include <vector>

class Stopwatch{
   std::chrono::high_resolution_clock::time_point begin, end, epoch;
   std::vector<double> laps;
   bool on;
public:
   Stopwatch();
   Stopwatch(bool start_state);
   void start();
   double stop();
   double lap();
   double total();
   double average();
private:
   double current_lap();
   double last_lap();
};