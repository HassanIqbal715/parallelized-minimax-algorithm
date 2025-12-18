#pragma once
#include <iostream>
#include <vector>
using namespace std;

class Tictactoe {
private:
    // board to have 3 states. 0 = empty, 1 = player1, 2 = player2.
    vector<vector<int>> board;

    // turn 1 = player1, 2 = player2
    int turn;

    // stores the path this node has taken
    vector<vector<vector<int>>*> path;

    int winner;

    int size;

public:
    Tictactoe(int size); // Constructor
    int getTurn();
    int getSize();
    void setSize(int size);
    vector<vector<int>> getBoard();
    vector<vector<vector<int>>*> getPath(); // Unused function.
    bool checkBounds(int x, int y); // Checks if an input is within the board
    bool checkCellEmpty(int x, int y);
    int checkWin();
    bool checkDraw();
    int gameOver(); // Checks win, draw, continue
    bool placeMarker(int x, int y);
    bool operator==(Tictactoe& board2); // For easy comparison of boards
};