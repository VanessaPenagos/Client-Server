#include "graphreaderMap.hh"
#include "stats.hh"
#include "timer.hh"
#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;
using Mat = vector<vector<int>>;

void ad0(unordered_map <string,int> &Mat, unordered_map <string,int> &MatResult, int sizeMat) {

  for (int a = 0; a < sizeMat; a++)
    for (int b = 0; b < sizeMat; b++)
      for (int c = 0; c < sizeMat; c++){
        string indexA = to_string(a)+","+to_string(c);
        string indexB = to_string(c)+","+to_string(b);
        string indexR = to_string(a)+","+to_string(b);
        //cout << Mat["6,4"];
        MatResult[indexR] = min(MatResult[indexR], (Mat[indexA] + Mat[indexB]));
      }
}

void printMat(unordered_map <string,int> &Mat) {
   for(const auto& p : Mat)
        cout << p.first << endl;
}

void benchmark(int times, const string &fileName) {
  unordered_map <string,int> Mat;
  unordered_map <string,int> MatResult;
  int sizeMat;
  readGraph(fileName, Mat, sizeMat);
  //ad0();
  //printMat(Mat);
  
}

int main(int argc, char **argv) {
  if (argc != 2) {
    cerr << "Error!!" << endl;
  }
  string fileName(argv[1]);
  benchmark(1, fileName);  
  cout <<"Final final no va mas " << endl;
  return 0;
}
