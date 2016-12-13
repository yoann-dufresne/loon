#include <vector>
#include "Problem.h"


#ifndef SOL_H
#define SOL_H

using namespace std;

class Solution {
public:
  vector<vector<int> > coverage;
  vector<vector<int> > loons;
  Problem problem;

  Solution (Problem prob);
  Solution ();

  void load (string filename);
  void save (string filename);
  void randomInit ();
  void initScore ();
  int getScore ();
};

#endif
