#pragma once
#include <unordered_set>
#include <iostream>
#include <stack>
#include <queue>
#include <chrono>
#include "tictactoe.h"
#include <omp.h>
using namespace std;

// Main game loop
void gameLoop(Tictactoe &board, int depth, int numThreads);

// Finds all the possible next moves
void exploreNextStates(Tictactoe &board);

// Prints the board
void drawBoard(Tictactoe &boardT);

// Terminal state is not reached. Evaluate the board
int evaluateState(Tictactoe& board);

// The main minimax algorithm
int minimax(Tictactoe& board, int &statesCount, int depth, int maxDepth);

// Runs the minimax algorithm on all the possible moves and finds the best move
vector<int> evaluateBestMove(Tictactoe& board, int depth, int numThreads);

// Just checks if an input is a number or not
int isNumber(string input);