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
#include <ostream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <algorithm>
#include "forker.hpp"
#include "stopwatch.hpp"

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

static const int s[] = {9,11,17,29}; // search sizes

int P = 1;
int N = 3;
int chunkSize  = 362;
int blockSize  = 100;
int resultSize = 10;



int main( int argc, const char **argv){ 

   Stopwatch timer;

   const int P =  atoi(argv[2]);
   const int N =  atoi(argv[3]);

   // 1:Load data, D, from file into Shared Memory 

   const std::vector<std::vector<float>> D = Ben::floatVectorFromFile( argc, &argv[0], chunkSize, blockSize );
   // std::cout << "import took " << timer.total() << " seconds" << std::endl;
   // std::cout << "D.size()="<< D.size() << " " << "D[1].size()=" << D[1].size() << std::endl;


   // 2: for Each vector size 9;11;17;29 do
   // 3: Run Test Vector against circularSubvectorMatch(Ts, D=P, N).

   int test = 1;
   for(int i = 0; i < 4; i++) {
      std::cout << "searchSize: " << s[i] << std::endl;
      Ben::circularForkSearch( s[i], P, N, test, D );
   }

   std::cout << "\nTest vectors took " << timer.total() << " seconds\n\n" << std::endl;

   test = 0;


   timer.start();

     // 4: Generate V as a set of 30 random vectors of length s
     for(int i = 0; i < 4; i++) {
        std::cout << "searchSize: " << s[i] << std::endl;
        Ben::circularForkSearch( s[i], P, N, test, D );
     }
   std::cout << "\n30 random vector operation took " << timer.total() << " seconds\n";

   std::cout << "Using " << P << " processes to get top " << N << std::endl;

   // 11: Report Search Time for v

   timer.stop();
   // std::cout << "operation took " << timer.total() << " seconds\n";
	 return 0;
}


// for each size s, print out top ten


// -----------------------------------------------------------------------------
// instrucitons
// -----------------------------------------------------------------------------

// 1: Load data, D, from file into Shared Memory
    // 2: for Each vector size 9;11;17;29 do
        // 3: Run Test Vector against circularSubvectorMatch(Ts, D=P, N) . 
        // Verify and display self test results
        // 4: Generate V as a set of 30 random vectors of length s
        // 5: for Each vector v 2 V do
        // 6: Using P parallel processes
        // 7: circularSubvectorMatch(s, D=P, N)
        // 8: where D=P is an even portion of D searched by one of the P processes
        // 9: Merge P Partial Results
        // 10: Alternative to Merge, you can use Shared Memory and Mutex 
        // to merge results during searching
        // 11: Report Search Time for v
        // 12: Report Match Results for v
        // 13: end for
    // 14: end for
// 15: Free and Release Shared Memory
// 16: Report average search time for each size, s.
// Your output will be formatted as shown below, to include the:
// test vector, time, and rank-ordered top-ten matched
// Dissimilarity:X:Y:Oset, assuming N = 10.
// Search Vector: comma_separated_search_values
// Time (s) : ???
// Results:
// 1. dissimilarity1:X1:Y1:Offset1
// 2. dissimilarity2:X2:Y2:Offset2
// . . .
// 10. dissimilarity10:X10:Y10:Offset10


// struct SVDRT {
//    float x; 
//    float y; 
//    float distance;
//    int offset; 
//    bool operator<(SVDR const &SVDR) const {
//      return distance > SVDR.distance;
//    }
// };


// std::vector<SVDRT> res(1000);
// int i= 100;
// while(i--)
//    std::cout << res[i].distance;


// -----------------------------------------------------------------------------
// overload the operator to print vectors
// -----------------------------------------------------------------------------

// Oh yeah, and you can overload the print function (cout <<) with this :
// ostream& operator<<(ostream& os, const SearchVectorDataPoint& sv){...}
// then inside: { os << sv.distance << endl;  } or whatever
// so then you can just print them like:
// std::cout << “this data point is: “ << *sv_pntr; or whatever.

// load D -> shared memory
// for each vector sizes 9 11 17 29
// test vector against circularSubvectorMatch(Ts, D=P, N)
// foreach V[v] 
// use P processes
// circularsubvectormatch()

// -----------------------------------------------------------------------------
// Overzealous oopiness to revisit later maybe
// -----------------------------------------------------------------------------

// class SearchVector {
//   public:
//     std::vector<float> searchVector;
//     short size;
//     SearchVector(){ 
//         size = BIG_V_SIZE;
//         searchVector = generateRandomVector(BIG_V_SIZE);
//     }
//     std::vector<float> sv() { return searchVector; }
// };

// class VectorVector {
//   public:
//     std::vector<std::vector<float>> V;
//     float distance;
//     VectorVector(){ 
//         V.reserve(VBIG_V_SIZE);
//     }
// };

// class ResultVector {
//   public:
//     std::vector<std::vector<float>> RV;
//     float distance;
//     ResultVector(){ 
//         RV.reserve(R_COUNT);
//     }
// };



