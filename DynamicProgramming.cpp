#include "DynamicProgramming.h"


//#define IDX(t, x, y, z) ((t)*ROWS*COLS*(LAYERS) + (x)*COLS*(LAYERS) + (y)*(LAYERS) + (z))
#define IDX(t, x, y, z) ((t)*ROWS*COLS*LAYERS + (x)*COLS*LAYERS + (y)*LAYERS + (z)-1)


DynamicProgramming::DynamicProgramming () {};

DynamicProgramming::DynamicProgramming (Problem prob, Solution sol) {
  this->sol = sol;
  this->prob = prob;

  scores = (int *) malloc ((long)(TURNS+1) * (long)COLS * (long)ROWS * (long)LAYERS * sizeof(int));
}

DynamicProgramming::~DynamicProgramming() {};

void DynamicProgramming::reinitArray () {
  for (int t=0 ; t<=TURNS ; t++)
    for (int row=0 ; row<ROWS ; row++)
      for (int col=0 ; col<COLS ; col++)
        for (int layer=1 ; layer<=LAYERS ; layer++) {
          scores[IDX(t,row,col,layer)] = -1;
        }
}

/*int MAX;
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
}*/


void DynamicProgramming::fillArray () {
  //srand(time(0));

  Coord & first = this->prob.getNextTile (this->prob.start.x, this->prob.start.y, 1);
  int best = 0;

  //                                       /!!!/
  for (int currentTurn=0 ; currentTurn<TURNS-1 ; currentTurn++ ) {
    int firstScore = scores[IDX(currentTurn, first.x, first.y, 1)];
    if (firstScore == -1)
      scores[IDX(currentTurn, first.x, first.y, 1)] = this->sol.scoreByTile[currentTurn][first.x][first.y];
    int nbCalc = 0;

    for (int r=0 ; r<ROWS ; r++) {
      for (int c=0 ; c<COLS ; c++) {
        for (int l=1 ; l<=LAYERS ; l++) {
          // Si il existe un moyen d'arriver sur cette case, alors il faut continuer
          int currentScore = scores[IDX(currentTurn, r, c, l)];
          if (currentScore != -1) {
            nbCalc++;
            // Toutes les possibilités de changement d'altitude
            for (int dz=-1 ; dz<=1 ; dz++) {
              // Cas critiques
              if (l + dz <= 0 || l + dz > LAYERS)
                continue;

              Coord & next = this->prob.getNextTile (r, c, l+dz);
              // Hors limite en x
              if (next.x < 0 || next.x >= this->prob.rows) {
                continue;
              }

              // Calcule le score et l'ajoute si meilleur
              int score = currentScore + this->sol.scoreByTile[currentTurn+1][next.x][next.y];
              if (score > scores[IDX(currentTurn+1, next.x, next.y, l+dz)])
                scores[IDX(currentTurn+1, next.x, next.y, l+dz)] = score;

              if (score > best)
                best = score;
            }
          }
        }
      }
    }

    cout << nbCalc << endl;
  }

  cout << best << endl;
}

void DynamicProgramming::addLoonIter (int idx) {
  this->reinitArray();

  fillArray();
}

/*int DynamicProgramming::getBest (int nbTurns, Coordz & currentTile) {
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
}*/









