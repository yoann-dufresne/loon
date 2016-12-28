#include "HillClimbing.h"
#include <ctime>



HillClimbing::HillClimbing (Problem prob, Solution sol) {
	this->prob = prob;
	this->sol = Solution (prob);
	this->dp = DynamicProgramming(prob, sol);
};

HillClimbing::~HillClimbing() {};

void HillClimbing::saveLoon (int idx) {
	for (int t=0 ; t<this->prob.nbTurns ; t++) {
		this->sol.loons[t][idx] = this->dp.sol.loons[t][idx];
	}
};

void HillClimbing::loadLoon (int idx) {
	vector<int> loon;
	for (int t=0 ; t<this->prob.nbTurns ; t++) {
		loon.push_back(this->sol.loons[t][idx]);
	}

	this->dp.sol.rmvLoon(idx);
	this->dp.sol.addLoon(idx, loon);
};

void HillClimbing::oneStep (int nbLoons) {
	int before = this->dp.sol.score;
	cout << "increased from " << before << endl;

	// Save and remove random baloons
	srand(time(NULL));
	unordered_set<int> loons;
	for (int i=0 ; i<nbLoons ; i++) {
		int idx = rand() % this->prob.nbLoons;
		if (loons.find(idx) != loons.end()) {
			i--;
			continue;
		}
		cout << idx << " ";
		loons.insert(idx);
		this->saveLoon (idx);
		this->dp.sol.rmvLoon(idx);
	}
	cout << endl;

	// Compute new score
	for (int val : loons) {
		cout << "Recomputation for " << val << endl;
		this->dp.addLoon(val, 20);
	}

	// Reload old balloons if score is worse
	int score = this->dp.sol.score;
	cout << "to " << score << endl;
	if (before > score) {
		cout << "Rollback" << endl;
		for (int val : loons)
			this->loadLoon(val);
	}
};
