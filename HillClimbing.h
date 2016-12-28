#include <unordered_set>
#include <vector>
#include "Coordz.h"
#include "Problem.h"
#include "Solution.h"
#include "DynamicProgramming.h"


#ifndef HILL_H
#define HILL_H

class HillClimbing {
	void saveLoon (int idx);
	void loadLoon (int idx);

public:
  Solution sol;
  Problem prob;
  DynamicProgramming dp;

  HillClimbing (Problem prob, Solution sol);
  ~HillClimbing();

  void oneStep (int nbLoons);
};

#endif