#include <vector>
#include <cstdlib>
#include <cmath>
#include "Coordz.h"

#ifndef PROBLEM_H
#define PROBLEM_H

using namespace std;

class Winds {
public:
  vector<vector<vector<Coord> > >directions;

  Winds (int rows, int cols, int layers);
  Winds();
};

class Problem {
public:
  int rows;
  int cols;
  int layers;
  
  Winds winds;
  int nbTargets;
  
  int nextTarget;
  vector<Coord> targets;
  vector<vector<int> > targetIdx;
  
  int radius;
  int nbLoons;
  int nbTurns;

  Coord start;

  vector<vector<vector<int> > > reachableTargets;

  Problem (int rows, int cols, int layers);
  Problem ();

  Coord getWindDirection (int row, int col, int layer);
  void setWindDirection (int row, int col, int layer, Coord c);
  void setTarget (Coord c);
};

#endif