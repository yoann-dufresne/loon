#include <vector>
#include "Coordz.h"
#include "Problem.h"
#include "Solution.h"


#ifndef DYN_H
#define DYN_H

class DynamicProgramming {
public:
  int **** scores;
  Coordz **** from;
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
  int getBest (int nbTurns, int x, int y, int z);
};

#endif
