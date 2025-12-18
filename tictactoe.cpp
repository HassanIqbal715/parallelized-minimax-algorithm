#include "tictactoe.h"
using namespace std;

Tictactoe::Tictactoe(int size) {
    board.resize(size, vector<int>(size, 0));
    turn = 1;
    winner = 0;
    this->size = size;
    path.push_back(new vector<vector<int>>(board));
}

int Tictactoe::getTurn() {
    return turn;
}

int Tictactoe::getSize() {
    return size;
}

void Tictactoe::setSize(int size) {
    this->size = size;
}

vector<vector<int>> Tictactoe::getBoard() {
    return board;
}

vector<vector<vector<int>>*> Tictactoe::getPath() {
    return path;
}

bool Tictactoe::checkBounds(int x, int y) {
    // Making sure the index provided is within the board's limits
    return !(x < 0 || x > size - 1 || y < 0 || y > size - 1);
}

bool Tictactoe::checkCellEmpty(int x, int y) {
    return (board[y][x] == 0);
}

int Tictactoe::checkWin() {
    // Check Rows and Cols in one pass
    for (int i = 0; i < size; i++) {
        // Check Row i
        if (board[i][0] != 0) { // Optimization: Don't check empty starts
            bool rowWin = true;
            for (int c = 1; c < size; c++) {
                if (board[i][c] != board[i][0]) {
                    rowWin = false;
                    break;
                }
            }
            if (rowWin) return board[i][0];
        }

        // Check Col i
        if (board[0][i] != 0) {
            bool colWin = true;
            for (int r = 1; r < size; r++) {
                if (board[r][i] != board[0][i]) {
                    colWin = false;
                    break;
                }
            }
            if (colWin) return board[0][i];
        }
    }

    // Check Main Diagonal
    if (board[0][0] != 0) {
        bool diagWin = true;
        for (int i = 1; i < size; i++) {
            if (board[i][i] != board[0][0]) {
                diagWin = false;
                break;
            }
        }
        if (diagWin) return board[0][0];
    }

    // Check Anti-Diagonal
    if (board[0][size - 1] != 0) {
        bool antiWin = true;
        for (int i = 1; i < size; i++) {
            if (board[i][size - 1 - i] != board[0][size - 1]) {
                antiWin = false;
                break;
            }
        }
        if (antiWin) return board[0][size - 1];
    }

    return 0; // No winner
}

bool Tictactoe::checkDraw() {
    // some player already won the game
    if (checkWin() == 1 || checkWin() == 2) return false;

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            // There is an empty space available
            if (board[i][j] == 0) {
                return false;
            }
        }
    } 

    // No empty spaces found, board is full, draw
    return true; 
}

bool Tictactoe::placeMarker(int x, int y) {
    if (!checkBounds(x, y) || !checkCellEmpty(x, y) ||
        checkWin() == 1 || checkWin() == 2 || checkDraw())
        return false; // Move invalid

    // Place for player 1
    if (turn == 1) {
        board[y][x] = 1;
        turn = 2;
    }
    // Place for player 2
    else {
        board[y][x] = 2;
        turn = 1;
    }

    // Keep track of the branch followed to help for backtracking
    path.push_back(new vector<vector<int>>(board));

    return true;  // Move valid
}

int Tictactoe::gameOver() {
    int win = checkWin();
    
    if (win == 1) // Player 1 win
        return 1;
    else if (win == 2) // Player 2 win
        return -1;
    else if (checkDraw()) // Draw
        return 0;
    
    return 2; // Continue (Not a terminal state)
}

// For easy comparison of different boards
bool Tictactoe::operator==(Tictactoe& board2) {
    return (board == board2.getBoard());
}