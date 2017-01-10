#include <iostream>
#include <vector>
#include <ctime>
#include "Problem.h"
#include "Solution.h"
#include "Coordz.h"
#include "HillClimbing.h"

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
  cout << "Problem loaded" << endl;

  Solution improve(prob);
  /*/cout << "load" << endl;
  //improve.load("results/2loons_699764.txt");
  cout << "score " << improve.score << endl;

  HillClimbing hc (prob, improve);

  for (int i=0 ; i<3 ; i++) {
    hc.oneStep(2);
    int score = hc.dp.sol.score;
    hc.dp.sol.save("results/2loons_" + to_string(score) + ".txt");
  }*/

  DynamicProgramming dyn (prob, improve);
  cout << "Dynamic programming" << endl;
  
  dyn.addLoonIter(0);

  return 0;
}
