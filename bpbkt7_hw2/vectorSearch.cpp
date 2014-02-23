#include "pfv.hpp"
#include "vectorSearch.hpp"
#include <limits>

bool operator< (SearchVectorDataPoint & a, SearchVectorDataPoint & b){
   return (a.dist < b.dist);
}

vectorSearch::SearchVectorDataPoint(){};

vectorSearch::SearchVectorDataPoint(const float * subvector){
   dist = std::numeric_limits<float>::max();
   x = subvector[0];
   y = subvector[1];
   v = &subvector[2];
   offset = 0;
}

void vectorSearch::findMinDist(const vector<float> & key){
   for(size_t t_offset = 0; t_offset < 360; t_offset += 5){
      float temp = 0;
      for(size_t i = 0; i < key.size(); ++i){
         temp += abs(v[((t_offset+i)%360)] - key[i]);
         if(temp >= dist) break;
      }
      if(temp < dist){
         offset = t_offset;
         dist = temp;
      }
   }
}

vectorSearch::SearchVectorData(short short_list_size){
   size = short_list_size;
};

void vectorSearch::hold(const float * subvector){
   pool.push_back(SearchVectorDataPoint(subvector));
}

void vectorSearch::search(vector<float> &key){
   for(auto i = pool.begin(); i != pool.end(); ++i){
      (*i).findMinDist(key);
      insert(*i);
   }
}

void vectorSearch::insert(SearchVectorDataPoint & new_dp){
   if(short_list.size() == 0){
         short_list.push_front(new_dp);
         return;
   }
   auto it = short_list.begin();
   while(it != short_list.end()){
      if(new_dp.dist >= it->dist){
         break;
      }
      ++it;
   }
   short_list.insert(it, new_dp);
   if(short_list.size() > size){
      //cout << "pop!\n";
      short_list.pop_front();
      return;
   }
}