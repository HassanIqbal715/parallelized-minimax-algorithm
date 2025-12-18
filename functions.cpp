#include "functions.h"

void printMenu() {
    cout << "1. Play your turn.\n";
    cout << "2. Check next possible moves\n";
    cout << "3. Check optimal move\n";
    cout << "0. Exit\n";
}

void gameLoop(Tictactoe &board, int depth, int numThreads) {
    string xInput;
    string yInput;
    string optionInput;

    int x = -1;
    int y = -1;
    int option = -1;
    vector<int> bestMove;

    // Main loop
    while(board.checkWin() == 0 && !board.checkDraw()) {
        cout << "\n================\n";
        drawBoard(board);     

        int turn = board.getTurn(); // Current turn
        char letter = (turn == 1) ? 'O' : 'X';  // If player 1, turn O

        cout << "\n================\n";
        cout << "Player " << turn << "'s turn("<< letter <<")\n";
        printMenu();
        cout << "Enter your option: ";
        cin >> optionInput;

        option = isNumber(optionInput); // Check if input is valid

        // Input wrong!
        if (option == -1) {
            cout << "Wrong input! Try again\n";
            continue;
        }

        bool inputFlag = false; // To check if input is valid.

        switch (option) {
            // Exit
            case 0:
                cout << "Thank you for playing!\n";
                return;
                break;

            // Play. Enter coordinates.
            case 1:
                cout << "Enter coordinates:\n";
                cout << "x: ";
                cin >> xInput;
                x = isNumber(xInput);

                if (x == -1) {
                    cout << "Wrong input! Try again\n";
                    inputFlag = true;
                    break;
                }

                cout << "y: ";
                cin >> yInput;
                y = isNumber(yInput);

                if (y == -1) {
                    cout << "Wrong input! Try again\n";
                    inputFlag = true;
                    break;
                }

                if (!board.placeMarker(x, y)) {
                    cout << "\n Wrong input... try again";
                    break;
                }

                break;
            
            // All possible next moves
            case 2:
                exploreNextStates(board);
                break;
            
            // Use minimax to find the best move
            case 3:
                bestMove = evaluateBestMove(board, depth, numThreads);
                cout << "Best Move:\n";
                cout << "(x, y) = " << "(" << bestMove[0] << ", " << bestMove[1] 
                    << ")";
                cout << '\n';
                break;
            
            default:
                cout << "Wrong input...\n";
                break;
        }

        if (inputFlag)
            continue;
    }

    // Returns a number for win(1, -1), draw(0) or continue(2) condition
    int isEnded = board.gameOver();
    if (isEnded == 1) {
        drawBoard(board); 
        cout << "Player 1 is the winner!";
        return;
    }
    else if (isEnded == -1) {
        drawBoard(board); 
        cout << "Player 2 is the winner!";
        return;
    }
    else if (isEnded == 0) {
        drawBoard(board); 
        cout << "Its a Draw!!";
        return;
    }
}

void drawBoard(Tictactoe &boardT) {
    vector<vector<int>> board = boardT.getBoard();
    int size = boardT.getSize();
    cout << "    ";

    for (int i = 0; i < size; i++) {
        cout << i << " ";
    }

    cout << "\n";

    for (int i = 0; i < size; i++) {
        cout << i <<" | ";

        for (int j = 0; j < size; j++) {
            if (board[i][j] == 0) {
                cout << " ";
            }
            else if (board[i][j] == 1) {
                cout << "O";
            }
            else {
                cout << "X";
            }

            cout << " ";
        }

        cout << "|\n";
    }
}

void exploreNextStates(Tictactoe &board) {
    // A copy which acts as a restore point for the actual board.
    Tictactoe copyBoard = board;
    int size = board.getSize();

    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            if (board.placeMarker(j, i)) {
                drawBoard(board);  // Only draw if its a valid move.
            }
            board = copyBoard;  // revert to board's actual state.
        }
    }
}

// To generate a key for the visited set
string encodeBoard(Tictactoe& b) {
    string s;
    int size = b.getSize();
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            s.push_back('0' + b.getBoard()[i][j]);
        }
    }
    return s;
}

int minimax(Tictactoe &board, int &statesCount, int depth, int maxDepth) {
    int score = board.gameOver(); 
    
    // 1. Base Case: Terminal State (Win/Loss)
    // Use very high numbers so actual wins always beat heuristics
    if (score == 1) return 100000 - depth;  
    if (score == -1) return -100000 + depth; 
    if (score == 0) return 0;  

    // 2. Base Case: Max Depth Reached
    // If we hit the limit, stop recursing and evaluate the state
    if (depth >= maxDepth) {
        return evaluateState(board);
    }
    
    ++statesCount;
    int size = board.getSize();

    if (board.getTurn() == 1) { // Maximizing
        int maxVal = -999999; // Lower than any possible heuristic (-infinity)
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                Tictactoe newBoard = board;
                if (newBoard.placeMarker(j, i)) {
                    // Pass maxDepth along
                    int eval = minimax(newBoard, statesCount, depth + 1, maxDepth);
                    maxVal = max(maxVal, eval);
                }
            }
        }
        return maxVal;
    } 
    else { // Minimizing
        int minVal = 999999; // Higher than any possible heuristic (infinity)
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                Tictactoe newBoard = board;
                if (newBoard.placeMarker(j, i)) {
                    int eval = minimax(newBoard, statesCount, depth + 1, maxDepth);
                    minVal = min(minVal, eval);
                }
            }
        }
        return minVal;
    }
}

int evaluateState(Tictactoe& board) {
    int size = board.getSize();
    vector<vector<int>> b = board.getBoard();
    int score = 0;

    // Lambda to score a single line (row, col, or diag)
    auto scoreLine = [&](int p1Count, int p2Count) {
        if (p1Count > 0 && p2Count > 0) return 0; // Blocked line, worthless
        if (p1Count > 0) {
            if (p1Count == size - 1) return 1000; // One move away from win
            if (p1Count == size - 2) return 100;
            return 10;
        }
        if (p2Count > 0) {
            if (p2Count == size - 1) return -1000; // Opponent one move away
            if (p2Count == size - 2) return -100;
            return -10;
        }
        return 0;
    };

    // 1. Check Rows
    for (int i = 0; i < size; i++) {
        int p1 = 0, p2 = 0;
        for (int j = 0; j < size; j++) {
            if (b[i][j] == 1) p1++; // Assuming 1 is Player 1
            else if (b[i][j] == 2) p2++; // Assuming 2 is Player 2
        }
        score += scoreLine(p1, p2);
    }

    // 2. Check Cols
    for (int j = 0; j < size; j++) {
        int p1 = 0, p2 = 0;
        for (int i = 0; i < size; i++) {
            if (b[i][j] == 1) p1++;
            else if (b[i][j] == 2) p2++;
        }
        score += scoreLine(p1, p2);
    }

    // 3. Check Diagonals
    int p1_d1 = 0, p2_d1 = 0;
    int p1_d2 = 0, p2_d2 = 0;
    for (int i = 0; i < size; i++) {
        // Primary diagonal
        if (b[i][i] == 1) p1_d1++;
        else if (b[i][i] == 2) p2_d1++;
        
        // Secondary diagonal
        if (b[i][size - 1 - i] == 1) p1_d2++;
        else if (b[i][size - 1 - i] == 2) p2_d2++;
    }
    score += scoreLine(p1_d1, p2_d1);
    score += scoreLine(p1_d2, p2_d2);

    return score;
}

vector<int> evaluateBestMove(Tictactoe& board, int depth, int numThreads) {
    int currentTurn = board.getTurn();
    
    // Global bests
    int globalBestVal = (currentTurn == 1) ? -999999 : 999999;
    vector<int> globalBestMove = {-1, -1};
    
    int size = board.getSize();
    int statesCount = 0;

    auto start = chrono::steady_clock::now();

    // Parallel region which will divide all the moves work to threads
    #pragma omp parallel for collapse(2) reduction(+:statesCount) num_threads(numThreads) schedule(runtime)
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < size; j++) {
            
            // Private copy for this thread's root move
            Tictactoe localBoard = board;

            if (localBoard.placeMarker(j, i)) {
                int localStates = 0;
                // Pass depth 0
                int moveVal = minimax(localBoard, localStates, 0, depth); 
                
                statesCount += localStates;

                // Critical section to update the Global Best
                #pragma omp critical
                {
                    if (currentTurn == 1) { // Maximize
                        if (moveVal > globalBestVal) {
                            globalBestVal = moveVal;
                            globalBestMove = {j, i};
                        }
                    } 
                    else { // Minimize
                        if (moveVal < globalBestVal) {
                            globalBestVal = moveVal;
                            globalBestMove = {j, i};
                        }
                    }
                }
            }
        }
    }

    auto end = chrono::steady_clock::now();
    auto difference = chrono::duration_cast<chrono::milliseconds>(end - start);

    cout << "Time taken: " << difference.count() << "ms\n";
    cout << statesCount << " states explored.\n";
    
    return globalBestMove;
}

int isNumber(string input) {
    int result;
    try {
        result = stoi(input);
        return result;
    }
    catch (const invalid_argument& e) {
        return -1;
    }
    catch (const out_of_range& e) {
        return -1;
    }
}