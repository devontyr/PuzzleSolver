#include "puzzlepiece.h"

puzzlepiece::puzzlepiece(const int id, const bool is_border, const vector<vector<int>> north, const vector<vector<int>> east, const vector<vector<int>> south, const vector<vector<int>> west)
    : id(id), location(pair<int, int>(-1, -1)), is_border(is_border), north(north), east(east), south(south),  west(west){}

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
    cout << "is_border: " << is_border << endl;

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
