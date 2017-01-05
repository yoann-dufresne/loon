#include <vector>
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
public:
  int scores[TURNS][ROWS][COLS][LAYERS+1];
  Coordz from[TURNS][ROWS][COLS][LAYERS+1];
  Solution sol;
  Problem prob;
  int bestScore;

  DynamicProgramming (Problem prob, Solution sol);
  DynamicProgramming ();
  ~DynamicProgramming();

  void reinitArray ();
  int floorScore (unordered_set<Coordz, my_coordz_hash> elems, int minScore, int maxMinScore,
                 int nbElements, int epsilon, int t);
  void addLoon (int idx, int maxStart);
  int getBest (int nbTurns, Coordz & currentTile);
};

#endif
