#include "graphreaderMap.hh"
#include "stats.hh"
#include "timer.hh"
#include <cassert>
#include <iostream>
#include <string>
#include <map>

using namespace std;

void ad0(map <pair<int,int>,int> &Mat, map <pair<int,int>,int> &MatResult, int sizeMat) {

  pair<int,int> indexR;
  pair<int,int> indexA;
  pair<int,int> indexB;

  for (int a = 0; a < sizeMat; a++){
    for (int b = 0; b < sizeMat; b++){
      for (int c = 0; c < sizeMat; c++){
        indexA = make_pair(a,c);
        indexB = make_pair(c,b);
        indexR = make_pair(a,b);
        MatResult[indexR] = min(MatResult[indexR], (Mat[indexA] + Mat[indexB]));
      }
    }
    cout << "Hola mundo " << endl;
  }
}

void printMat(map <pair<int,int>,int> &Mat) {
    pair<int,int> key;
    for(const auto& p : Mat){
        key = make_pair(p.first.first, p.first.second);
        int value = Mat[key];
        cout << p.first.first <<","<< p.first.second << " : " << value << endl;
    }
}

void benchmark(int times, const string &fileName) {
  map <pair<int,int>, int> Mat;
  map <pair<int,int>, int> MatResult;
  int sizeMat;
  readGraph(fileName, Mat, sizeMat);
  printMat(Mat);
  ad0(Mat,MatResult,sizeMat);
}

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "Error!!" << endl;
  }
  string fileName(argv[1]);
  benchmark(1, fileName);  
  return 0;
}
