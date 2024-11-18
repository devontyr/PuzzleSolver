#ifndef PUZZLE_SOLVER_H
#define PUZZLE_SOLVER_H

#include "puzzlepiece.h"

class puzzle_solver
{
public:
    puzzle_solver(vector<puzzlepiece>& internal_pieces, vector<puzzlepiece>& border_pieces);
    void fit_internal_piece(pair<int, int> pos);
    void solve();
    double calculate_similarity(const vector<pair<int, int>>& edge1, const vector<pair<int, int>>& edge2);
    void fit_top_row(vector<pair<int, int>>& border_edge);
    void fit_right_col(vector<pair<int, int>>& border_edge);
    void fit_bottom_row(vector<pair<int, int>>& border_edge);
    void fit_left_col(vector<pair<int, int>>& border_edge);
    puzzlepiece find_first_corner(vector<pair<int, int>>& border_edge);

private:
    vector<vector<puzzlepiece>> grid;
    vector<puzzlepiece> unfitted_internal_pieces;
    vector<puzzlepiece> unfitted_border_pieces;
    int THRESHOLD;
    int total_number_pieces;
};

#endif // PUZZLE_SOLVER_H
