#include "graphreaderMap.hh"
#include "stats.hh"
#include "timer.hh"
#include <cassert>
#include <iostream>
#include <string>
#include <map>
#include <vector>

using namespace std;

void ad0(map <pair<int,int>,int> &Mat, map <pair<int,int>,int> &MatResult, int sizeMat) {

  pair<int,int> indexR;
  pair<int,int> indexA;
  pair<int,int> indexB;

  int current_i = 0, cols = 0;
  int count_i = 0;
  
  for (int a = 0; a < sizeMat; a++) {
    for (int b = 0; b < sizeMat; b++) {
      for (int c = 0; c < sizeMat; c++) {
        indexA = make_pair(a,c);
        indexB = make_pair(c,b);
        indexR = make_pair(a,b);
        if (MatResult.count(indexR) > 0 && Mat.count(indexA) > 0 && Mat.count(indexB) > 0)
          MatResult[indexR] = min(MatResult[indexR], (Mat[indexA] + Mat[indexB]));
        else if(Mat.count(indexA) > 0 && Mat.count(indexB) > 0)
          MatResult[indexR] = Mat[indexA] + Mat[indexB];
      }
    }
  }
}

void printMat(vector<map<int, int>> &Mat) {

  for (int i = 0; i < Mat.size(); ++i){
    cout << i << endl; 
    for (const auto& p : Mat[i]){
      cout << "Esto dentro de :::: " << endl;
    }
  }
}

void benchmark(int times, const string &fileName) {
  vector<map<int, int>> Mat;
  vector<map<int, int>> MatResult;
  int sizeMat;
  readGraph(fileName, Mat, sizeMat);
  //ad0(Mat,MatResult,sizeMat);
  cout << "Ready! " << endl;
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
