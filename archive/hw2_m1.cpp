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
#include "stopwatch.hpp"
#include "forker.hpp"

#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>


#define BIG_V_SIZE  3
#define LITTLE_V_SIZE 72
#define RV_COUNT 5

// -----------------------------------------------------------------------------
// Instantiate text vector, Big V vector, and array of vector sizes.
// -----------------------------------------------------------------------------
std::vector<float> SV = generateRandomVector(BIG_V_SIZE);
std::vector<float> TV(LITTLE_V_SIZE);
std::vector<std::vector<float>> V(LITTLE_V_SIZE * BIG_V_SIZE);
std::vector<std::vector<float>> RV(LITTLE_V_SIZE * RV_COUNT);
std::string br = "\n\n###";
static const int vectorSize[] = {9,11,17,29};

// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------

int main( int argc, char **argv ){
   
   Stopwatch timer;


   // For each circular search vector size
   for(int l=0; l < 4; l++){
      std::cout << br << "\nCircular Search Size: " << vectorSize[l] << "\n";

      // Big V
      for(size_t v = 0; v < BIG_V_SIZE; v++){
         std::cout << "V[" << v << "] - circularSize: " << LITTLE_V_SIZE + vectorSize[l] << "\n";
         V[v] = generateRandomVector(BIG_V_SIZE);
         
         
         pid_t pid;

         int g = 0;

         pid_t pID = fork();
         if (pID < 0) { std::cerr << "Failed to fork" << std::endl;exit(1);}

         if (pID == 0) { //child

            g = 0;
            std::cout << "Child Process" << g << std::endl;

            
         } else { // parent

            std::cout << "Parent Process:" << g << std::endl;;
            ++g;
         }

         // little v 
			for(int i = 0; i < LITTLE_V_SIZE + vectorSize[l]; i++){

            // std::cout << i << ":" << i%LITTLE_V_SIZE << ":" << V[v][i%LITTLE_V_SIZE] << ", ";

            TV[i%vectorSize[l]] = V[v][i%LITTLE_V_SIZE];
			}
      }
   }


	 timer.stop();
    std::cout << "\n\noperation took " << timer.total() << " seconds\n";
	 return 0;
}


// Oh yeah, and you can overload the print function (cout <<) with this :
// ostream& operator<<(ostream& os, const SearchVectorDataPoint& sv){...}
// then inside: { os << sv.distance << endl;  } or whatever
// so then you can just print them like:
// std::cout << “this data point is: “ << *sv_pntr; or whatever.


// std::vector<int> data(10);
// for (int i = 0 ; i < 100 ; ++i){
//     data[i%10] = i;
// }
// for (std::vector<int>::const_iterator it = data.begin() ; it !=data.end(); ++it){
//      std::cout << *it << std::endl;
// }


// load D -> shared memory
// for each vector sizes 9 11 17 29
// test vector against circularSubvectorMatch(Ts, D=P, N)
// foreach V[v] 
// use P processes
// circularsubvectormatch()



// -----------------------------------------------------------------------------
// Classes
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


