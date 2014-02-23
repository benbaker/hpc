/* Includes */
#include "Splitter.hpp"
#include "rvec.hpp"

#include <boost/lexical_cast.hpp>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <stdio.h>

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

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <cstdlib>
#include <set>


struct SVDRT {
   float x; 
   float y; 
   float distance;
   int offset; 
   bool operator<(SVDRT const &SVDR) const {
     return distance > SVDR.distance;
   }
};


namespace Ben {

// -----------------------------------------------------------------------------
// Forks and find matches on circular vector sets.
// -----------------------------------------------------------------------------

   int circularForkSearch( const int s, const int P
       , const std::vector<std::vector<float>> D ){

      int threadCount = P;
      Ben::Splitter splitter;

      unsigned int procs = boost::lexical_cast<unsigned int>(threadCount);
      std::cout << "Testing multifork using " << procs << " threads." << std::endl;
      
      for (int p = 0; p < procs ; ++p) {

         pid_t pid = splitter.spawn();
         if (pid < 0) {
            std::cerr << "Could not fork!!! ("<< pid <<")" << std::endl;
            
            // do not exit, we may have a process
            // spawned from an earlier iteration
            break; 
         }

         if (0 == pid) { // Child


            std::cout << "Child Process (" << pid << ")" << std::endl;  

            // Do work
               


            for(int v = 0; v < 1000; v++){

            // Big V
            std::cout << "\n# " << v << "\t" << "D[v]Size:" << D[v].size() << " circularSize: " << D[v].size() + s << "\n";


               // std::cout << "\n\nV[" << v << "] - circularSize: " << SV.size() + s << "\n";

               // little v 
               // for(int i = 0; i < SV.size() + s; i++){
                  // std::cout << i << ":" << i%SV.size()  << ":" << D[v][i%SV.size() ] << ", ";
               // }
            }

            // sleep(p+2);
            _exit(0);


         } else {

            ; // Nothing to do for the parent
         }
      }

      // Blocking function
      splitter.reap_all(); 
   }


// -----------------------------------------------------------------------------
// Find matches on circular vector sets against key
// -----------------------------------------------------------------------------

   void circularSubvectorMatch(const std::vector<float> key){

      // dist = INF;
      // size_t temp_offset, i, key_size = key.size();
      // float temp;

      // for(temp_offset = 0; temp_offset < 360; temp_offset += 5){
      //    temp = 0;
      //    for(i = 0; i < key_size; ++i){
      //       temp += abs(v[((temp_offset+i)%360)] - key[i]);
      //       if(temp >= dist) break;
      //    }
      //    if(temp < dist){
      //       offset = temp_offset;
      //       dist = temp;
      //    }
      // }
   }


// -----------------------------------------------------------------------------
// Reads in a giant float vector vector from a csv file
// -----------------------------------------------------------------------------
   const std::vector<std::vector<float>> 
      floatVectorFromFile(int argc, const char *argv[], int chunkSize){

   // Get filename or die
   if(argc < 2){ std::cout << "Please provide a file name!\n";exit(1); }
   // Open file or die
   std::ifstream fin(argv[1], std::ios::in);
   if (!(fin)){ std::cout << "File '" << argv[1] << "' not found!\n";exit(1);}
      std::string line;
      int element_count = 0; int i = 0; int j = chunkSize; int k=0;
      std::vector<std::vector<float>> D(36200000);
      while(getline(fin, line)){
         std::stringstream ss(line);
         std::string num;
         while(getline(ss, num, ',')){
            j > 1 ? j-- : (j=chunkSize, k++, i=0);
            // if (j == chunkSize || j == chunkSize-1 ){
               // D.at(k).push_back(stof(num));
               // i=0;
            // }
            ++element_count;
            D.at(k).push_back(stof(num));
            // i%5 ?i-- : (D.at(k).push_back(stof(num)),++element_count, i--); // every 5 
         }
      }
      // std::cout << "\n" << element_count << " elements pulled in from csv file." << std::endl;
      return D;
   }










// -----------------------------------------------------------------------------
// ###### DEAD ######
// Forks and searches 
// -----------------------------------------------------------------------------
   int circleSearch( const int s[]
                   , const std::vector<std::vector<float>> D
                   , const std::vector<float> SV
                   , std::vector<std::vector<float>> RV  ){
      int p;
      for(int l=0; l < 4; l++){
         pid_t pID = fork(); if (pID < 0) { std::cerr << "Failed to fork"; exit(1); }
         std::cout << " New Child - Circular Search Size: " << s[l] << std::endl;
         std::cout << "D:" << D.size() << "  v:" << SV.size() << std::endl;
         if (pID == 0) { p++; //child
            // Big V
            for(int v = 0; v < 1000; v++){
               std::cout << "V[" << v << "] - circularSize: " << SV.size() + s[l] << "\n";
               // little v 
               for(int i = 0; i < SV.size() + s[l]; i++){
                  std::cout << i << ":" << i%SV.size()  << ":" << D[v][i%SV.size() ] << ", ";
               }
            }
         } else { // parent
            return 0;
            wait(&p);
         }
      }
      return 1;
   }


   // V[v] = generateRandomVector(BIG_V_SIZE);

}







