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
  cout << "load" << endl;
  improve.load("results/sol_694757.txt");
  cout << "score " << improve.score << endl;
  DynamicProgramming algo (prob, improve);

  /*for (int i=0 ; i<prob.nbLoons ; i++) {
    improve.rmvLoon(i);
    cout << improve.score << endl;
  }*/


  // Opti

  srand(time(0));
  for (int i=0 ; i<1000 ; i++) {
    // Remove a balloon
    int idx = rand() % prob.nbLoons;

    // Re optimize
    cout << algo.sol.score << " -> ";
    algo.sol.rmvLoon(idx);
    cout << algo.sol.score << " -> ";
    algo.addLoon(idx, 20);
    cout << algo.sol.score << endl;

    // Save score
    algo.sol.save("results/sol_" + to_string(algo.sol.score) + ".txt");
  }

  return 0;
}
