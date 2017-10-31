#include "graphreaderMap.hh"
#include "stats.hh"
#include "timer.hh"
#include "threadPool.hh"
#include <cassert>
#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;
using vec = vector<map<int, int>>;

void minData(vec &Mat1,vec &Mat2, vec &MatResult, int i){
	for (int j = 0; j < Mat2.size() ; j++) {
	auto m1 = Mat1[i].begin();
	auto m2 = Mat2[j].begin();
	while(m1 != Mat1[i].end() && m2 != Mat2[j].end()) {
	  if(m1->first == m2->first) {
	    if (MatResult[i].find(j) != MatResult[i].end()) {
	      MatResult[i][j] = min(MatResult[i][j], m1->second + m2->second);
	    } else {
	      MatResult[i][j] = m1->second + m2->second;
	    }
	    ++m1;
	    ++m2; 
	  }else if(m1->first > m2->first)
	    ++m2;
	  else 
	    ++m1;        
		}
	}
}

void mult(vec &Mat1,vec &Mat2, vec &MatResult) {
	thread_pool pool;
  for (int i = 0; i < Mat1.size(); i++) {
		pool.submit([&Mat1,&Mat2,&MatResult,i](){ minData(Mat1,Mat2,MatResult,i);});
  }
}

void printMat(vec &Mat) {

  for (int i = 0; i < Mat.size(); i++) {
    for (auto j = Mat[i].begin(); j != Mat[i].end(); ++j){
        cout << i <<","<< j->first <<" : "<<  j->second << "  " ;
      }
    cout<<endl;
    }
}

vec copyMat(vec &Mat){
  vec MatCopy;
  size_t MatSize = Mat.size();
  MatCopy.resize(MatSize);
  for(int i = 0 ; i<MatSize; i++){
    MatCopy[i] = Mat[i];
  }
  return MatCopy;
}

void buildMap(vec& MatResult){
  int sizeMat = MatResult.size();
  for(int i = 0; i < sizeMat; ++i){
    map<int, int> destiny;
    MatResult[i] = destiny;
  }
}

void merge(vec& dest, vec& source){
  for(int i = 0 ; i<source.size(); i++) dest[i] = source[i];
}

void Adiamond(vec& Mat, int n){
  vec MatResult;
  MatResult.resize(Mat.size());
  buildMap(MatResult);
  if(n == 1) return;
  if(n % 2 == 0){ // even case
    mult(Mat,Mat,MatResult);
    merge(Mat,MatResult);
    Adiamond(Mat,n/2);
  }
  else{ // odd case
    vec copiedMat = copyMat(Mat);
    mult(Mat,Mat,MatResult);
    merge(Mat,MatResult);
    MatResult.clear();
    MatResult.resize(Mat.size());
    buildMap(MatResult);
    Adiamond(Mat,(n-1)/2);
    mult(Mat,copiedMat,MatResult);
    merge(Mat,MatResult);
  }
}

void benchmark(int times, const string &fileName) {
  vec Mat;
  vec MatResult;
  readGraph(fileName, Mat, MatResult);
  Timer timer("a-diamantMap.cpp");
  Adiamond(Mat,Mat.size());
  long _time = timer.elapsed();
  //cout << _time/1000.0;
   printMat(Mat);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "Error!!" << endl;
  }
  string fileName(argv[1]);
  benchmark(1, fileName);
  return 0;
}
