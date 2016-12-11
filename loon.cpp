#include <iostream>
#include <vector>
#include <algorithm>
#include <ctime>
#include <cstdlib>
#include <cmath>
#include <unordered_set>

#define ROWS 75
#define COLS 300
#define LAYERS 8


using namespace std;


// Classes
class Coord {
public:
  int x;
  int y;

  Coord(int x, int y) {
    this->x = x;
    this->y = y;
  }

  Coord() {}
};

class Coordz : public Coord {
public:
  int z;

  Coordz(int x, int y, int z) {
    this->x = x;
    this->y = y;
    this->z = z;
  }

  Coordz () {
    this->x = 0;
    this->y = 0;
    this->z = 0;
  }

  bool operator==(const Coordz& a2) const {
    return this->x == a2.x && this->y == a2.y && this->z == a2.z;
  }
};

class my_coordz_hash{
public:
  size_t operator()( const Coordz & key ) const // <-- don't forget const
  {
    size_t hashVal = key.z;
    return hashVal;
  }
};

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

class Solution {
public:
  vector<vector<int> > coverage;
  vector<vector<int> > loons;
  Problem problem;

  Solution (Problem prob) {
    this->loons = vector<vector<int> >(prob.nbTurns, vector<int>(prob.nbLoons));
    this->problem = prob;
    this->coverage = vector<vector<int> >(prob.nbTargets, vector<int>(prob.nbTurns, 0));
  }

  Solution () {}

  void randomInit () {
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

  void print () {
    for (int turn=0 ; turn<this->problem.nbTurns ; turn++) {
      for (int loon=0 ; loon<this->problem.nbLoons ; loon++) {
        cout << this->loons[turn][loon] << ' ';
      }
      cout << endl;
    }
  }

  void initScore () {

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
        vector<int> towns = this->problem.getTargetsInRadius(x, y);
        for (int town : towns) {
          this->coverage[town][t] += 1;
        }
      }
    }
  }

  int getScore () {
    int score = 0;
    for (int town=0 ; town<this->problem.nbTargets ; town++) {
      for (int t=0 ; t<this->problem.nbTurns ; t++) {
        if (this->coverage[town][t] > 0)
          score++;
      }
    }

    return score;
  }
};


#define GET(row,col,lay,t) (lay==0 || t<0 ? 0 : this->scores[t][row][col][lay-1])
#define SET(row,col,lay,t, val) this->scores[t][row][col][lay-1] = val

class DynamicProgramming {
public:
  int **** scores;
  Coordz **** from;
  Solution sol;
  Problem prob;

  DynamicProgramming (Problem prob, Solution sol) {
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

  ~DynamicProgramming() {};

  void reinitArray () {
    for (int t=0 ; t<this->prob.nbTurns ; t++)
      for (int row=0 ; row<this->prob.rows ; row++)
        for (int col=0 ; col<this->prob.cols ; col++)
          for (int layer=0 ; layer<this->prob.layers ; layer++) {
            scores[t][row][col][layer] = 0;
            from[t][row][col][layer].x = -1;
          }
  }

  int floorScore (unordered_set<Coordz, my_coordz_hash> elems, int minScore, int maxMinScore,
                 int nbElements, int epsilon, int t) {
    int midScore = minScore + (maxMinScore - minScore) / 2;
    int count = 0;

    for (const Coordz & c : elems)
      if (GET(c.x, c.y, c.z, t) >= midScore)
        count++;

    if (midScore == minScore)
      return minScore + 1;
    else if (abs(nbElements-count) <= epsilon)
      return midScore;
    else if (nbElements-count > 0)
      return floorScore (elems, minScore, midScore, nbElements, epsilon, t);
    else
      return floorScore (elems, midScore, maxMinScore, nbElements, epsilon, t);
  }

  void addLoon (int idx) {
    this->reinitArray();

    unordered_set<Coordz, my_coordz_hash> toProcess;
    toProcess.insert (Coordz(this->prob.start.x, this->prob.start.y, 0));

    int best = 0;
    Coordz bestCoords;
    int floor = 0;

    for (int t=0 ; t<this->prob.nbTurns/**/ ; t++) {
      unordered_set<Coordz, my_coordz_hash> nextStep;
      cerr << t << " nb to process: " << toProcess.size() << " best: " << best << endl;
      if (toProcess.size() > 100) {
        floor = floorScore(toProcess, 0, best, 100, 100, t-1);
      }

      for (Coordz current : toProcess) {
        // All altitude changes
        for (int dz=-1 ; dz<=1 ; dz++) {
          // Cas où pas encore décolé
          if (current.z == 0 && dz != 1) {
            nextStep.insert(current);
            dz = 0;
            continue;
          }

          // Cas trop haut torp bas
          int z = current.z + dz;
          if (z == 0 || z > this->prob.layers)
            continue;

          // Create new coordinates
          Coord w = this->prob.getWindDirection(current.x, current.y, z);
          Coordz next (
            current.x + w.x,
            (current.y + w.y + this->prob.cols) % this->prob.cols,
            z
          );

          // Traiter les cas particuliers x
          if (next.x < 0 || next.x >= this->prob.rows)
            continue;

          // calculer le score
          int score = GET(current.x, current.y, current.z, t-1);
          if (score < floor)
            continue;

          vector<int> targets = this->prob.getTargetsInRadius(next.x, next.y);
          for (int town : targets) {
            if (this->sol.coverage[town][t] == 0)
              score++;
          }

          // Ajouter à la liste
          if (score >= GET(next.x, next.y, next.z, t)) {
            SET(next.x, next.y, next.z, t, score);
            nextStep.insert (next);

            this->from[t][next.x][next.y][next.z-1] = current;

            if (score >= best) {
              best = score;
              bestCoords = next;
            }
          }
        }
      }

      // Mettre a jour la liste
      toProcess = nextStep;
    }

    // Traceback
    Coordz current = bestCoords;
    for (int t=this->prob.nbTurns-1 ; t>=0 ; t--) {
      Coordz next = this->from[t][current.x][current.y][current.z-1];
      this->sol.loons[t][idx] = (current.z - next.z);
      current = next;
    }
    this->sol.initScore();
  }
};



int main () {
  // Parsing
  
  // Header
  int rows, cols, layers;
  cin >> rows >> cols >> layers;
  Problem prob(rows, cols, layers+1);

  cin >> prob.nbTargets >> prob.radius >> prob.nbLoons >> prob.nbTurns;

  // Start cell
  cin >> prob.start.x >> prob.start.y;

  prob.nbTurns = 10;

  // Targets
  for (int i=0 ; i<prob.nbTargets ; i++) {
    Coord c;
    cin >> c.x >> c.y;
    prob.setTarget(c);
  }

  // Winds
  for (int layer=1 ; layer<=prob.layers ; layer++) {
    for (int r=0 ; r<prob.rows ; r++) {
      for (int c=0 ; c<prob.cols ; c++) {
      	Coord coord;
      	cin >> coord.x >> coord.y;
      	prob.setWindDirection(r, c, layer, coord);
      }
    }
  }

  Solution random(prob);
  //random.randomInit();
  //random.print();
  //random.initScore();
  //cout << random.getScore() << endl;

  Solution dynamic(prob);
  DynamicProgramming algo (prob, dynamic);
  for (int i=0 ; i<30 ; i++) {
    algo.addLoon(i);
    cerr << algo.sol.getScore() << endl;
  }

  algo.sol.print();

  return 0;
}

