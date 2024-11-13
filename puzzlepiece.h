#ifndef PUZZLEPIECE_H
#define PUZZLEPIECE_H

#include <utility>
#include <vector>
#include <iostream>
#include <stack>
#include <queue>
#include <unordered_set>

using namespace std;

class puzzlepiece
{
public:
    puzzlepiece(int id, const vector<vector<int>>& north,
                const vector<vector<int>>& east, const vector<std::vector<int>>& south,
                const vector<vector<int>>& west);
    puzzlepiece();
    void rotate(); // rotate clockwise
    void print_piece();
    int id;
    vector<vector<int>> north;
    vector<vector<int>> east;
    vector<vector<int>> south;
    vector<vector<int>> west;
    pair<int, int> location; // -1, -1 if not fitted yet
};

#endif // PUZZLEPIECE_H
