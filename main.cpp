#include <iostream>
#include <string>
#include "tictactoe.h"
#include "functions.h"

using namespace std;

int main() {
    int boardSize = 0;
    int depth = 0; // This depth is the maxdepth of minimax algorithm
    string option;
    bool flag = false; // To help judge correct and wrong inputs

    // Board size picking
    while (true) {
        cout << "Pick a board size:\n";
        cout << "1. 3x3\n";
        cout << "2. 4x4\n";
        cout << "3. 5x5\n";
        cout << "4. 6x6\n";
        cout << "0. exit\n";
        cin >> option;
        int result = -1;

        result = isNumber(option);

        if (result == -1) {
            cout << "Wrong input! Try again\n";
            continue;
        }

        switch(result) {
            case 1:
                boardSize = 3;
                depth = 100; // No depth limit
                flag = true;
                break;

            case 2:
                boardSize = 4;
                depth = 5;
                flag = true;
                break;

            case 3:
                boardSize = 5;
                depth = 4;
                flag = true;
                break;

            case 4:
                boardSize = 6;
                depth = 3;
                flag = true;
                break;

            case 0:
                cout << "Thanks for playing!\n";
                return 0;

            default:
                cout << "Wrong input! Try again\n";
                break;
        }

        if (flag)
            break;
    }

    int numThreads = 0;
    flag = false;

    while(true) {
        cout << "Pick number of threads:\n";
        cout << "1. 1\n";
        cout << "2. 2\n";
        cout << "3. 4\n";
        cout << "4. 8\n";
        cout << "5. 12\n";
        cout << "0. exit\n";
        cin >> option;
        int result = -1;

        result = isNumber(option);

        if (result == -1) {
            cout << "Wrong input! Try again\n";
            continue;
        }

        switch(result) {
            case 1:
                numThreads = 1;
                flag = true;
                break;

            case 2:
                numThreads = 2;
                flag = true;
                break;

            case 3:
                numThreads = 4;
                flag = true;
                break;

            case 4:
                numThreads = 8;
                flag = true;
                break;
                
            case 5:
                numThreads = 12;
                flag = true;
                break;

            case 0:
                cout << "Thanks for playing!\n";
                return 0;

            default:
                cout << "Wrong input! Try again\n";
                break;
        }

        if (flag)
            break;
    }

    flag = false;

    while(true) {
        cout << "Pick scheduling type:\n";
        cout << "1. static\n";
        cout << "2. guided\n";
        cout << "3. dynamic\n";
        cout << "0. exit\n";
        cin >> option;

        int result = -1;

        result = isNumber(option);
        
        if (result == -1) {
            cout << "Wrong input! Try again\n";
            continue;
        }

        switch(result) {
            case 1:
                omp_set_schedule(omp_sched_static, 1);
                flag = true;
                break;
            
            case 2:
                omp_set_schedule(omp_sched_guided, 1);
                flag = true;
                break;
            
            case 3:
                omp_set_schedule(omp_sched_dynamic, 1);
                flag = true;
                break;
            
            case 0:
                cout << "Thanks for playing!\n";
                flag = true;
                return 0;
            
            default:
                cout << "Wrong input! Try again\n";
                break;
        }

        if (flag)
            break;
    }

    Tictactoe board1(boardSize); // Board instance
    gameLoop(board1, depth, numThreads);
}