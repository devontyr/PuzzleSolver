#include "edgematcher.h"

EdgeMatcher::EdgeMatcher(){

}

double EdgeMatcher::calculate_similarity(const vector<vector<int>>& edge1, const vector<vector<int>>& edge2) {
    size_t rows = edge1.size();
    size_t cols = edge1[0].size();

    if (rows != edge2.size() || cols != edge2[0].size()) return 0.0;

    size_t total = rows * cols;
    size_t matching = 0;

    for (size_t index = 0; index < total; ++index) {
        size_t i = index / cols;
        size_t j = index % cols;

        if ((edge1[i][j] ^ edge2[i][j]) == 0) {
            ++matching;
        }
    }

    return static_cast<double>(matching) / total;
}

// find best piece method
//  is this given a select piece and a list of all the other pieces? some possible other peices?
