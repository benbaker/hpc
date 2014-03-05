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
#include <deque>
#include <chrono>
#include <limits>
#include <numeric>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <algorithm>
#include <limits>
#include <iomanip>
#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

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


const char *name = "bpbkt7";
int fd;
void  *addr;

namespace Ben {

struct Result {
   float x; 
   float y; 
   float distance;
   int offset;
    bool operator<( const Result& val ) const { 
      return distance < val.distance; 
    }
};

// -----------------------------------------------------------------------------
// Creates and returns a result data structure from x y distance offset
// -----------------------------------------------------------------------------

   Result makeResult(float x, float y, float distance, int offset){
      Result res;
      res.x=x;
      res.y=y;
      res.distance=distance;
      res.offset=offset;
      return res;
   }

// -----------------------------------------------------------------------------
// Prints result set
// -----------------------------------------------------------------------------

   void printResults(const std::deque<Result> &resultSet, int s){

      std::cout  << std::left
            << std::setw(10) << "   x"
            << std::setw(8) <<  "   y"
            << std::setw(7) << "offset"
            << std::setw(10) << "   score"
            << std::endl;

      int l=36; while (l--) std::cout << "-";
      std::cout << std::endl;

      for(auto i = resultSet.size()-1; i ; i--){
         std::cout << std::left
            << std::setw(10) << resultSet.at(i).x
            << std::setw(10) << resultSet.at(i).y
            << std::setw(6)  << resultSet.at(i).offset
            << std::setw(12) << resultSet.at(i).distance / s
            << std::endl;
      }     std::cout << std::endl;
   }



// -----------------------------------------------------------------------------
// inserts into resultSet deque if better match
// -----------------------------------------------------------------------------

   int topInsert(Result r, std::deque<Result> &resultSet, int resultSize) {
      auto it = resultSet.begin();
      while(r < (*it)) ++it;
      resultSet.insert(it, r);
      if(resultSet.size() > resultSize){
         resultSet.pop_front();
         return 1;
      }
   }

// -----------------------------------------------------------------------------
// Find matches on circular vector sets against key
// -----------------------------------------------------------------------------

   Result circularSubvectorMatch(const int s
      , const std::vector<float> key
      , const std::vector<float> V   ){

      Result res;
      const int keysize = s;
      
      float x, y, temp, temptemp, dist;
      size_t temp_offset, i, offset, t;

      for(temp_offset = 0; temp_offset < 360 + s; temp_offset += 5){
         temp = 0;
         dist = 99999999;
         for(i = 0; i < keysize; i++){
            t = ((temp_offset+i) % 360);
            temp += fabs( V[2+t] - key[i] );

            if(temp > dist) break;
         }

         if(temp < dist){
            dist = temp;
            res = makeResult(V[0],V[1],dist,temp_offset);
         }
      }
      return res;
   }

// -----------------------------------------------------------------------------
// Returns hard coded test searchvectors
// -----------------------------------------------------------------------------

   std::vector<float> getSearchVector(int s){

      std::vector<std::vector<float>> searchVectors(66);
      searchVectors[9]  = {0.0536727,0.0384691,0.00146231,0.0122459,0.0198738,-0.116341,0.0998519,0.0269831,-0.000772231};
      searchVectors[11] = {0.0572175,-0.139987,-0.143134,-0.0428729,0.118296,0.0105897,0.0302701,-0.134377,-0.0855214,-0.0757894,0.140506};
      searchVectors[17] = {0.0416003,0.0107126,0.0120675,-0.00547709,-0.00533309,0.00230428,0.00302696,0.0717615,0.0265672,-0.0319207,0.0132625,-0.0669076,-0.0196825,0.0726006,-0.0277976,0.0813408,-0.0819924};
      searchVectors[29] = {0.0208535,0.00124793,0.0221286,-0.0251518,-0.00935498,0.000144995,0.0543099,0.152197,-0.0436211,-0.0536875,-0.0389395,-0.163685,0.105508,0.0135837,-0.0582674,-0.0525398,0.107217,-0.0480279,0.00522108,0.0145284,0.10181,-0.19426,0.0345067,0.126417,-0.143776,0.125843,-0.0239083,0.0613458,-0.265121};
      return searchVectors[s];
   }

// -----------------------------------------------------------------------------
// Forks and find matches on circular vector sets.
// -----------------------------------------------------------------------------

   int circularForkSearch( const int s, const int P, const int N
       , const std::vector<std::vector<float>> D ){

      unsigned int procs = boost::lexical_cast<unsigned int>(P);

      int shmid;
      int sharedIndexID;
      Result *sharedResults;
      unsigned long *sharedIndex;

      key_t shmKey = 86753309; key_t shmKey2 = 313337;

      sharedIndexID = shmget(shmKey2, N * sizeof(unsigned long), IPC_CREAT | 0666);
      sharedIndex = (unsigned long *) shmat(sharedIndexID, NULL, 0);

       if ((shmid = shmget(shmKey, N * sizeof(Result), IPC_CREAT | 0666)) < 0) {
           std::cerr<<"shmget";
           exit(1);
       }

      sharedResults = (Result *) shmat(shmid, NULL, 0);
      *sharedIndex = 0;

      Ben::Splitter splitter;
      pid_t pid;

      std::vector<float> searchVector = getSearchVector(s);



      for (int p = 0; p < procs ; ++p) { 

         pid_t pid = splitter.spawn();
         if (pid < 0) { std::cerr << "Fork failed" << std::endl; break;}

         if (0 == pid) { // Child

            unsigned int share=floor(D.size()/procs)
               , start = (share*p)
               , stop  = p < procs-1 ? (p+1)*share-1 : D.size()-1;
         
            std::cout << "share: " << start << "-" << stop << std::endl;
            std::deque<Result> resultSet (N);
            Result filler;
            filler.distance=99999;
            std::fill (resultSet.begin(),resultSet.end(),filler);

            Result tmp;
            for(auto i = start; i <= stop; i++){
               tmp = circularSubvectorMatch( s, searchVector, D.at(i) );
               topInsert(tmp, resultSet, N);

            }

            for(auto iter = resultSet.begin(); iter < resultSet.end(); iter++){

               (*sharedIndex) +=1;
               sharedResults[(*sharedIndex)] = (*iter);
            }

            sleep(1);
            exit(0);

         } else { //parent

            // wait(NULL);

         }
            splitter.reap_all(); 
      }


      std::deque<Result> orderedResultSet (N);
      Result filler;
      filler.distance=99999;
      std::fill (orderedResultSet.begin(),orderedResultSet.end(),filler);

      int f = N*P;
      Result tmp;

      while (f--){
         tmp = sharedResults[f];
         topInsert(tmp, orderedResultSet, N);
      }

      // std::cout << (*sharedIndex) << std::endl;
      printResults(orderedResultSet, s);

      return 1;
   }

// -----------------------------------------------------------------------------
// Reads in a stupid big float vector vector from a csv file
// -----------------------------------------------------------------------------

   const std::vector<std::vector<float>> 
      floatVectorFromFile( int argc, const char *argv[], int chunkSize, int blockSize ){
      
      // Get filename or die
      if(argc < 2){ std::cout << "Please provide a file name!\n";exit(1); }
      
      // Open file or die
      std::ifstream fin(argv[1], std::ios::in);
      
      // Instantiate things
      if (!(fin)){ std::cout << "File '" << argv[1] << "' not found!\n";exit(1);}
      std::string line;int element_count = 0; int i = 0; int j = chunkSize; int k=0;
      std::vector<std::vector<float>> D(1000);

      // Loop and consume
      while(getline(fin, line)){
         std::stringstream ss(line);
         std::string num;
         while(getline(ss, num, ',')){
            ++element_count;
            D[k].push_back(stof(num));
            if (!--j) {
               k++;
               j=chunkSize;
            }
         }
         line.clear();
      }
      return D;
   }



   // ostream &operator<<(ostream &out, const vector<Result> &v) {
   //     out << v.distance << " " << v.offset;
   //     return out;
   // }




   // GET FILE SIZE FOR MMAP

   // struct stat stat_buf;
   // int rc = stat(argv[1], &stat_buf);
   // long fileSize = rc == 0 ? stat_buf.st_size : -1;



   // OLD FILE READ IN

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



   // PRINT ARRAY

   // void printResults(const Result resultSet[], int s, int N){

   //    std::cout  << std::left
   //          << std::setw(10) << "   x     "
   //          << std::setw(8) << "   y "
   //          << std::setw(7) << "offset  "
   //          << std::setw(10) << "  score"
   //          << std::endl;

   //    int l=36; while (l--) std::cout << "-";
   //    std::cout << std::endl;

   //    for(auto i = N; i ; i++){
   //       std::cout << std::left
   //          << std::setw(10) << resultSet[i].x
   //          << std::setw(10) << resultSet[i].y
   //          << std::setw(6)  << resultSet[i].offset
   //          << std::setw(12) << resultSet[i].distance / s
   //          << std::endl;
   //    }     std::cout << std::endl;
   // }






   // MEMORY MAP

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



   // SEMAPHORE STUFF


   // int semId;        // ID of semaphore set
   // key_t semKey = 123459;     // key to pass to semget(), key_t is an IPC key type defined in sys/types
   // int semFlag = IPC_CREAT | 0666; // Flag to create with rw permissions

   // int semCount = 1;       // number of semaphores to pass to semget()
   // int numOps = 1;      // number of operations to do

   // if ((semId = semget(semKey, semCount, semFlag)) == -1){
   //    std::cerr << "Failed to semget(" << semKey << "," << semCount << "," << semFlag << ")" << std::endl;
   //    exit(1);
   // } else {
   //    std::cout << "Successful semget resulted in (" << semId << std::endl;
   // }

   // // Initialize the semaphore
   // union semun {
   //    int val;
   //    struct semid_ds *buf;
   //    ushort * array;
   // } argument;

   // argument.val = 1; // NOTE: We are setting this to one to make it a MUTEX
   // if( semctl(semId, 0, SETVAL, argument) < 0){
   //    std::cerr << "Init: Failed to initialize (" << semId << ")" << std::endl; 
   //    exit(1);
   // } else {
   //    std::cout << "Init: Initialized (" << semId << ")" << std::endl; 
   // }





} // end namespace







