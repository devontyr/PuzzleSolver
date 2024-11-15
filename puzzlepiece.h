#ifndef PUZZLEPIECE_H
#define PUZZLEPIECE_H

#include <utility>
#include <vector>
#include <iostream>
#include <stack>
#include <queue>
#include <unordered_set>
#include <cmath>

using namespace std;

class puzzlepiece
{
public:
    puzzlepiece(int id, const vector<pair<int, int>>& north,
                const vector<pair<int, int>>& east, const vector<pair<int, int>>& south,
                const vector<pair<int, int>>& west);
    puzzlepiece();
    void rotate(); // rotate clockwise
    void print_piece();
    int id;
    vector<pair<int, int>> north; // THESE SHOULD ALREADY BE SORTED BY X AXIS
    vector<pair<int, int>> east;
    vector<pair<int, int>> south;
    vector<pair<int, int>> west;
    pair<int, int> location; // -1, -1 if not fitted yet
};

#endif // PUZZLEPIECE_H
