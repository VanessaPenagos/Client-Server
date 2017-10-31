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
      Mat.resize(sizeMat);
      MatResult.resize(sizeMat);

      for (int i = 0; i < sizeMat; ++i){
        map<int, int> destiny;
        Mat[i] = destiny;
        MatResult[i] = destiny;
      }

    } else if (line[0] == 'e' || line[0] == 'a'){
      char e;
      int source, target, weight=1;

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
