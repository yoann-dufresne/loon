#include <vector>
#include <cstdlib>
#include <iostream>
#include <cmath>
#include "Coordz.h"

// #define ROWS 3
// #define COLS 5
// #define LAYERS 2
// #define TURNS 3

#define ROWS 75
#define COLS 300
#define LAYERS 8
#define TURNS 400


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
  vector<vector<vector<vector<Coordz> > > > origins;

  Problem (int rows, int cols, int layers);
  Problem ();

  Coord getWindDirection (int row, int col, int layer);
  void setWindDirection (int row, int col, int layer, Coord c);
  void setTarget (Coord c);
  Coord & getNextTile(int x, int y, int z);
};

#endif