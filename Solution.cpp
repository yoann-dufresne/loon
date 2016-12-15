#include "Solution.h"

Solution::Solution (Problem prob) {
  this->loons = vector<vector<int> >(prob.nbTurns, vector<int>(prob.nbLoons));
  this->problem = prob;
  this->coverage = vector<vector<int> >(prob.nbTargets, vector<int>(prob.nbTurns, 0));
}

Solution::Solution () {}

void Solution::load (string filename) {
  ifstream file;
  file.open (filename);

  for (int t=0 ; t<this->problem.nbTurns ; t++) {
    for (int l=0 ; l<this->problem.nbLoons ; l++) {
      file >> this->loons[t][l];
      cout << this->loons[t][l] << ' ';
    }
    cout << endl;
  }

  file.close();
  this->initScore();
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

void Solution::initScore () {
  // Reinit
  for (int town=0 ; town<this->problem.nbTargets ; town++) {
    for (int t=0 ; t<this->problem.nbTurns ; t++) {
      this->coverage[town][t] = 0;
    }
  }

  // Follow the paths
  int x, y, z;
  for (int l=0 ; l<this->problem.nbLoons ; l++) {
    x = this->problem.start.x;
    y = this->problem.start.y;
    z = 0;

    for (int t=0 ; t<this->problem.nbTurns ; t++) {
      int dz = this->loons[t][l];
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
      Coord wind = this->problem.getWindDirection(x, y, z);
      
      x += wind.x;
      y = (y + wind.y + this->problem.cols) % this->problem.cols;

      // Exclude if out of range
      if (x < 0 || x >= this->problem.rows)
        break;

      // Scan each town
      vector<int> & towns = this->problem.reachableTargets[x][y];
      for (int town : towns) {
        this->coverage[town][t] += 1;
      }
    }
  }
}

int Solution::getScore () {
  int score = 0;
  for (int town=0 ; town<this->problem.nbTargets ; town++) {
    for (int t=0 ; t<this->problem.nbTurns ; t++) {
      if (this->coverage[town][t] > 0)
        score++;
    }
  }

  return score;
}