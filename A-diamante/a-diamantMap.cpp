#include "graphreaderMap.hh"
#include "stats.hh"
#include "timer.hh"
#include <cassert>
#include <iostream>
#include <string>
#include <unordered_map>

using namespace std;
using Mat = vector<vector<int>>;

string split(string s, char del){
  string data = "";
  for (int i = 0; i < int(s.size()); i++) {
    if (s[i] != del)
      data += s[i];
    else
      break;
  }
   return data;
}

void ad0(unordered_map <string,int> &Mat, unordered_map <string,int> &MatResult, int sizeMat) {

  for (int a = 0; a < Mat.size(); a++)
    for (int b = 0; b < Mat.size(); b++){
        string indexA = to_string(a)+","+to_string(c);
        string indexB = to_string(c)+","+to_string(b);
        string indexR = to_string(a)+","+to_string(b);
        MatResult[indexR] = min(MatResult[indexR], (Mat[indexA] + Mat[indexB]));
      }
}

void printMat(unordered_map <string,int> &Mat) {
   for(const auto& p : Mat)
        cout << p.first << endl;
}

void benchmark(int times, const string &fileName) {
  unordered_map <string,int> Mat[50];
  unordered_map <string,int> MatResult;
  readGraph(fileName, Mat, sizeMat);
  ad0(Mat,MatResult,sizeMat);
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
