#include "DynamicProgramming.h"


#define IDX(t, x, y, z) (t*ROWS*COLS*(LAYERS+1) + x*COLS*(LAYERS+1) + y*(LAYERS+1) + z)


DynamicProgramming::DynamicProgramming () {};

DynamicProgramming::DynamicProgramming (Problem prob, Solution sol) {
  this->sol = sol;
  this->prob = prob;

  scores = (int *) malloc (TURNS * COLS * ROWS * LAYERS * sizeof(int));
  from = (Coordz *) malloc (TURNS * COLS * ROWS * LAYERS  * sizeof(Coordz));

  /*  scores = (int ****) malloc (prob.nbTurns * sizeof(int ***));
  from = (Coordz ****) malloc (prob.nbTurns * sizeof(Coordz ***));

  for (int t=0 ; t<prob.nbTurns ; t++) {
    scores[t] = (int ***) malloc (prob.rows * sizeof(int **));
    from[t] = (Coordz ***) malloc (prob.rows * sizeof(Coordz **));

    for (int row=0 ; row<prob.rows ; row++) {
      scores[t,row] = (int **) malloc (prob.cols * sizeof(int *));
      from[t,row] = (Coordz **) malloc (prob.cols * sizeof(Coordz *));

      for (int col=0 ; col<prob.cols ; col++) {
        scores[t,row,col] = (int *) malloc ((prob.layers+1) * sizeof (int));
        from[t,row,col] = (Coordz *) malloc ((prob.layers+1) * sizeof (Coordz));

        for (int layer=0 ; layer<=prob.layers ; layer++) {
          from[IDX(t,row,col,layer)].x = -1;
        }
      }
    }
    }*/
}

DynamicProgramming::~DynamicProgramming() {};

void DynamicProgramming::reinitArray () {
  for (int t=0 ; t<this->prob.nbTurns ; t++)
    for (int row=0 ; row<this->prob.rows ; row++)
      for (int col=0 ; col<this->prob.cols ; col++)
        for (int layer=0 ; layer<=this->prob.layers ; layer++) {
          scores[IDX(t,row,col,layer)] = -1;
          from[IDX(t,row,col,layer)].x = -1;
        }
}

int MAX;
void DynamicProgramming::addLoon (int idx, int maxStart) {
  this->reinitArray();
  this->bestScore = 0;
  MAX = maxStart;

  Coordz init = Coordz(this->prob.start.x, this->prob.start.y, 0);
  getBest (this->prob.nbTurns, init);

  // Traceback
  vector<int> path (this->prob.nbTurns, 0);
  Coordz current = Coordz(this->prob.start.x, this->prob.start.y, 0);

  for (int t=0 ; t<this->prob.nbTurns ; t++) {
    Coordz previous = this->from[IDX(t,current.x,current.y,current.z)];

    if (previous.x < 0 || previous.x >= this->prob.rows) {
      break;
    }

    int dz = previous.z - current.z;
    if (abs(dz) > 1 || (previous.z < 1 && t > maxStart) || previous.z>this->prob.layers) {
      break;
    }

    path[t] = dz;

    current = previous;
  }

  this->sol.addLoon(idx, path);
}

int DynamicProgramming::getBest (int nbTurns, Coordz & currentTile) {
  if (nbTurns == 0)
    return 0;

  int currentTurn = this->prob.nbTurns-nbTurns;

  // If saved return previous best
  if (currentTile.z != 0 && this->scores[IDX(currentTurn,currentTile.x,currentTile.y,currentTile.z)] != -1)
    return this->scores[IDX(currentTurn,currentTile.x,currentTile.y,currentTile.z)];

  int best = 0;
  Coordz from;
  // test pour tous les z possibles
  for (int dz=-1 ; dz<=1 ; dz++) {
    if (currentTile.z == 0 && dz != 1) {
      if (currentTurn <= MAX) {
        int score = getBest (nbTurns-1, currentTile);
        if (score > best) {
          best = score;
          from = currentTile;
        }
      }
      continue;
    }

    // Hors limite en z
    if (currentTile.z + dz <= 0 || currentTile.z + dz > this->prob.layers)
      continue;

    Coord & next = this->prob.getNextTile (currentTile.x, currentTile.y, currentTile.z+dz);
    // Hors limite en x
    if (next.x < 0 || next.x >= this->prob.rows) {
      continue;
    }

    // Récursion classique
    Coordz coord = Coordz(next.x, next.y, currentTile.z+dz);
    int score = getBest (nbTurns-1, coord)
        + this->sol.scoreByTile[currentTurn][next.x][next.y];
    if (score > best) {
      best = score;
      from = coord;
    }
  }

  this->scores[IDX(currentTurn,currentTile.x,currentTile.y,currentTile.z)] = best;
  this->from[IDX(currentTurn,currentTile.x,currentTile.y,currentTile.z)] = from;

  // Absolute best
  if (best > this->bestScore) {
    this->bestScore = best;
  }

  return best;
}









