#include <vector>
#include <ctime>
#include <cstdlib>
#include "Coordz.h"
#include "Problem.h"
#include "Solution.h"

#define ROWS 75
#define COLS 300
#define LAYERS 8
#define TURNS 400

#ifndef DYN_H
#define DYN_H

class DynamicProgramming {
  void fillArray ();
public:
  int * scores;
  Coordz * from;
  Solution sol;
  Problem prob;
  int bestScore;

  DynamicProgramming (Problem prob, Solution sol);
  DynamicProgramming ();
  ~DynamicProgramming();

  void reinitArray ();
  //void addLoon (int idx, int maxStart);
  void addLoonIter (int idx);
  int getBest (int nbTurns, Coordz & currentTile);
};

#endif
