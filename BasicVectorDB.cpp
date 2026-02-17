#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <random>
#include <math.h>

using namespace std;

const int EMBEDDING_DIMENSION = 300;
int globalIndex = 0;



vector<float> getNormalizedVector(vector<float>e){
  float deno = 0;
  for(auto a:e){
    deno+= a*a;
  }
  
  
  // null vector
  if(deno==0){
    return e;
  }
  
  
  deno = pow(deno,0.5);
  
  for(int i=0;i<e.size();i++){
    e[i]/=deno;
  }
  return e;
}

// O(EMBEDDING_DIMENSION)
float computeVectorSimilarity(vector<float>e1, vector<float>e2){
  e1 = getNormalizedVector(e1);
  e2 = getNormalizedVector(e2);
  
  float ans = 0.0;
  for(int i=0;i<e1.size();i++){
    ans+=(e1[i]*e2[i]);
  }
  return ans;
}


class VectorData{
  public:
    int index;
    vector<float>embedding;
    string text;
    
    VectorData(){
      index = globalIndex++;
      embedding = vector<float>(EMBEDDING_DIMENSION, 0);
      text = "";
    }
    
    VectorData(string text, vector<float>embedding){
      index = globalIndex++;
      this->text = text;
      this->embedding = embedding;
    }
    
    void print(){
      cout<<"index"<<this->index<<endl;
      cout<<"text"<<this->text<<endl;
      cout<<"embedding";
      for(int i=0;i<10;i++){
        cout<<this->embedding[i]<<" ";
      }
      cout<<".....";
      cout<<endl;
    }
    
};

//create index on embedding
class VectorDB{
  public:
  int size;
  vector<VectorData> storage;
  
  // O(1)
  bool insertData(VectorData d){
    storage.push_back(d);
    size++;
    return true;
  }
  
  // O(n)
  bool deleteData(int index){
    for(int i=0;i<storage.size();i++){
      if(storage[i].index == index){
        storage.erase(storage.begin() + i);
      }
    }
    size--;
    return true;
  }
  
  // O(EMBEDDING_DIMENSION*n + nlogn)
  vector<pair<float, int>> getSimilarK(vector<float> emb, int k){
    
    // stores (score,index)
    vector<pair<float,int>>similarityscores;
    for(int i=0;i<storage.size();i++){
      similarityscores.push_back({computeVectorSimilarity(emb, storage[i].embedding), i});
    }
    
    sort(similarityscores.begin(),similarityscores.end(), greater<pair<float,int>>());
    
    for(int i=0;i<k;i++){
      cout<<"score "<<similarityscores[i].first<<"for index"<< similarityscores[i].second <<endl;
    }
    
    return vector<pair<float,int>>(similarityscores.begin(),similarityscores.begin()+k);
  }
  
};


// test functions 
vector<float> generateRandomEmbedding(){
  vector<float>e(EMBEDDING_DIMENSION);
  for(int i=0;i<EMBEDDING_DIMENSION;i++){
    e[i] = (static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
  }
  
  return e;
}


int main() 
{
  VectorDB db;
  
  for(int i=0;i<50;i++){
    VectorData d("somerandomtext", generateRandomEmbedding());
    db.insertData(d);
  }
  
  db.getSimilarK(generateRandomEmbedding(), 10);
  
  cout<< db.size;
  
}
