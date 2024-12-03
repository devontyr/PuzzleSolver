#include "puzzle_solver.h"

puzzle_solver::puzzle_solver(vector<puzzlepiece>& internal_pieces, vector<puzzlepiece>& border_pieces)
    : grid(vector<vector<puzzlepiece>>()), unfitted_internal_pieces(internal_pieces), unfitted_border_pieces(border_pieces), THRESHOLD(600) {} // threshold will need to be found experimentally

double puzzle_solver::calculate_similarity(const vector<pair<int, int>>& edge1, const vector<pair<int, int>>& edge2){
    /*

    Returns the similarity score, calculated by the sum, for all points x of edge1, of the smallest Euclidean distance between x and some point y in edge2
    Input:
        edge1 (vector of pairs {int, int}: the edge we compare to
        edge2 (vector of pairs {int, int}: the edge to iterate through and calculate distances for
    Output:
        double: the sum of the euclidean distance between the edges

    */

    double similarity = 0;
    for (const pair<int, int>& loc : edge1){
        double min_sim = INT_MAX;
        for (const pair<int, int>& edgetwo_loc : edge2){
            double this_sim = sqrt((loc.first - edgetwo_loc.first)*(loc.first - edgetwo_loc.first) + (loc.second - edgetwo_loc.second)*(loc.second - edgetwo_loc.second));
            if (this_sim < min_sim){
                min_sim = this_sim;
            }
        }
        similarity += min_sim;
    }
    return similarity;

}

puzzlepiece puzzle_solver::find_first_corner(vector<pair<int, int>>& border_edge){

    /*

    Returns a corner piece, which is we define by our convention as the top left corner of our puzzle
    Input:
        border_edge (vector of pairs {int, int}: our convention for defining a border edge
    Output:
        puzzlepiece: the piece being used as our first corner

    */

    int n = unfitted_border_pieces.size();
    for (int idx = 0; idx < n; ++idx){
        puzzlepiece piece = unfitted_border_pieces[idx];
        int num_rotations = 0;
        while (num_rotations < 4){
            if (calculate_similarity(border_edge, piece.west) < THRESHOLD && calculate_similarity(border_edge, piece.north) < THRESHOLD){ // two border edges
                unfitted_border_pieces.erase(unfitted_border_pieces.begin() + idx);
                return piece;
            }
            piece.rotate();
            num_rotations++;
        }
    }
    return puzzlepiece();

}

void puzzle_solver::fit_internal_piece(pair<int, int> pos){

    /*

    O - fitted
    ? - to be fitted
    X - not yet fitted

    OOOO
    O??O
    O??O
    OOOO

    The method with goes row-wise down our puzzle, finding and fitting the vest matching piece
    Input:
        pos (pair {int, int}): The position that we want to fit a piece for
    Output:
        None

    */

    int x_row = pos.first, y_col = pos.second;
    pair<double, puzzlepiece> best_fit(INT_MAX, puzzlepiece());
    bool check_bottom = (x_row == (int) grid.size()-2);
    bool check_right = (y_col == (int) grid[0].size()-2);
    for (puzzlepiece piece : unfitted_internal_pieces){
        int num_rotations = 0;
        while (num_rotations < 4){
            double score = 0;
            score += calculate_similarity(grid[x_row][y_col-1].east, piece.west);
            score += calculate_similarity(grid[x_row-1][y_col].south, piece.north);
            if (check_bottom){
                score += calculate_similarity(grid[x_row+1][y_col].north, piece.south);
            }
            if (check_right){
                score += calculate_similarity(grid[x_row][y_col+1].west, piece.east);
            }
            if (score < best_fit.first){
                best_fit = {score, puzzlepiece(piece.id, piece.north, piece.east, piece.south, piece.west)};
            }
            num_rotations++;
            piece.rotate();
        }
    }

    int n = unfitted_internal_pieces.size();
    for (int idx = 0; idx < n; ++idx){
        if (unfitted_internal_pieces[idx].id == best_fit.second.id){
            unfitted_internal_pieces.erase(unfitted_internal_pieces.begin() + idx);
            break;
        }
    }
    best_fit.second.location = {x_row, y_col};
    grid[x_row][y_col] = best_fit.second;
}

void puzzle_solver::fit_top_row(vector<pair<int, int>>& border_edge){

    /*

    O - fitted
    ? - to be fitted
    X - not yet fitted

    O???
    XXXX
    XXXX
    XXXX

    Fits the top row of our puzzle grid
    Input:
         border_edge (vector of pairs {int, int}: our convention for defining a border edge
    Output:
        None

    */

    vector<puzzlepiece> top_row;
    top_row.push_back(find_first_corner(border_edge));
    while (calculate_similarity(border_edge, top_row.back().east) > THRESHOLD || calculate_similarity(border_edge, top_row.back().north) > THRESHOLD){
        pair<double, puzzlepiece> best_fit = {INT_MAX, puzzlepiece()};
        for (puzzlepiece& piece : unfitted_border_pieces){
            int num_rotations = 0;
            while (num_rotations < 4){
                if (calculate_similarity(border_edge, piece.north) <= THRESHOLD){
                    double score = calculate_similarity(top_row.back().west, piece.east);
                    if (score < best_fit.first){
                        best_fit = {score, puzzlepiece(piece.id, piece.north, piece.east, piece.south, piece.west)};
                    }
                }
                ++num_rotations;
                piece.rotate();
            }
        }
        best_fit.second.location = {0, top_row.size()};
        int n = unfitted_internal_pieces.size();
        for (int idx = 0; idx < n; ++idx){
            if (unfitted_internal_pieces[idx].id == best_fit.second.id){
                unfitted_internal_pieces.erase(unfitted_internal_pieces.begin() + idx);
                break;
            }
        }
        top_row.push_back(best_fit.second);
    }
    grid.push_back(top_row);
}

void puzzle_solver::fit_right_col(vector<pair<int, int>>& border_edge){

    /*

    O - fitted
    ? - to be fitted
    X - not yet fitted

    OOOO
    XXX?
    XXX?
    XXX?

    Fits the rightmost column of our puzzle grid
    Input:
         border_edge (vector of pairs {int, int}: our convention for defining a border edge
    Output:
        None

    */

    while (calculate_similarity(border_edge, grid.back().back().south) > THRESHOLD || calculate_similarity(border_edge, grid.back().back().east) > THRESHOLD){
        vector<puzzlepiece> next_row(grid[0].size(), puzzlepiece());
        pair<double, puzzlepiece> best_fit = {INT_MAX, puzzlepiece()};
        for (puzzlepiece& piece : unfitted_border_pieces){
            int num_rotations = 0;
            while (num_rotations < 4){
                if (calculate_similarity(border_edge, piece.east) <= THRESHOLD){
                    double score = calculate_similarity(grid.back().back().south, piece.north);
                    if (score < best_fit.first){
                        best_fit = {score, puzzlepiece(piece.id, piece.north, piece.east, piece.south, piece.west)};
                    }
                }
                ++num_rotations;
                piece.rotate();
            }
        }
        best_fit.second.location = {grid.size()-1, grid[0].size()-1};
        int n = unfitted_internal_pieces.size();
        for (int idx = 0; idx < n; ++idx){
            if (unfitted_internal_pieces[idx].id == best_fit.second.id){
                unfitted_internal_pieces.erase(unfitted_internal_pieces.begin() + idx);
                break;
            }
        }
        next_row.back() = best_fit.second;
        grid.push_back(next_row);
    }
}

void puzzle_solver::fit_bottom_row(vector<pair<int, int>>& border_edge){

    /*

    O - fitted
    ? - to be fitted
    X - not yet fitted

    OOOO
    XXXO
    XXXO
    ???O

    Fits the bottom row of our puzzle grid
    Input:
         border_edge (vector of pairs {int, int}: our convention for defining a border edge
    Output:
        None

    */

    while (calculate_similarity(border_edge, grid.back().front().west) > THRESHOLD || calculate_similarity(border_edge, grid.back().front().south) > THRESHOLD){
        pair<double, puzzlepiece> best_fit = {INT_MAX, puzzlepiece()};
        for (puzzlepiece& piece : unfitted_border_pieces){
            int num_rotations = 0;
            while (num_rotations < 4){
                if (calculate_similarity(border_edge, piece.south) <= THRESHOLD){
                    double score = calculate_similarity(grid.back().front().west, piece.east);
                    if (score < best_fit.first){
                        best_fit = {score, puzzlepiece(piece.id, piece.north, piece.east, piece.south, piece.west)};
                    }
                }
                ++num_rotations;
                piece.rotate();
            }
        }
        best_fit.second.location = {grid.size()-1, grid[0].size() - grid.back().size()};
        int n = unfitted_internal_pieces.size();
        for (int idx = 0; idx < n; ++idx){
            if (unfitted_internal_pieces[idx].id == best_fit.second.id){
                unfitted_internal_pieces.erase(unfitted_internal_pieces.begin() + idx);
                break;
            }
        }
        grid.back().insert(grid.back().begin(), best_fit.second);
    }

}

void puzzle_solver::fit_left_col(vector<pair<int, int>>& border_edge){

    /*

    O - fitted
    ? - to be fitted
    X - not yet fitted

    OOOO
    ?XXO
    ?XXO
    OOOO

    Fits the leftmost column of our puzzle grid
    Input:
         border_edge (vector of pairs {int, int}: our convention for defining a border edge
    Output:
        None

    */

    int n = grid.size();
    for (int row_idx = n-1; row_idx > 0; --row_idx){
        pair<double, puzzlepiece> best_fit = {INT_MAX, puzzlepiece()};
        for (puzzlepiece& piece : unfitted_border_pieces){
            int num_rotations = 0;
            while (num_rotations < 4){
                if (calculate_similarity(border_edge, piece.west) <= THRESHOLD){
                    double score = calculate_similarity(grid[row_idx+1][0].north, piece.south);
                    if (score < best_fit.first){
                        best_fit = {score, puzzlepiece(piece.id, piece.north, piece.east, piece.south, piece.west)};
                    }
                }
                ++num_rotations;
                piece.rotate();
            }
        }
        best_fit.second.location = {row_idx, 0};
        int n = unfitted_internal_pieces.size();
        for (int idx = 0; idx < n; ++idx){
            if (unfitted_internal_pieces[idx].id == best_fit.second.id){
                unfitted_internal_pieces.erase(unfitted_internal_pieces.begin() + idx);
                break;
            }
        }
    }
}

void puzzle_solver::solve(){

    /*

    Master method that calls all our relevant functions
    Input:
        None
    Output:
        None

    */

    vector<pair<int, int>> border_edge;
    int num_edge_cols = 600;
    for (int idx =0; idx < num_edge_cols; ++idx){
        border_edge.push_back({0, idx});
    }

    find_first_corner(border_edge);
    fit_top_row(border_edge);
    fit_right_col(border_edge);
    fit_bottom_row(border_edge);
    fit_left_col(border_edge);

    int n = grid.size(), m = grid[0].size();
    for (int row_idx = 1; row_idx < n-1; ++row_idx){
        for (int col_idx = 1; col_idx < m-1; ++col_idx){
            fit_internal_piece({row_idx, col_idx});
        }
    }
}
