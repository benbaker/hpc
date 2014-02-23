#include <string>
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <sstream>


namespace Ben {

   const std::vector<std::vector<float>> floatVectorFromCSV(int argc, const char *argv[], int chunkSize){

   // Get filename or die
   if(argc < 2){ std::cout << "Please provide a file name!\n";exit(1); }

   // Open file or die
   std::ifstream fin(argv[1], std::ios::in);
   if (!(fin)){ std::cout << "File '" << argv[1] << "' not found!\n";exit(1);}

      std::string line;
      int element_count = 0; int i = 0; int j = chunkSize; int k=0;
      std::vector<std::vector<float>> D(1000000);

      while(getline(fin, line)){

         std::stringstream ss(line);
         std::string num;
         while(getline(ss, num, ',')){

            j ? j-- : (j=chunkSize, k++, i=0);
            if (j == chunkSize || j == chunkSize-1 ){
               D.at(k).push_back(stof(num));
               i=0;
            }
            ++element_count;
            D.at(k).push_back(stof(num));
            // i%5 ?i-- : (D.at(k).push_back(stof(num)),++element_count, i--); // every 5 
         }
      }
      std::cout << "\n" << element_count << " elements pulled in from csv file." << std::endl;
      return D;
   }

}