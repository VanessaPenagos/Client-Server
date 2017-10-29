#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <map>
#include <vector>

using namespace std;

void readGraph(string fileName, vector<map<int, int>> &Mat, int &sizeMat){
  ifstream infile(fileName);
  string line;
  int current_source = -1;

  while (getline(infile, line)) {
    istringstream iss(line);

    if (line[0] == 'p') {
      string s1, s2;

      iss >> s1 >> s2 >> sizeMat;

    } else if (line[0] == 'e'){
      char e;
      int source, target, weight;

      iss >> e >> source >> target >> weight;
      map<int, int> col;

      if (source != current_source){
        Mat.push_back(col);
        current_source = source;
      }
      
      if(target != source){
        col[target-1] = weight; 
        cout << "Holi";
      }
      else{
        col[target-1] = 0; 
        cout << "Chau";
      }
    }
  }
}
