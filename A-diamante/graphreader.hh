#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

void readGraph(string fileName, vector <int> &vectorData, vector<int> &vectorCols, vector<int> &vectorRows){
  ifstream infile(fileName);
  string line;
  int currentSource = -1, sizeVector = 0;

  while (getline(infile, line)) {
    istringstream iss(line);

    if (line[0] == 'p') {
      string s1, s2;
      int nodes;

      iss >> s1 >> s2 >> nodes;

    } else if (line[0] == 'e'){
      char e;
      int source, target, weight;

      iss >> e >> source >> target >> weight;
      vectorData.push_back(weight);
      vectorCols.push_back(target-1);

      if(source != currentSource) {
        currentSource = source;
        sizeVector = vectorData.size();
        vectorRows.push_back(sizeVector-1);
      }      
    }
  }
}
