#ifndef PUZZLE_SOLVER_H
#define PUZZLE_SOLVER_H

#include "puzzlepiece.h"

class puzzle_solver
{
public:
    puzzle_solver(vector<puzzlepiece>& pieces, pair<int, int> direction);
    void fit_piece(pair<int, int> pos);
    void solve();

private:
    vector<vector<puzzlepiece>> grid;
    vector<puzzlepiece> unfitted_pieces;

};
#endif // PUZZLE_SOLVER_H
