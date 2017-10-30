#include "graphreaderMap.hh"
#include "stats.hh"
#include "timer.hh"
#include <cassert>
#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

void ad0(vector<map<int, int>> &Mat, vector<map<int, int>> &MatResult, int sizeMat) {
  
  for (int i = 0; i < Mat.size(); i++) {
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
        }

        if(m1->first > m2->first)
          ++m2;
        else 
          ++m1;        
      }
    }
  }
}

void printMat(vector<map<int, int>> &Mat) {

  for (int i = 0; i < Mat.size(); i++) {
    for (auto j = Mat[i].begin(); j != Mat[i].end(); ++j){
        cout << i <<","<< j->first <<" : "<<  j->second << "  " ;
      }
    cout<<endl;
    }
}

void benchmark(int times, const string &fileName) {
  vector<map<int, int>> Mat;
  vector<map<int, int>> MatResult;
  int sizeMat;
  readGraph(fileName, Mat, MatResult, sizeMat);
  ad0(Mat,MatResult,sizeMat);
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
