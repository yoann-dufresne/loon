#include "Problem.h"



Winds::Winds (int rows, int cols, int layers) {
  this->directions.resize(rows, vector<vector<Coord> >(cols, vector<Coord>(layers, Coord(0,0))));
}
Winds::Winds() {}



Problem::Problem (int rows, int cols, int layers) {
  this->rows = rows;
  this->cols = cols;
  this->layers = layers-1;

  this->nextTarget = 0;
  this->targetIdx = vector<vector<int> > (rows, vector<int>(cols, -1));
  this->winds = Winds(rows, cols, layers);

  reachableTargets = vector<vector<vector<int> > > (rows, vector<vector<int> >(cols, vector<int>(0)));
}

Problem::Problem () {}

Coord Problem::getWindDirection (int row, int col, int layer) {
  return this->winds.directions[row][col][layer];
}

void Problem::setWindDirection (int row, int col, int layer, Coord c) {
  if (row + c.x < 0 || row + c.x >= this->rows)
    this->winds.directions[row][col][layer].x = -1;
  else
    this->winds.directions[row][col][layer].x = row + c.x;
  
  this->winds.directions[row][col][layer].y = (col + c.y + this->cols) % this->cols;
}

void Problem::setTarget (Coord c) {
  // Set the target
  this->targetIdx[c.x][c.y] = this->nextTarget;
  this->targets.push_back(c);

  // Set up the target in reachable tiles
  int row = c.x;
  int col = c.y;
  for (int x=row-this->radius ; x<=row+this->radius ; x++) {
    if (x < 0 || x >= this->rows)
      continue;

    for (int j=col-this->radius ; j<=col+this->radius ; j++) {
      int y = (j + this->cols) % this->cols;

      int colDist = min(abs(col-y), this->cols-abs(col-y));        
      int dist = (row-x) * (row-x) + colDist * colDist;

      if (dist > this->radius * this->radius)
        continue;

      this->reachableTargets[x][y].push_back(this->nextTarget);
    }
  }

  this->nextTarget += 1;
}

Coord Problem::getNextTile(int x, int y, int z) {
  return this->winds.directions[x][y][z];
}

