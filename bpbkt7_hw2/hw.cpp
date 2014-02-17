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

#define V_SIZE  4
#define V_COUNT 10
static const int sizes[] = {16,2,77,29};


// -----------------------------------------------------------------------------
// Classes
// -----------------------------------------------------------------------------


class SV {
  public:
    std::vector<float> searchVector;
    short size, findCount;
    SV(){ 
        size = V_SIZE; findCount = V_COUNT;
        searchVector = generateRandomVector(size);
    }
    std::vector<float> sv() { return searchVector; }
};

class SVV {
  public:
    std::vector<std::vector<float>> V;
    float distance;
    SVV(){ 
            
        }
};



// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------


int main( int argc, char **argv ){


    SV sv;
    std::vector<float> test = sv.sv();
    for(size_t i = 0; i < test.size(); ++i){
       std::cout << test[i] << " ";
    }


    SV sv2;
    test = sv2.sv();
    for(size_t i = 0; i < test.size(); ++i){
       std::cout << test[i] << " ";
    }
    // stopwatch




    // stopwatch

    return 0;
}









// The goal is to accelerate the search of a single vector against the whole dataset. Your program must be
// structured as the following algorithm for generating timing results:
// 1: Load data, D, from file into Shared Memory
    // 2: for Each vector size 9;11;17;29 do
        // 3: Run Test Vector against circularSubvectorMatch(Ts, D=P, N) . Verify and display self test results
        // 4: Generate V as a set of 30 random vectors of length s
        // 5: for Each vector v 2 V do
        // 6: Using P parallel processes
        // 7: circularSubvectorMatch(s, D=P, N)
        // 8: . where D=P is an even portion of D searched by one of the P processes
        // 9: Merge P Partial Results
        // 10: . Alternative to Merge, you can use Shared Memory and Mutex to merge results during searching
        // 11: Report Search Time for v
        // 12: Report Match Results for v
        // 13: end for
    // 14: end for
// 15: Free and Release Shared Memory
// 16: Report average search time for each size, s.
// Your output will be formatted as shown below, to include the test vector, time, and rank-ordered top-ten matched
// Dissimilarity:X:Y:Oset, assuming N = 10.
// Search Vector: comma_separated_search_values
// Time (s) : ???
// Results:
// 1. dissimilarity1:X1:Y1:Offset1
// 2. dissimilarity2:X2:Y2:Offset2
// . . .
// 10. dissimilarity10:X10:Y10:Offset10


