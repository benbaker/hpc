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
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>

#include <cstdlib>
#include <set>

#include <sys/mman.h>
#include <err.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#define handle_error(msg) \
        do { perror(msg); exit(EXIT_FAILURE); } while (0)

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

   void circularSubvectorMatch(const int s
      , const std::vector<float> key
      , const std::vector<float> V   );

// -----------------------------------------------------------------------------
// Forks and find matches on circular vector sets.
// -----------------------------------------------------------------------------

   int circularForkSearch( const int s, const int P
       , const std::vector<std::vector<float>> D ){

      int threadCount=0;;
      Ben::Splitter splitter;

      std::vector<float> searchVector = generateRandomVector(s);

      unsigned int procs = boost::lexical_cast<unsigned int>(P);
      std::cout << "Testing multifork using " << procs << " threads." << std::endl;
      
      // std::cout << "D[v]" << D[v];



      for (int p = 0; p < procs ; ++p) {

         pid_t pid = splitter.spawn();
         if (pid < 0) {
            std::cerr << "Could not fork!!! ("<< pid <<")" << std::endl;
            break; 
         }
         if (0 == pid) { // Child

            std::cout << "\n#p=" << p << std::endl;
            unsigned int share = D.size()/procs;

            for(auto i = share*p; i < (p+1)*share; i++){
               circularSubvectorMatch( s, searchVector, D.at(i) );
            }




            // sleep(1);
            _exit(0);


         } else { //parent

            ;
         }

      }

      // Blocking function
      splitter.reap_all(); 
   }


// -----------------------------------------------------------------------------
// Find matches on circular vector sets against key
// -----------------------------------------------------------------------------

   void circularSubvectorMatch(const int s
      , const std::vector<float> key
      , const std::vector<float> V   ){

      // std::cout << "V[v] - circularSize: " << V.size() + s << "\n";

      // for(int v = 0; v < V.size() + s; v++){
      // }
   }


// -----------------------------------------------------------------------------
// Reads in a stupid big float vector vector from a csv file
// -----------------------------------------------------------------------------

   const std::vector<std::vector<float>> 
      floatVectorFromFile(int argc, const char *argv[], int chunkSize){

      // Get filename or die
      if(argc < 2){ std::cout << "Please provide a file name!\n";exit(1); }
      // Open file or die
      std::ifstream fin(argv[1], std::ios::in);

      // struct stat stat_buf;
      // int rc = stat(argv[1], &stat_buf);
      // long fileSize = rc == 0 ? stat_buf.st_size : -1;

      // Eat file contents
      if (!(fin)){ std::cout << "File '" << argv[1] << "' not found!\n";exit(1);}
      std::string line;
      int element_count = 0; int i = 0; int j = chunkSize; int k=0;
      std::vector<std::vector<float>> D(10000);
      while(getline(fin, line)){
         std::stringstream ss(line);
         std::string num;
         while(getline(ss, num, ',')){
            j-- ? : (j=chunkSize-1, k++);
            ++element_count;
            D[k].push_back(stof(num));
         }
      }
      return D;
   }








   // const std::vector<std::vector<float>> 
   //    floatVectorFromFile(int argc, const char *argv[], int chunkSize){
   // // Get filename or die
   // if(argc < 2){ std::cout << "Please provide a file name!\n";exit(1); }
   // // Open file or die
   // std::ifstream fin(argv[1], std::ios::in);
   // if (!(fin)){ std::cout << "File '" << argv[1] << "' not found!\n";exit(1);}
   //    std::string line;
   //    int element_count = 0; int i = 0; int j = chunkSize; int k=0;
   //    std::vector<std::vector<float>> D(36200000);
   //    while(getline(fin, line)){
   //       std::stringstream ss(line);
   //       std::string num;
   //       while(getline(ss, num, ',')){
   //          j > 1 ? j-- : (j=chunkSize, k++, i=0);
   //          // if (j == chunkSize || j == chunkSize-1 ){
   //             // D.at(k).push_back(stof(num));
   //             // i=0;
   //          // }
   //          ++element_count;
   //          D.at(k).push_back(stof(num));
   //          // i%5 ?i-- : (D.at(k).push_back(stof(num)),++element_count, i--); // every 5 
   //       }
   //    }
   //    // std::cout << "\n" << element_count << " elements pulled in from csv file." << std::endl;
   //    return D;
   // }






   // int mmapTest() {

     //  // MAP_NONBLOCK (since Linux 2.5.46)
     //  //   Only meaningful in conjunction with MAP_POPULATE.  Don't
     //  //   perform read-ahead: create page tables entries only for pages
     //  //   that are already present in RAM.  Since Linux 2.6.23, this
     //  //   flag causes MAP_POPULATE to do nothing.  One day the
     //  //   combination of MAP_POPULATE and MAP_NONBLOCK may be
     //  //   reimplemented.

     //  // MAP_NORESERVE
     //  //   Do not reserve swap space for this mapping.  When swap space
     //  //   is reserved, one has the guarantee that it is possible to
     //  //   modify the mapping.  When swap space is not reserved one might
     //  //   get SIGSEGV upon a write if no physical memory is available.
     //  //   See also the discussion of the file
     //  //   /proc/sys/vm/overcommit_memory in proc(5).  In kernels before
     //  //   2.6, this flag had effect only for private writable mappings.

     //  // MAP_POPULATE (since Linux 2.5.46)
     //  //   Populate (prefault) page tables for a mapping.  For a file
     //  //   mapping, this causes read-ahead on the file.  Later accesses
     //  //   to the mapping will not be blocked by page faults.
     //  //   MAP_POPULATE is supported for private mappings only since
     //  //   Linux 2.6.23.

     // void *addr;
     // int fd;
     // struct stat sb;
     // off_t offset, pa_offset;
     // size_t length;
     // ssize_t s;
     // fd = open("/content/cs/hpc/data/sp14_1k.csv", O_RDONLY);
     // if (fd == -1)
     //     handle_error("open");

     // if (fstat(fd, &sb) == -1)
     //     handle_error("fstat");

     // offset = atoi("3");
     // pa_offset = offset & ~(sysconf(_SC_PAGE_SIZE) - 1);
     //     /* offset for mmap() must be page aligned */

     // if (offset >= sb.st_size) {
     //     fprintf(stderr, "offset is past end of file\n");
     //     exit(EXIT_FAILURE);
     // }

     //     length = sb.st_size - offset;

     // addr = mmap(NULL, length + offset - pa_offset, PROT_READ,
     //             MAP_PRIVATE, fd, pa_offset);

     // if (addr == MAP_FAILED)
     //     handle_error("mmap");

     // s = write(STDOUT_FILENO, addr + offset - pa_offset, length);
     // if (s != length) {
     //     if (s == -1)
     //         handle_error("write");

     //     fprintf(stderr, "partial write");
     //     exit(EXIT_FAILURE);
     // }

     // // exit(EXIT_SUCCESS);

     // // OWW MY BRAIN
   // }








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




// int mmapTest1() {
//         const char str1[] = "string 1";
//         const char str2[] = "string 2";
//         int parpid = getpid(), childpid;
//         int fd = -1;
//         char *anon, *zero;
 
//         if ((fd = open("results.csv", O_RDWR, 0)) == -1)
//                 err(1, "open");
 
//         anon = (char*)mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_ANON|MAP_SHARED, -1, 0);
//         zero = (char*)mmap(NULL, 4096, PROT_READ|PROT_WRITE, MAP_FILE|MAP_SHARED, fd, 0);
 
//         if (anon == MAP_FAILED || zero == MAP_FAILED)
//                 errx(1, "either mmap");
 
//         strcpy(anon, str1);
//         strcpy(zero, str1);
 
//         printf("PID %d:\tanonymous %s, zero-backed %s\n", parpid, anon, zero);
//         switch ((childpid = fork())) {
//         case -1:
//                 err(1, "fork");
//                 /* NOTREACHED */
 
//         case 0:
//                 childpid = getpid();
//                 printf("PID %d:\tanonymous %s, zero-backed %s\n", childpid, anon, zero);
//                 sleep(3);
 
//                 printf("PID %d:\tanonymous %s, zero-backed %s\n", childpid, anon, zero);
//                 munmap(anon, 4096);
//                 munmap(zero, 4096);
//                 close(fd);
//                 return (EXIT_SUCCESS);
//         }
 
//         sleep(2);
//         strcpy(anon, str2);
//         strcpy(zero, str2);
 
//         printf("PID %d:\tanonymous %s, zero-backed %s\n", parpid, anon, zero);
//         munmap(anon, 4096);
//         munmap(zero, 4096);
//         close(fd);
//         // return (EXIT_SUCCESS);
// }






}







