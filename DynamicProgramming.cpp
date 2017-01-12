#include "DynamicProgramming.h"


//#define IDX(t, x, y, z) ((t)*ROWS*COLS*(LAYERS) + (x)*COLS*(LAYERS) + (y)*(LAYERS) + (z))
#define IDX(t, x, y, z) ((t)*ROWS*COLS*LAYERS + (x)*COLS*LAYERS + (y)*LAYERS + (z)-1)


DynamicProgramming::DynamicProgramming () {};

DynamicProgramming::DynamicProgramming (Problem prob, Solution sol) {
  this->sol = sol;
  this->prob = prob;

  scores = (int *) malloc ((long)(TURNS) * (long)COLS * (long)ROWS * (long)LAYERS * sizeof(int));
}

DynamicProgramming::~DynamicProgramming() {};

void DynamicProgramming::reinitArray () {
  for (int t=0 ; t<TURNS ; t++)
    for (int row=0 ; row<ROWS ; row++)
      for (int col=0 ; col<COLS ; col++)
        for (int layer=1 ; layer<=LAYERS ; layer++) {
          scores[IDX(t,row,col,layer)] = -1;
        }
}


void DynamicProgramming::print () {
  for (int t=0 ; t<TURNS ; t++) {
    for (int l=1 ; l<=LAYERS ; l++) {
      for (int r=0 ; r<ROWS ; r++) {
        for (int c=0 ; c<COLS ; c++) {
          cout << scores[IDX(t, r, c, l)] << " ";
        }

        cout << endl;
      }
    }

    cout << endl;
  }
}


void DynamicProgramming::fillArray () {
  //srand(time(0));

  Coord & first = this->prob.getNextTile (this->prob.start.x, this->prob.start.y, 1);
  int best = 0;
  int max = TURNS;

  // Init
  scores[IDX(0, first.x, first.y, 1)] = this->sol.scoreByTile[0][first.x][first.y];
  best = this->sol.scoreByTile[0][first.x][first.y];
  this->bestTile = Coordz(first.x, first.y, 1);
  this->bestTime = 0;

  // Recur
  for (int currentTurn=0 ; currentTurn<TURNS-1 ; currentTurn++ ) {
    // Décolage tardif
    int firstScore = scores[IDX(currentTurn+1, first.x, first.y, 1)];
    if (firstScore == -1) {
      firstScore = this->sol.scoreByTile[currentTurn+1][first.x][first.y];
      scores[IDX(currentTurn+1, first.x, first.y, 1)] = firstScore;

      if (firstScore > best) {
        best = firstScore;
        this->bestTile = Coordz(first.x, first.y, 1);
        this->bestTime = currentTurn+1;
      }
    }

    // Chemin classique
    for (int r=0 ; r<ROWS ; r++) {
      for (int c=0 ; c<COLS ; c++) {
        for (int l=1 ; l<=LAYERS ; l++) {
          // Si il existe un moyen d'arriver sur cette case, alors il faut continuer
          int currentScore = scores[IDX(currentTurn, r, c, l)];
          if (currentScore != -1) {
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

              if (score > best) {
                best = score;
                this->bestTile = Coordz(next.x, next.y, l+dz);
                this->bestTime = currentTurn+1;
              }
            }
          }
        }
      }
    }
  }
  
}


vector<int> DynamicProgramming::backtrack () {
  srand(21);//time(0));
  vector<int> path ((TURNS-1 - this->bestTime), 0);
  
  // Init première solution
  vector<Coordz> candidates;
  candidates.push_back(this->bestTile);

  Coord & first = this->prob.getNextTile (this->prob.start.x, this->prob.start.y, 1);

  // Init score
  int score = scores[IDX(this->bestTime, this->bestTile.x, this->bestTile.y, this->bestTile.z)];
  Coordz previous = this->bestTile;
  Coordz current = previous;

  int t=this->bestTime;
  for ( ; t>0 ; t--) {
    // Calcul du score a atteindre
    int wantedScore = score - this->sol.scoreByTile[t][current.x][current.y];

    // Recherche de toutes les cases précédentes correspondant au score recherché
    candidates.clear();
    for (Coordz & origin : this->prob.origins[current.x][current.y][current.z]) {
      int originScore = scores[IDX(t-1, origin.x, origin.y, origin.z)];

      // Si le score est le bon, ajouter à la liste des candidats pour l'étape suivante du traceback
      if (wantedScore == originScore) {
        candidates.push_back(origin);
      }
    }

    // Sélection au hasard d'un prédécésseur
    if (candidates.size() == 1)
      previous = candidates[0];
    else
      previous = candidates[rand() % candidates.size()];


    // Ajout du dz au chemin
    path.push_back(current.z - previous.z);
    

    // Si la première case est atteinte on sort
    int firstScore = this->sol.scoreByTile[t-1][first.x][first.y];
    if (wantedScore == firstScore && first.x == previous.x && first.y == previous.y && previous.z == 1) {
      break;
    }

    score = wantedScore;
    current = previous;
  }

  // Ajoute le décollage
  path.push_back(1);
  // Ajoute toutes les attentes au sol
  for (int i=1 ; i<t ; i++) {
    path.push_back(0);
  }

  // Retourne le vecteur dans le bon sens
  reverse(path.begin(), path.end());

  return path;
}

void DynamicProgramming::addLoonIter (int idx) {
  this->reinitArray();

  this->fillArray();
  vector<int> path = this->backtrack();

  // for (int val : path)
  //   cout << val << " ";
  // cout << endl;

  this->sol.addLoon(idx, path);

  //cout << "Time " << this->bestTime << endl;
}









