#include "puzzlepiece.h"

puzzlepiece::puzzlepiece(int id, const std::vector<std::vector<int>>& north, const std::vector<std::vector<int>>& east, const std::vector<std::vector<int>>& south, const std::vector<std::vector<int>>& west)
    : id(id), north(north), east(east), south(south), west(west), location(std::pair<int, int>(-1, -1)) {}

puzzlepiece::puzzlepiece() : id(0), location(pair<int, int>(-1, -1)) {};

void puzzlepiece::rotate(){
    const vector<vector<int>> temp = north;
    north = west;
    west = south;
    south = east;
    east = temp;
}

void puzzlepiece::print_piece(){
    cout << "id: " << id << endl;
    cout << "location: (" << location.first << ", " << location.second << ")" << endl;

    auto print_side = [](const vector<vector<int>>& side, const string& label) {
        cout << label << ":" << endl;
        cout << "[";
        for (const vector<int>& row : side) {
            cout << "[";
            for (const int edge : row) {
                cout << edge << " ";
            }
            cout << "]" << endl;
        }
        cout << "]" << endl << endl;
    };

    print_side(north, "north");
    print_side(east, "east");
    print_side(south, "south");
    print_side(west, "west");
}
