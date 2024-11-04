#ifndef EDGEMATCHER_H
#define EDGEMATCHER_H

#include <vector>
using namespace std;

class EdgeMatcher
{
public:
    EdgeMatcher();
    double calculate_similarity(const vector<vector<int>>& edge1, const vector<vector<int>>& edge2);

private:

};

#endif // EDGEMATCHER_H
