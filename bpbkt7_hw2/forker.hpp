namespace Ben {

   float circularSubvectorMatch(const int s
      , const std::vector<float> key
      , const std::vector<float> V );

   int circularForkSearch( const int s, const int P, const int resultSize, const short test
       , const std::vector<std::vector<float>> D );

   int circleSearch( const int s[]
                   , const std::vector<std::vector<float>> D
                   , const std::vector<float> SV
                   , std::vector<std::vector<float>> RV );

   const std::vector<std::vector<float>> 
      floatVectorFromFile( int argc, const char *argv[], int chunkSize, int blockSize );


}