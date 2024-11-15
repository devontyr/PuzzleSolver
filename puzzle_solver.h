#ifndef PUZZLE_SOLVER_H
#define PUZZLE_SOLVER_H

#include "puzzlepiece.h"

class puzzle_solver
{
public:
    puzzle_solver(vector<puzzlepiece>& internal_pieces, vector<puzzlepiece>& border_pieces, int total_number_pieces);
    void fit_internal_piece(pair<int, int> pos);
    void solve();
    int calculate_similarity(const vector<vector<int>>& edge1, const vector<vector<int>>& edge2);
    void fit_top_row(vector<vector<int>>& border_edge);
    void fit_right_col(vector<vector<int>>& border_edge);
    void fit_bottom_row(vector<vector<int>>& border_edge);
    void fit_left_col(vector<vector<int>>& border_edge);
    puzzlepiece find_first_corner(vector<vector<int>>& border_edge);

private:
    vector<vector<puzzlepiece>> grid;
    vector<puzzlepiece> unfitted_internal_pieces;
    vector<puzzlepiece> unfitted_border_pieces;
    int total_number_pieces;

};
#endif // PUZZLE_SOLVER_H
