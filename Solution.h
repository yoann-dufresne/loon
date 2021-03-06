#include <vector>
#include <iostream>
#include <fstream>
#include "Problem.h"


#ifndef SOL_H
#define SOL_H

using namespace std;

class Solution {
public:
  vector<vector<int> > coverage;
  vector<vector<int> > loons;
  vector<vector<vector<int> > > scoreByTile;
  Problem problem;

  int score;

  Solution (Problem prob);
  Solution ();

  void load (string filename);
  void save (string filename);
  void print ();
  void randomInit ();
  void addLoon (int idx, vector<int> path);
  void rmvLoon (int idx);
  void computeLocalScore ();
};

#endif
