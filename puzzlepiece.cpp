#include "puzzlepiece.h"

puzzlepiece::puzzlepiece(int id, const vector<pair<int, int>>& north, const vector<pair<int, int>>& east, const vector<pair<int, int>>& south, const vector<pair<int, int>>& west)
    : id(id), north(north), east(east), south(south), west(west), location(pair<int, int>(-1, -1)) {}

puzzlepiece::puzzlepiece() : id(0), location(pair<int, int>(-1, -1)) {};

void puzzlepiece::rotate(){
    /*

    Rotates the piece 90 degrees clockwise
    Input:
        None
    Output:
        None

    */

    const vector<pair<int, int>> temp = north;
    north = west;
    west = south;
    south = east;
    east = temp;
}

void puzzlepiece::print_piece(){
    /*

    Helper function to print out the details of each piece, i.e. if it has been fitted and each side
    Input:
        None
    Output:
        None (I/O)

    */

    cout << "id: " << id << endl;
    cout << "location: (" << location.first << ", " << location.second << ")" << endl;

    auto print_side = [](const vector<pair<int, int>>& side, const string& label) {
        cout << label << ":" << endl;
        cout << "[ ";
        for (const pair<int, int>& loc : side) {
            cout << "(" << loc.first << ", " << loc.second << ") ";
        }
        cout << "]" << endl << endl;
    };

    print_side(north, "north");
    print_side(east, "east");
    print_side(south, "south");
    print_side(west, "west");
}
