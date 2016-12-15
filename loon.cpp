#include <iostream>
#include <vector>
#include <ctime>
#include "Problem.h"
#include "Solution.h"
#include "Coordz.h"
#include "DynamicProgramming.h"

#define ROWS 75
#define COLS 300
#define LAYERS 8


using namespace std;



int main () {
  // Parsing
  
  // Header
  int rows, cols, layers;
  cin >> rows >> cols >> layers;
  Problem prob(rows, cols, layers+1);

  cin >> prob.nbTargets >> prob.radius >> prob.nbLoons >> prob.nbTurns;

  // Start cell
  cin >> prob.start.x >> prob.start.y;

  //prob.nbTurns = 145;

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

  Solution improve(prob);
  improve.load("results/sol_615947.txt");
  DynamicProgramming algo (prob, improve);

  srand(time(0));
  for (int i=0 ; i<1 ; i++) {
    // Remove a balloon
    int idx = rand() % prob.nbLoons;

    for (int t=0 ; t<prob.nbTurns ; t++)
      algo.sol.loons[t][idx] = 0;
    algo.sol.initScore();
    cout << idx << ' ' << algo.sol.getScore() << endl;

    // Re optimize
    algo.addLoon(idx);

    // Save score
    algo.sol.save("results/sol_" + to_string(algo.sol.getScore()) + ".txt");
  }

  return 0;
}
