#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include "Coordz.h"
#include "Problem.h"
#include "Solution.h"


#ifndef DYN_H
#define DYN_H

class DynamicProgramming {
  void fillArray ();
  vector<int> backtrack ();
public:
  int * scores;
  Coordz * from;
  Solution sol;
  Problem prob;
  int bestScore;
  Coordz bestTile;
  int bestTime;

  DynamicProgramming (Problem prob, Solution sol);
  DynamicProgramming ();
  ~DynamicProgramming();

  void print ();
  void reinitArray ();
  //void addLoon (int idx, int maxStart);
  void addLoonIter (int idx);
  //int getBest (int nbTurns, Coordz & currentTile);
};

#endif
