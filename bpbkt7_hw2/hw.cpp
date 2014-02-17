// ----------------------------------------------------------------------------
//   Ben Baker
//   CS4001 HPC hw2
// 
// 1.9 million line file:30-D vector 
// 72 measurements per line, 
// 15 seconds to find the 1000-nearest neighbors.
// ----------------------------------------------------------------------------

#include <string>
#include <vector>
#include <chrono>
#include <limits>
#include <numeric>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <algorithm>
#include "rvec.hpp"

// -----------------------------------------------------------------------------
// Classes
// -----------------------------------------------------------------------------

class SV {
  public:
    short size, findCount;
    SV() {
        size=100;
        findCount=10;
    }
    int getSize() { return size; }
};

// class SVD {
//   public:
//     float x,y,size;
//     short findCount;

//     SV() {
//          m_nNumerator = 0;
//          m_nDenominator = 1;
//     }
// };



// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------


int main( int argc, char **argv ){

    // Get start time
    std::chrono::high_resolution_clock::time_point start 
        = std::chrono::high_resolution_clock::now();

    SV sv;
    std::cout << sv.getSize() << std::endl;

    std::vector<float> test = generateRandomVector(2000);
    for(size_t i = 0; i < test.size(); ++i){
       std::cout << test[i] << std::endl;
    }



    // Get stop time
    std::chrono::high_resolution_clock::time_point stop 
        = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> time_span 
        = std::chrono::duration_cast<std::chrono::duration<double> >
        (stop - start);

    // if(argc < 2){ std::cout << "Provide filename\n"; return 1;}
    return 0;
}




