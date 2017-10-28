#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>

using namespace std;

void readGraph(string fileName, map<pair<int,int>,int> &Mat, int &sizeMat){
  ifstream infile(fileName);
  string line;

  while (getline(infile, line)) {
    istringstream iss(line);

    if (line[0] == 'p') {
      string s1, s2;

      iss >> s1 >> s2 >> sizeMat;

    } else if (line[0] == 'a'){
      char e;
      int source, target, weight;
      pair<int,int> key;

      iss >> e >> source >> target >> weight;
      key = make_pair(source-1,target-1);
      Mat[key] = weight;
    }
  }
}
