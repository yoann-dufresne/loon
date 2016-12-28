#include "Solution.h"

Solution::Solution (Problem prob) {
  this->loons = vector<vector<int> >(prob.nbTurns, vector<int>(prob.nbLoons));
  this->problem = prob;
  // Couverture de chaque point d'intérêt à un moment donné.
  this->coverage = vector<vector<int> >(prob.nbTargets, vector<int>(prob.nbTurns, 0));
  // Score local à chaque case pour une solution donnée
  this->computeLocalScore();

  this->score = 0;
}

Solution::Solution () {}

void Solution::load (string filename) {
  ifstream file;
  file.open (filename);

  vector<vector<int> > loons (this->problem.nbLoons, vector<int>(this->problem.nbTurns, 0));

  for (int t=0 ; t<this->problem.nbTurns ; t++)
    for (int l=0 ; l<this->problem.nbLoons ; l++)
      file >> loons[l][t];

  for (int l=0 ; l<this->problem.nbLoons ; l++)
    this->addLoon(l, loons[l]);

  file.close();
}

void Solution::save (string filename) {
  ofstream file;
  file.open (filename);

  for (int t=0 ; t<this->problem.nbTurns ; t++) {
    for (int l=0 ; l<this->problem.nbLoons ; l++) {
      file << this->loons[t][l] << ' ';
    }

    file << endl;
  }
}

void Solution::randomInit () {
  srand(time(0));
  
  for (int loon=0 ; loon<this->problem.nbLoons ; loon++) {
    int alt=0;
    for (int turn=0 ; turn<this->problem.nbTurns ; turn++) {
    	int val = (rand() % 3) - 1;

    	if ((alt == 1 || alt == 0) && val == -1)
    	  val = rand() % 2;

    	if (alt == this->problem.layers && val == 1)
    	  val = (rand() % 2) - 1;

    	alt += val;
    	
    	this->loons[turn][loon] = val;
    }
  }
}

void Solution::print () {
  for (int turn=0 ; turn<this->problem.nbTurns ; turn++) {
    for (int loon=0 ; loon<this->problem.nbLoons ; loon++) {
      cout << this->loons[turn][loon] << ' ';
    }
    cout << endl;
  }
}

void Solution::computeLocalScore () {
  this->scoreByTile = vector<vector<vector<int> > > (this->problem.nbTurns, vector<vector<int> > (
      this->problem.rows, vector<int> (this->problem.cols)));
  for (int t=0 ; t<problem.nbTurns ; t++) {
    for (int row=0 ; row<this->problem.rows ; row++) {
      for (int col=0 ; col<this->problem.cols ; col++) {
        this->scoreByTile[t][row][col] = 0;
        for (int town : this->problem.reachableTargets[row][col]) {
          if (this->coverage[town][t] == 0) {
            this->scoreByTile[t][row][col] += 1;
          }
        }
      }
    }
  }
}

void Solution::addLoon (int idx, vector<int> path) {
  // Follow the paths
  int x = this->problem.start.x;
  int y = this->problem.start.y;
  int z = 0;

  for (int t=0 ; t<this->problem.nbTurns ; t++) {
    int dz = path[t];

    this->loons[t][idx] = dz;

    if (z == 0 && dz != 1)
      continue;

    // Go back to correct layers
    z += dz;
    if (z < 1)
      z = 1;
    if (z > this->problem.layers) {
      z = this->problem.layers;
    }

    // Update x and y
    Coord next = this->problem.getWindDirection(x, y, z);
    x = next.x;
    y = next.y;

    // Exclude if out of range
    if (x < 0)
      break;

    // Update score
    this->score += this->scoreByTile[t][x][y];
    vector<int> & towns = this->problem.reachableTargets[x][y];
    for (int town : towns) {
      this->coverage[town][t] += 1;
    }
  }

  this->computeLocalScore();
}

void Solution::rmvLoon (int idx) {
  // Follow the paths
  int x = this->problem.start.x;
  int y = this->problem.start.y;
  int z = 0;

  for (int t=0 ; t<this->problem.nbTurns ; t++) {
    int dz = this->loons[t][idx];

    this->loons[t][idx] = dz;

    if (z == 0 && dz != 1)
      continue;

    // Go back to correct layers
    z += dz;
    if (z < 1)
      z = 1;
    if (z > this->problem.layers) {
      z = this->problem.layers;
    }

    // Update x and y
    Coord next = this->problem.getWindDirection(x, y, z);
    x = next.x;
    y = next.y;

    // Exclude if out of range
    if (x < 0)
      break;

    // Update score
    int dScore = 0;
    vector<int> & towns = this->problem.reachableTargets[x][y];
    for (int town : towns) {
      this->coverage[town][t] -= 1;
      if (this->coverage[town][t] == 0)
        dScore++;
    }

    this->score -= dScore;
  }

  this->computeLocalScore();
}
