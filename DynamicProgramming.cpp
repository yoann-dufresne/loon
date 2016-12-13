#include "DynamicProgramming.h"

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
      if (toProcess.size() > 10000) {
        floor = floorScore(toProcess, 0, best, 10000, 1000, t-1);
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
          if (floor > 1 && score < floor)
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

            this->from[t][next.x][next.y][next.z-1].x = current.x;
            this->from[t][next.x][next.y][next.z-1].y = current.y;
            this->from[t][next.x][next.y][next.z-1].z = current.z;

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