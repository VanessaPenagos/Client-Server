#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

using namespace std;
using vec = vector<map<int, int>>;

void readGraph(string fileName, vec &Mat, vec &MatResult){
  ifstream infile(fileName);
  string line;
  int sizeMat;

  while (getline(infile, line)) {
    istringstream iss(line);

    if (line[0] == 'p') {
      string s1, s2;

      iss >> s1 >> s2 >> sizeMat;

      for (int i = 0; i < sizeMat; ++i){
        map<int, int> destiny;
        Mat.push_back(destiny);
        MatResult.push_back(destiny);   
      }

    } else if (line[0] == 'e'){
      char e;
      int source, target, weight;

      iss >> e >> source >> target >> weight;
      if(target != source){
        Mat[source-1][target-1] = weight;       
      }
      else{
        Mat[source-1][target-1] = 0;
      }
    }
  }
}
