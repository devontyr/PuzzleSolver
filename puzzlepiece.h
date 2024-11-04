#ifndef PUZZLEPIECE_H
#define PUZZLEPIECE_H

#include <utility>
#include <vector>
#include <iostream>

using namespace std;

class puzzlepiece
{
public:
    puzzlepiece(int id, bool is_border, vector<vector<int>> north, vector<vector<int>> east, vector<vector<int>> south, vector<vector<int>> west);
    void rotate(); // rotate clockwise
    void print_piece();

private:
    int id;
    pair<int, int> location; // -1, -1 if not fitted yet
    bool is_border;
    vector<vector<int>> north;
    vector<vector<int>> east;
    vector<vector<int>> south;
    vector<vector<int>> west;
};

#endif // PUZZLEPIECE_H
