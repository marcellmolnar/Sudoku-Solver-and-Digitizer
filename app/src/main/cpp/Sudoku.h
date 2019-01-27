//
// Created by Marci on 2019. 01. 26..
//

#ifndef SUDOKUSOLVERANDDIGITIZER_SUDOKU_H
#define SUDOKUSOLVERANDDIGITIZER_SUDOKU_H

// Based on this: http://pi.math.cornell.edu/~mec/Summer2009/meerkamp/Site/Solving_any_Sudoku_II.html

#include <algorithm>
#include <iostream>
#include <vector>

class Sudoku {
    // Indicates if a cell is solved
    bool* solvedCell;
    // Holds the cell's solution
    int* cellSolution;
    // It is a 9 cell long array at every cell
    bool** markUps;
    // Holds the solution(s)
    int** mySolutions;
    int numberOfSolutions;

    void refreshMarkUps();
    //void printMarkUps();
    bool solveSingleMarkups();
    bool searchForOneLineMarkup();
    bool findAndEliminatePreemptiveSets();
    int findLeastLongMarkupCell();
    bool solver(int**& solution, int& , bool = true);

public:
    Sudoku(int* );
    Sudoku(const Sudoku&);
    ~Sudoku();
    int solveSudoku();
    int getValue(int);
    //void printSudoku();
    //void printSolution();
    bool isValid();
    bool isSolved();
};

/*

void Sudoku::printMarkUps() {
    for (int i = 0; i < 81; i++) {
        if (solvedCell[i]) {
            cout << "ok           ";
        }
        else {
            int cnt = 0;
            for (int j = 0; j < 9; j++) {
                if (markUps[i][j]) {
                    cout << j+1 << ",";
                    cnt++;
                }
            }
            while (cnt < 6) {
                cout << "  ";
                cnt++;
            }
            cout << " ";
        }
        if ((i + 1) % 9 == 0) {
            cout << endl;
        }
    }
}

void Sudoku::printSudoku() {
    for (int i = 0; i < 81; i++) {
        if (solvedCell[i]) {
            cout << cellSolution[i] << " ";
        }
        else {
            cout << "0 ";
        }
        if ((i + 1) % 9 == 0) {
            cout << endl;
        }
    }
}

void Sudoku::printSolution() {
    for (int solNum = 0; solNum < numberOfSolutions; solNum++){
        for (int i = 0; i < 81; i++) {
            cout << mySolutions[solNum][i] << " ";
            if ((i + 1) % 9 == 0) {
                cout << endl;
            }
        }
        cout << endl;
    }
}
 */


#endif //SUDOKUSOLVERANDDIGITIZER_SUDOKU_H
