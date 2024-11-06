#include "puzzle_solver.h"

puzzle_solver::puzzle_solver(vector<puzzlepiece>& pieces, pair<int, int> dimensions)
    : grid(vector<vector<puzzlepiece>>(dimensions.first, vector<puzzlepiece>(dimensions.second))), unfitted_pieces(pieces){}
