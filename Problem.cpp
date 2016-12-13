#include "Problem.h"



class Winds {
public:
  vector<vector<vector<Coord> > >directions;

  Winds (int rows, int cols, int layers) {
    this->directions.resize(rows, vector<vector<Coord> >(cols, vector<Coord>(layers, Coord(0,0))));
  }

  Winds() {}
};


class Problem {
public:

  Problem (int rows, int cols, int layers) {
    this->rows = rows;
    this->cols = cols;
    this->layers = layers-1;

    this->nextTarget = 0;
    this->targetIdx = vector<vector<int> > (rows, vector<int>(cols, -1));
    this->winds = Winds(rows, cols, layers);
  }

  Problem () {}

  Coord getWindDirection (int row, int col, int layer) {
    return this->winds.directions[row][col][layer];
  }

  void setWindDirection (int row, int col, int layer, Coord c) {
    this->winds.directions[row][col][layer].x = c.x;
    this->winds.directions[row][col][layer].y = c.y;
  }

  void setTarget (Coord c) {
    this->targetIdx[c.x][c.y] = this->nextTarget;
    this->targets.push_back(c);
    this->nextTarget += 1;
  }

  vector<int> getTargetsInRadius(int row, int col) {
    vector<int> targets;

    for (int x=row-this->radius ; x<=row+this->radius ; x++) {
      if (x < 0 || x >= this->rows)
        continue;

      for (int j=col-this->radius ; j<=col+this->radius ; j++) {
        int y = (j + this->cols) % this->cols;

        int colDist = min(abs(col-y), this->cols-abs(col-y));        
        int dist = (row-x) * (row-x) + colDist * colDist;

        if (dist > this->radius * this->radius)
          continue;

        if (this->targetIdx[x][y] != -1) {
          targets.push_back(this->targetIdx[x][y]);
        }
      }
    }

    return targets;
  }
};

