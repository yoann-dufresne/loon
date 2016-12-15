#include "DynamicProgramming.h"


DynamicProgramming::DynamicProgramming (Problem prob, Solution sol) {
  this->sol = sol;
  this->prob = prob;

  scores = (int ****) malloc (prob.nbTurns * sizeof(int ***));
  from = (Coordz ****) malloc (prob.nbTurns * sizeof(Coordz ***));

  for (int t=0 ; t<prob.nbTurns ; t++) {
    scores[t] = (int ***) malloc (prob.rows * sizeof(int **));
    from[t] = (Coordz ***) malloc (prob.rows * sizeof(Coordz **));

    for (int row=0 ; row<prob.rows ; row++) {
      scores[t][row] = (int **) malloc (prob.cols * sizeof(int *));
      from[t][row] = (Coordz **) malloc (prob.cols * sizeof(Coordz *));

      for (int col=0 ; col<prob.cols ; col++) {
        scores[t][row][col] = (int *) malloc (prob.layers * sizeof (int));
        from[t][row][col] = (Coordz *) malloc (prob.layers * sizeof (Coordz));

        for (int layer=0 ; layer<prob.layers ; layer++) {
          from[t][row][col][layer].x = -1;
        }
      }
    }
  }
}

DynamicProgramming::~DynamicProgramming() {};

void DynamicProgramming::reinitArray () {
  for (int t=0 ; t<this->prob.nbTurns ; t++)
    for (int row=0 ; row<this->prob.rows ; row++)
      for (int col=0 ; col<this->prob.cols ; col++)
        for (int layer=0 ; layer<this->prob.layers ; layer++) {
          scores[t][row][col][layer] = -1;
          from[t][row][col][layer].x = -1;
        }
}

void DynamicProgramming::addLoon (int idx) {
  this->reinitArray();

  cout << getBest (this->prob.nbTurns, this->prob.start.x, this->prob.start.y, 0) << endl;
}

int DynamicProgramming::getBest (int nbTurns, int x, int y, int z) {
  if (nbTurns == 0)
    return 0;

  // If saved return previous best
  if (this->scores[this->prob.nbTurns-nbTurns][x][y][z] != -1)
    return this->scores[this->prob.nbTurns-nbTurns][x][y][z];

  int best = 0;
  // test pour tous les z possibles
  for (int dz=-1 ; dz<=1 ; dz++) {
    if (z == 0 && dz != 1) {
      // Cas du report de décollage
    }

    // Hors limite en z
    if (z + dz <= 0 || z + dz > this->prob.layers)
      continue;

    Coord next = this->prob.getNextTile (x, y, z+dz);
    // Hors limite en x
    if (next.x < 0 || next.x >= this->prob.rows)
      continue;

    // Récursion classique
    int score = getBest (nbTurns-1, next.x, next.y, z+dz)
        + this->sol.scoreByTile[this->prob.nbTurns-nbTurns][next.x][next.y];
    if (score > best) {
      best = score;
    }
  }

  this->scores[this->prob.nbTurns-nbTurns][x][y][z] = best;
  return best;
}









