#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <utility>

using namespace std;

void readGraph(string fileName, unordered_map<string,int> &Mat, int &sizeMat){
  ifstream infile(fileName);
  string line;

  while (getline(infile, line)) {
    istringstream iss(line);

    if (line[0] == 'p') {
      string s1, s2;

      iss >> s1 >> s2 >> sizeMat;

    } else if (line[0] == 'e'){
      char e;
      int source, target, weight;

      iss >> e >> source >> target >> weight;
      string key = to_string(source-1)+","+to_string(target-1);
      Mat[key] = weight;
    }
  }
}
