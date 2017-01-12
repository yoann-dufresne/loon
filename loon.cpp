#include <iostream>
#include <vector>
#include <ctime>
#include "Problem.h"
#include "Solution.h"
#include "Coordz.h"
#include "HillClimbing.h"


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
  cout << "Problem loaded" << endl;

  Solution improve(prob);

  cout << "Solution created" << endl;
  DynamicProgramming dyn (prob, improve);
  cout << "Dynamic programming" << endl;
  
  for (int i=0 ; i<prob.nbLoons ; i++) {
    cout << "Optimisation loon " << i << endl;
    //dyn.sol.printScoreByTile ();
    int score = dyn.sol.score;
    dyn.addLoonIter(i);
    //dyn.print();
    //dyn.sol.printScoreByTile();
    cout << score << " -> " << dyn.sol.score << endl;
    dyn.sol.save("iter/TestIter_" + to_string(dyn.sol.score) + ".txt");
  }

  return 0;
}
