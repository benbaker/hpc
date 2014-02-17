// ----------------------------------------------------------------------------
//   Ben Baker
//   CS4001 HPC hw0
// 
//  "I know it's late. But at least it's also really bad and broken!!"
// 
//   I'm sick of this one. Each part worked at one point.
//   I learned how to oop, how to C++, how to header files. 
//   If I started over I could make this good but I'm gonna HW1 instead.
// 
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

int   parseCSV(char *fileName);
int   split_line(std::string & l, std::string d, std::vector<double> &a);
void  min_max_vector(std::vector<float> &littleVector, std::vector<float>
        &min, std::vector<float> &max);
float min_val(const std::vector<float> &littleVector, std::vector<float> &min);
float max_val(const std::vector<float> &littleVector, std::vector<float> &max);
float smaller(float &x, float &y);
float  bigger(float &x, float &y);

// -----------------------------------------------------------------------------
// Classes
// -----------------------------------------------------------------------------

class Geo {
  public:
    float x,y;
    long unsigned int v;
    void set_xy (float,float);
};

void Geo::set_xy (float xx, float yy) {
  x = xx;
  y = yy;
};

double biggestDouble = std::numeric_limits<double>::max();

class Extremes {
  public:
    double max_x,min_x,max_y,min_y;
    void set_x(double);
    void set_y(double);
};
void Extremes::set_x(double f) {
    if (f > max_x){ max_x = f; }
    if (f < min_x){ min_x = f; }
};
void Extremes::set_y(double f) {
    if (f > max_y){ max_y = f; }
    if (f < min_y){ min_y = f; }
};

// -----------------------------------------------------------------------------
// Main
// -----------------------------------------------------------------------------

int main( int argc, char **argv ){ 
    // Fail on no file name
    if(argc < 2){ std::cout << "Provide filename\n"; return 1;}
    // Parse the CSV
    parseCSV(argv[1]);
    return 0;
}

// -----------------------------------------------------------------------------
// CSV Parser
// -----------------------------------------------------------------------------

int parseCSV( char * fileName ) {

    std::ifstream fin(fileName, std::ios::in);
    if (!(fin)){
        std::cout << "File '" << fileName << "' not found!\n";
        return 1;
    }
    std::vector<Geo> coords;
    std::vector<double> littleVector, bigVector;
    std::string line;
    unsigned lines=0, items=0;

    // Get start time
    std::chrono::high_resolution_clock::time_point start 
        = std::chrono::high_resolution_clock::now();

// -----------------------------------------------------------------------------
// Line splitter, Float-maker
// -----------------------------------------------------------------------------

    while ( getline ( fin, line, ',' ) ) {
        if (line.find('\n') != std::string::npos) {
            items += split_line(line, "\n", littleVector);
            bigVector.insert(bigVector.end(), littleVector.begin(), littleVector.end());
            littleVector.erase(littleVector.begin(),littleVector.end());
            lines++;
        } else { 
            items++;
            littleVector.push_back(stod(line));
        }
    }

// -----------------------------------------------------------------------------
// Data Structure
// -----------------------------------------------------------------------------

    Extremes minmax;
    minmax.min_x = bigVector[0];
    minmax.min_y = bigVector[0];

    for (int i = 0; i <= bigVector.size(); i++) {
        Geo geo; 
        double x= bigVector[i];
        double y= bigVector[i+1];
        minmax.set_x(x);
        minmax.set_y(y);
        geo.set_xy( x,y );
        geo.v = i;
        i+=359;
        coords.push_back(geo);
    }

// -----------------------------------------------------------------------------
// Maximum and minimum getter
// -----------------------------------------------------------------------------

    double min_vector[360];
    double max_vector[360];

    for (auto ii = coords.begin(); ii != coords.end(); ii++) {
        for ( int jj=0; jj < 360; jj++ ){
            if (min_vector[jj] > bigVector[(*ii).v + jj]){
                min_vector[jj] = bigVector[(*ii).v + jj];
            }
            if (max_vector[jj] < bigVector[(*ii).v + jj]){
                max_vector[jj] = bigVector[(*ii).v + jj];
            }
        }
    }

    // Get stop time
    std::chrono::high_resolution_clock::time_point stop 
        = std::chrono::high_resolution_clock::now();
    // Get duration as double
    std::chrono::duration<double> time_span 
        = std::chrono::duration_cast<std::chrono::duration<double> >
        (stop - start);

// -----------------------------------------------------------------------------
// Printer
// -----------------------------------------------------------------------------

    std::cout << "\n\n#min vector: ";
    for (int mm = 0; mm < 360; mm++) {
        std::cout << mm << min_vector[mm] << " ";
    }
    std::cout << "\n\n#max vector: ";
    for (int mm = 0; mm < 359; mm++) {
        std::cout << " (" << mm << ")" << max_vector[mm] << " ";
    }
    std::cout << "\n\n" << items << " items parsed from " << lines << " lines \n";
    std::cout << "Max: {x:" << minmax.max_x << ",y:" << minmax.max_y << "}\n";
    std::cout << "Min: {x:" << minmax.min_x << ",y:" << minmax.min_y << "}\n";

    std::cout << "Function duration: " << time_span.count() 
              << " seconds." << std::endl;

    return 1;
}

// -----------------------------------------------------------------------------
// Line splitter
// -----------------------------------------------------------------------------

int split_line(std::string &line, std::string delim, std::vector<double> &littleVector){

    int count =0;
    size_t pos = 0;
    while ((pos = line.find(delim, (pos + 1))) != std::string::npos) {
        count++;
        std::string p = line.substr(0, pos);
        double f = std::stof(p);
        littleVector.push_back(f);
        line = line.substr(pos + 1);
    }
    if (!line.empty()) {
        count++;
        double f = stod(line);
        littleVector.push_back(f);
    }

    return count;
}

// -----------------------------------------------------------------------------
// Max & Min utilities
// -----------------------------------------------------------------------------

double min_val(const std::vector<double> &littleVector, std::vector<double> &min){
    double x = *std::min_element( littleVector.begin(), littleVector.end() );
    min.push_back(x);
    return x;
}

double max_val(const std::vector<double> &littleVector, std::vector<double> &max){
    double x = *std::max_element( littleVector.begin(), littleVector.end() );
    max.push_back(x);
    return x;
}

void min_max_vector(std::vector<double> &vec, std::vector<double> &min, std::vector<double> &max){
    for(size_t i = 0; i < vec.size(); ++i){
        if (vec[i]>max[i]){ max[i] = vec[i]; } else 
        if (vec[i]<min[i]){ min[i] = vec[i]; }
    }
}

/*
1K File:
Min X: 34.4656 Min Y: 68.3321
Max X: 34.4681 Max Y: 68.3674

1M File:
Min X: 34.4014 Min Y: 68.2254
Max X: 34.5434 Max Y: 68.4029
*/




