#include "stopwatch.hpp"

using namespace std;
using namespace std::chrono;

// -----------------------------------------------------------------------------
// Joe F. Made this in 2014
// -----------------------------------------------------------------------------

// If stopwatch is instantiated without 'false' as an argument
// the stopwatch is created and timer is started. On method 'stop()'
// the time span from the start of the watch is returned.
// 'lap()' creates new timespan, meant to be used in loops/recusrsive functions,
// to be used for getting the average of a particular process.

Stopwatch::Stopwatch(){
   start();
}

Stopwatch::Stopwatch(bool start_state){
   if(start_state){
      start();
   }
}

void Stopwatch::start(){
   laps.clear();
   on = true;
   begin = epoch = high_resolution_clock::now();
}

double Stopwatch::stop(){
   end = high_resolution_clock::now();
   laps.push_back( duration_cast<duration<double>> (end - epoch).count());
   on = false;
   return total();
}

double Stopwatch::lap(){
   auto temp = begin;
   begin = high_resolution_clock::now();
   laps.push_back( duration_cast<duration<double>> (begin - temp).count());
   return last_lap();
}

double Stopwatch::total(){
   if(on){
      return duration_cast<duration<double>> (high_resolution_clock::now() - epoch).count();
   }
   return duration_cast<duration<double>> (end - epoch).count();
}

double Stopwatch::current_lap(){
   if(on){
      return duration_cast<duration<double>> (high_resolution_clock::now() - begin).count();
   }
   return duration_cast<duration<double>> (end - begin).count();
}

double Stopwatch::average(){
   return on? total()/(laps.size() + 1) : total()/(laps.size()); 
}

double Stopwatch::last_lap(){
   return *(laps.end()-1);
}