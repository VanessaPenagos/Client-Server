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

void minData(vec &Mat, vec &MatResult, int i){
	for (int j = 0; j < Mat.size() ; j++) {
	auto m1 = Mat[i].begin();
	auto m2 = Mat[j].begin();
	while(m1 != Mat[i].end() && m2 != Mat[j].end()) {
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

void a_diamant(vec &Mat, vec &MatResult) {
	thread_pool pool;
  for (int i = 0; i < Mat.size(); i++) {
		pool.submit([&Mat,&MatResult,i](){ minData(Mat,MatResult,i);});
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

void benchmark(int times, const string &fileName) {
  vec Mat;
  vec MatResult;
  readGraph(fileName, Mat, MatResult);
  a_diamant(Mat,MatResult);
  // cout << "Ready! " << endl;

   cout << "Mat" << endl;
   printMat(Mat);

   cout << "MatResult" << endl;
   printMat(MatResult);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "Error!!" << endl;
  }
  string fileName(argv[1]);
  benchmark(1, fileName);
  return 0;
}
