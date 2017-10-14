#include "graphreader.hh"
#include "stats.hh"
#include "timer.hh"
#include <cassert>
#include <iostream>
#include <string>

using namespace std;
using Mat = vector<vector<int>>;

void ad0(const Mat &m1, const Mat &m2, Mat &res) {
  int i = m1.size();    // number of rows in m1
  int j = m1[0].size(); // number of cols in m1
  int k = m2.size();    // number of rows in m2
  int l = m2[0].size(); // number of cols in m2

  assert(j == k);

  for (int a = 0; a < i; a++)
    for (int b = 0; b < l; b++)
      for (int c = 0; c < j; c++) {
        if (m1[a][c] != numeric_limits<int>::max() &&
            m2[c][b] != numeric_limits<int>::max())
          res[a][b] = min(res[a][b], m1[a][c] + m2[c][b]);
      }
}

void printMat(const vector <int> &vectorData, const vector<int> &vectorCols, const vector<int> &vectorRows) {
 
  for(int i= 0; i < vectorData.size(); i++) {
    cout << vectorData[i] << " ";
  }
  for(int i= 0; i < vectorData.size(); i++) {
    cout << vectorCols[i] << " ";
  }

  for(int i= 0; i < vectorRows.size(); i++) {
    cout << vectorRows[i] << " ";
  }
}

void benchmark(int times, const string &fileName) {
  vector<int> vectorData, vectorData_r;
  vector<int> vectorCols, vectorCols_r;
  vector<int> vectorRows; vectorCols_r;

  readGraph(fileName, vectorData,vectorCols,vectorRows);
  //ad0();

 printMat(vectorData, vectorCols, vectorRows);



/*  Mat r;
  r.resize(g.size());
  for (int i = 0; i < g.size(); i++)
    r[i].resize(g.size(), numeric_limits<int>::max());
  ad0(g, g, r);
  printMat(r);
*/
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