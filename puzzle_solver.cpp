#include "puzzle_solver.h"

// cant assume perfect border edges

puzzle_solver::puzzle_solver(vector<puzzlepiece>& internal_pieces, vector<puzzlepiece>& border_pieces, int total_number_pieces)
    : grid(vector<vector<puzzlepiece>>()), unfitted_internal_pieces(internal_pieces), unfitted_border_pieces(border_pieces), total_number_pieces(total_number_pieces) {}

int puzzle_solver::calculate_similarity(const vector<vector<int>>& edge1, const vector<vector<int>>& edge2){
}

puzzlepiece puzzle_solver::find_first_corner(vector<vector<int>>& border_edge){

    int n = unfitted_border_pieces.size();
    for (int idx = 0; idx < n; ++idx){
        puzzlepiece piece = unfitted_border_pieces[idx];
        int num_rotations = 0;
        while (num_rotations < 4){
            if (calculate_similarity(border_edge, piece.west) + calculate_similarity(border_edge, piece.north) == 0){ // two border edges
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

    int x_row = pos.first, y_col = pos.second;
    pair<int, puzzlepiece> best_fit(INT_MAX, puzzlepiece());
    bool check_bottom = (x_row == (int) grid.size()-2);
    bool check_right = (y_col == (int) grid[0].size()-2);
    for (puzzlepiece piece : unfitted_internal_pieces){
        int num_rotations = 0;
        while (num_rotations < 4){
            int score = 0;
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

void puzzle_solver::fit_top_row(vector<vector<int>>& border_edge){

    vector<puzzlepiece> top_row;
    top_row.push_back(find_first_corner(border_edge));
    while (calculate_similarity(border_edge, top_row.back().east) + calculate_similarity(border_edge, top_row.back().north) == 0){
        pair<int, puzzlepiece> best_fit = {INT_MAX, puzzlepiece()};
        for (puzzlepiece& piece : unfitted_border_pieces){
            int num_rotations = 0;
            while (num_rotations < 4){
                if (calculate_similarity(border_edge, piece.north) == 0){
                    int score = calculate_similarity(top_row.back().west, piece.east);
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

void puzzle_solver::fit_right_col(vector<vector<int>>& border_edge){

    while (calculate_similarity(border_edge, grid.back().back().south) + calculate_similarity(border_edge, grid.back().back().east) > 0){
        vector<puzzlepiece> next_row(grid[0].size(), puzzlepiece());
        pair<int, puzzlepiece> best_fit = {INT_MAX, puzzlepiece()};
        for (puzzlepiece& piece : unfitted_border_pieces){
            int num_rotations = 0;
            while (num_rotations < 4){
                if (calculate_similarity(border_edge, piece.east) == 0){
                    int score = calculate_similarity(grid.back().back().south, piece.north);
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

void puzzle_solver::fit_bottom_row(vector<vector<int>>& border_edge){

    while (calculate_similarity(border_edge, grid.back().front().west) + calculate_similarity(border_edge, grid.back().front().south) > 0){
        pair<int, puzzlepiece> best_fit = {INT_MAX, puzzlepiece()};
        for (puzzlepiece& piece : unfitted_border_pieces){
            int num_rotations = 0;
            while (num_rotations < 4){
                if (calculate_similarity(border_edge, piece.south) == 0){
                    int score = calculate_similarity(grid.back().front().west, piece.east);
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

void puzzle_solver::fit_left_col(vector<vector<int>>& border_edge){

    int n = grid.size();
    for (int row_idx = n-1; row_idx > 0; --row_idx){
        pair<int, puzzlepiece> best_fit = {INT_MAX, puzzlepiece()};
        for (puzzlepiece& piece : unfitted_border_pieces){
            int num_rotations = 0;
            while (num_rotations < 4){
                if (calculate_similarity(border_edge, piece.west) == 0){
                    int score = calculate_similarity(grid[row_idx+1][0].north, piece.south);
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

    int rows = unfitted_border_pieces[0].north.size(), cols = unfitted_border_pieces[0].north[0].size(); // grab dimensions of edge matrix
    vector<vector<int>> border_edge(rows, vector<int>(cols, 0));
    for (int col = 0; col < cols; ++col){
        border_edge[0][col] = 1;
    }

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
