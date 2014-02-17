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

#define BIG_V_SIZE  4
#define LITTLE_V_SIZE 3
#define R_COUNT 2


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

std::string br = "\n#######################";

// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------

int main( int argc, char **argv ){

	 std::vector<float> SV = generateRandomVector(BIG_V_SIZE);
	 std::vector<std::vector<float>> V(LITTLE_V_SIZE);
	 V.reserve(R_COUNT * LITTLE_V_SIZE);

   static const int vectorSize[] = {9,11,17,29};

   // for each vector size
   for(int l=0; l < 4; l++){

        std::cout << br << "Circular Search Size: " << vectorSize[l] << "\n";

        // BIG V
		  for(size_t v = 0; v < V.size(); v++){
				
            std::cout << "V[" << v << "] - circularSize: " << LITTLE_V_SIZE +  vectorSize[l] << "\n";
            V[v] = generateRandomVector(BIG_V_SIZE);



            // LITTLE V
				for(int i = 0; i < LITTLE_V_SIZE + vectorSize[l]; i++){
					 


			      std::cout << " " << ", " << V[v][i%vectorSize[l]];




				}
               std::cout << "\n";
				
				// std::cout << "\n\n";
		  }
	 }

	 // stopwatch



	 // int l=4;
	 // while(l--){
	 //     std::cout << vectorSize[l] << "\n";
	 //     for(size_t i = V.size(); i; --i){
				
	 //         std::vector<float> SV = generateRandomVector(BIG_V_SIZE);
	 //         for(size_t ii = 0; ii < SV.size(); ++ii){
	 //            // std::cout << SV[ii];
	 //         }
	 //         // std::cout << "\n\n";
	 //     }
	 // }


	 // OrderedResultSet circularSubvectorMatch(searchVector, circularDataset, N)


	 // stopwatch
	 
	 std::cout << "\n";

	 return 0;
}






// The goal is to accelerate the search of a single vector against the whole dataset. 
// Your program must be
// structured as the following algorithm for generating timing results:
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


