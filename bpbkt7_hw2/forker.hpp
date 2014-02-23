namespace Ben {

   int circularForkSearch( const int s, const int P
       , const std::vector<std::vector<float>> D );

   void circularSubvectorMatch(const std::vector<float> key);

   int circleSearch( const int s[]
                   , const std::vector<std::vector<float>> D
                   , const std::vector<float> SV
                   , std::vector<std::vector<float>> RV );

   const std::vector<std::vector<float>> floatVectorFromFile(int argc, const char *argv[], int chunkSize);


}