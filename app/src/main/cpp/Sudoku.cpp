//
// Created by Marci on 2019. 01. 26..
//
#include "Sudoku.h"

using namespace std;

Sudoku::Sudoku(int* numbers) {
    solvedCell = new bool[81];
    cellSolution = new int[81];
    markUps = new bool*[81];
    numberOfSolutions = 0;

    for (int i = 0; i < 81; i++) {
        cellSolution[i] = numbers[i];
        if (cellSolution[i] != 0) {
            solvedCell[i] = true;
        }
        else {
            solvedCell[i] = false;
        }
        markUps[i] = new bool[9];
        for (int j = 0; j < 9; j++) {
            // It can be any number
            markUps[i][j] = true;
        }
    }
}
Sudoku::Sudoku(const Sudoku& other){
    solvedCell = new bool[81];
    cellSolution = new int[81];
    markUps = new bool*[81];
    numberOfSolutions = 0;

    for (int i = 0; i < 81; i++) {
        cellSolution[i] = other.cellSolution[i];
        if (cellSolution[i] != 0) {
            solvedCell[i] = true;
        }
        else {
            solvedCell[i] = false;
        }
        markUps[i] = new bool[9];
        for (int j = 0; j < 9; j++) {
            // It can be any number
            markUps[i][j] = other.markUps[i][j];
        }
    }
}

int Sudoku::getValue(int v){
    return mySolutions[0][v];
}

void Sudoku::refreshMarkUps() {
    for (int i = 0; i < 81; i++) {
        // If not solved cell then contuinue
        if (!solvedCell[i]) continue;
        // Else refresh current cell's, row's and col's cells

        int currentCellsSolution = cellSolution[i];
        int currentCellsCol = i % 9;
        int currentCellsRow = (i - currentCellsCol) / 9;

        for (int running_row = 0; running_row < 9; running_row++) {
            markUps[currentCellsCol + 9 * running_row][currentCellsSolution - 1] = false;
        }
        for (int running_col = 0; running_col < 9; running_col++) {
            markUps[running_col + 9 * currentCellsRow][currentCellsSolution - 1] = false;
        }

        // Current 3x3 box's column index (0, 1 or 2)
        int boxColumnIndex = (currentCellsCol - (currentCellsCol % 3)) / 3;
        // Current 3x3 box's row index (0, 1 or 2)
        int boxRowIndex = (currentCellsRow - (currentCellsRow % 3)) / 3;
        int boxsTopLeftIndex = (3 * boxColumnIndex) + 9 * (3 * boxRowIndex);
        for (int row_offset = 0; row_offset < 3; row_offset++) {
            markUps[boxsTopLeftIndex + 9 * row_offset + 0][currentCellsSolution - 1] = false;
            markUps[boxsTopLeftIndex + 9 * row_offset + 1][currentCellsSolution - 1] = false;
            markUps[boxsTopLeftIndex + 9 * row_offset + 2][currentCellsSolution - 1] = false;
        }
    }
}


bool Sudoku::solveSingleMarkups() {
    bool foundNewSolution = false;
    for (int i = 0; i < 81; i++) {
        // Only if the cell is not already solved
        if (!solvedCell[i]) {
            bool oneNumberPossible = false;
            int solutionsIndex = 0;
            bool twoOrMoreNumberPossible = false;
            for (int j = 0; j < 9; j++) {
                // If this number possible
                if (markUps[i][j]) {
                    // If there are no possible number yet, this can be the solution
                    if (!oneNumberPossible) {
                        oneNumberPossible = true;
                        solutionsIndex = j;
                    }
                        // Else there IS already a possible number, so no clear solution yet
                    else {
                        twoOrMoreNumberPossible = true;
                    }
                }
            }

            // If only one number possible
            if (oneNumberPossible && !twoOrMoreNumberPossible) {
                solvedCell[i] = true;
                cellSolution[i] = solutionsIndex+1;
                foundNewSolution = true;
            }
        }
    }
    this->refreshMarkUps();
    return foundNewSolution;
}

bool Sudoku::searchForOneLineMarkup() {
    bool foundOneLineMarkup = false;

    // Searching in 3x3 boxes
    // Current 3x3 box's row index (0, 1 or 2)
    for (int boxRowIndex = 0; boxRowIndex < 3; boxRowIndex++) {
        // Current 3x3 box's column index (0, 1 or 2)
        for (int boxColumnIndex = 0; boxColumnIndex < 3; boxColumnIndex++) {
            int boxsTopLeftIndex = (3 * boxColumnIndex) + 9 * (3 * boxRowIndex);

            for (int number = 0; number < 9; number++){
                bool isNumberOk = false;
                bool oneLineInRow = false;
                int lineRow;
                bool oneLineInCol = false;
                int lineCol;

                // Cell offset: 0, 1, 2, 9, 10, 11, 18, 19, 20
                int* cell_offset_array = new int[9]{ 0, 1, 2, 9, 10, 11, 18, 19, 20 };
                for (int cell_offset_index = 0; cell_offset_index < 9; cell_offset_index++) {
                    int currentCell = boxsTopLeftIndex + cell_offset_array[cell_offset_index];

                    for (int matching_cell_offset_index = cell_offset_index + 1; matching_cell_offset_index < 9; matching_cell_offset_index++) {
                        int matchingCell = boxsTopLeftIndex + cell_offset_array[matching_cell_offset_index];
                        if (!solvedCell[currentCell] && !solvedCell[matchingCell]){
                            if (markUps[currentCell][number] && markUps[matchingCell][number]) {
                                if (cell_offset_index / 3 == matching_cell_offset_index / 3) {
                                    oneLineInRow = true;
                                    isNumberOk = true;
                                    lineRow = (currentCell - (currentCell % 9)) / 9;
                                }
                                else {
                                    if (cell_offset_index % 3 == matching_cell_offset_index % 3) {
                                        oneLineInCol = true;
                                        isNumberOk = true;
                                        lineCol = currentCell % 9;
                                    }
                                    else {
                                        isNumberOk = false;
                                    }
                                }
                            }
                        }
                    }
                }

                if (isNumberOk && oneLineInRow && !oneLineInCol) {
                    for (int col = 0; col < 9; col++) {
                        int currentCell = col + 9 * lineRow;
                        if ((col - (col % 3)) / 3 != boxColumnIndex) {
                            if (!solvedCell[currentCell]) {
                                if (markUps[currentCell][number]) {
                                    foundOneLineMarkup = true;
                                }
                                markUps[currentCell][number] = false;
                            }
                        }
                    }
                }
                if (isNumberOk && oneLineInCol && !oneLineInRow) {
                    for (int row = 0; row < 9; row++) {
                        int currentCell = lineCol + 9 * row;
                        if ((row - (row % 3)) / 3 != boxRowIndex) {
                            if (!solvedCell[currentCell]) {
                                if (markUps[currentCell][number]) {
                                    foundOneLineMarkup = true;
                                }
                                markUps[currentCell][number] = false;
                            }
                        }
                    }
                }
            }
        }
    }

    return foundOneLineMarkup;
}

bool isSetsIdentical(bool* set1, bool* set2) {
    for (int i = 0; i < 9; i++) {
        if (set1[i] != set2[i])
            return false;
    }
    return true;
}

bool Sudoku::findAndEliminatePreemptiveSets() {
    bool couldEliminate = false;

    // Search in rows
    for (int running_row = 0; running_row < 9; running_row++) {
        for (int col_number = 0; col_number < 9; col_number++) {
            bool foundMatch = false;
            vector<int> identicalWithCurrentCell;
            int currentCell = col_number + 9 * running_row;
            for (int mathing_col = col_number+1; mathing_col < 9; mathing_col++) {
                if (!solvedCell[currentCell] && !solvedCell[mathing_col + 9 * running_row]) {
                    if (isSetsIdentical(markUps[currentCell], markUps[mathing_col + 9 * running_row])) {
                        identicalWithCurrentCell.push_back(mathing_col);
                        foundMatch = true;
                    }
                }
            }

            int setSize = 0;
            for (int i = 0; i < 9; i++) {
                if (markUps[currentCell][i]) {
                    setSize++;
                }
            }
            if (setSize != identicalWithCurrentCell.size()+1 || !foundMatch) {
                continue;
            }

            for (int eliminate_col = 0; eliminate_col < 9; eliminate_col++) {
                // If it is not in the preemtive set
                if (find(identicalWithCurrentCell.begin(), identicalWithCurrentCell.end(), eliminate_col) ==
                    identicalWithCurrentCell.end() && eliminate_col != col_number) {
                    for (int number = 0; number < 9; number++) {
                        if (markUps[currentCell][number]) {
                            if (markUps[eliminate_col + 9 * running_row][number] == true) {
                                couldEliminate = true;
                            }
                            markUps[eliminate_col + 9 * running_row][number] = false;
                        }
                    }
                }
            }
        }
    }

    // Search in cols
    for (int running_col = 0; running_col < 9; running_col++) {
        for (int row_number = 0; row_number < 9; row_number++) {
            bool foundMatch = false;
            vector<int> identicalWithCurrentCell;
            int currentCell = running_col + 9 * row_number;
            for (int mathing_row = row_number + 1; mathing_row < 9; mathing_row++) {
                if (!solvedCell[currentCell] && !solvedCell[running_col + 9 * mathing_row]) {
                    if (isSetsIdentical(markUps[currentCell], markUps[running_col + 9 * mathing_row])) {
                        identicalWithCurrentCell.push_back(mathing_row);
                        foundMatch = true;
                    }
                }
            }

            int setSize = 0;
            for (int i = 0; i < 9; i++) {
                if (markUps[currentCell][i]) {
                    setSize++;
                }
            }
            if (setSize != identicalWithCurrentCell.size() + 1 || !foundMatch) {
                continue;
            }

            for (int eliminate_row = 0; eliminate_row < 9; eliminate_row++) {
                // If it is not in the preemtive set
                if (find(identicalWithCurrentCell.begin(), identicalWithCurrentCell.end(), eliminate_row) ==
                    identicalWithCurrentCell.end() && eliminate_row != row_number) {
                    for (int number = 0; number < 9; number++) {
                        if (markUps[currentCell][number]) {
                            if (markUps[running_col + 9 * eliminate_row][number] == true) {
                                couldEliminate = true;
                            }
                            markUps[running_col + 9 * eliminate_row][number] = false;
                        }
                    }
                }
            }
        }
    }

    // Searching in 3x3 boxes
    // Current 3x3 box's row index (0, 1 or 2)
    for (int boxRowIndex = 0; boxRowIndex < 3; boxRowIndex++){
        // Current 3x3 box's column index (0, 1 or 2)
        for (int boxColumnIndex = 0; boxColumnIndex < 3; boxColumnIndex++) {
            int boxsTopLeftIndex = (3 * boxColumnIndex) + 9 * (3 * boxRowIndex);

            // Cell offset: 0, 1, 2, 9, 10, 11, 18, 19, 20
            int* cell_offset_array = new int[9] {0, 1, 2, 9, 10, 11, 18, 19, 20};
            for (int cell_offset_index = 0; cell_offset_index < 9; cell_offset_index++) {
                int currentCell = boxsTopLeftIndex + cell_offset_array[cell_offset_index];

                bool foundMatch = false;
                vector<int> identicalWithCurrentCell;
                for (int matching_cell_offset_index = cell_offset_index + 1; matching_cell_offset_index < 9; matching_cell_offset_index++) {
                    int matchingCell = boxsTopLeftIndex + cell_offset_array[matching_cell_offset_index];

                    if (!solvedCell[currentCell] && !solvedCell[matchingCell]) {
                        if (isSetsIdentical(markUps[currentCell], markUps[matchingCell])) {
                            identicalWithCurrentCell.push_back(matchingCell);
                            foundMatch = true;
                        }
                    }
                }

                int setSize = 0;
                for (int i = 0; i < 9; i++) {
                    if (markUps[currentCell][i]) {
                        setSize++;
                    }
                }
                if (setSize != identicalWithCurrentCell.size() + 1 || !foundMatch) {
                    continue;
                }

                for (int elim_cell_offset_index = 0; elim_cell_offset_index < 9; elim_cell_offset_index++) {
                    int eliminateCell = boxsTopLeftIndex + cell_offset_array[elim_cell_offset_index];

                    // If it is not in the preemtive set
                    if (find(identicalWithCurrentCell.begin(), identicalWithCurrentCell.end(), eliminateCell) ==
                        identicalWithCurrentCell.end() && eliminateCell != currentCell) {
                        for (int number = 0; number < 9; number++) {
                            if (markUps[currentCell][number]) {
                                if (markUps[eliminateCell][number] == true) {
                                    couldEliminate = true;
                                }
                                markUps[eliminateCell][number] = false;
                            }
                        }
                    }
                }
            }
        }
    }


    return couldEliminate;
}


bool Sudoku::isValid() {
    // Checking rows
    for (int running_row = 0; running_row < 9; running_row++) {
        for (int col_number = 0; col_number < 9; col_number++) {
            int currentCell = col_number + 9 * running_row;
            for (int matchingCol = col_number + 1; matchingCol < 9; matchingCol++) {
                int matchingCell = matchingCol + 9 * running_row;
                if (solvedCell[currentCell] && solvedCell[matchingCell]) {
                    if (cellSolution[currentCell] == cellSolution[matchingCell]) {
                        return false;
                    }
                }
            }
        }
    }

    // Checking cols
    for (int running_col = 0; running_col < 9; running_col++) {
        for (int row_number = 0; row_number < 9; row_number++) {
            int currentCell = running_col + 9 * row_number;
            for (int matchingRow = row_number + 1; matchingRow < 9; matchingRow++) {
                int matchingCell = running_col + 9 * matchingRow;
                if (solvedCell[currentCell] && solvedCell[matchingCell]) {
                    if (cellSolution[currentCell] == cellSolution[matchingCell]) {
                        return false;
                    }
                }
            }
        }
    }

    // Checking 3x3 boxes
    // Current 3x3 box's row index (0, 1 or 2)
    for (int boxRowIndex = 0; boxRowIndex < 3; boxRowIndex++) {
        // Current 3x3 box's column index (0, 1 or 2)
        for (int boxColumnIndex = 0; boxColumnIndex < 3; boxColumnIndex++) {
            int boxsTopLeftIndex = (3 * boxColumnIndex) + 9 * (3 * boxRowIndex);

            // Cell offset: 0, 1, 2, 9, 10, 11, 18, 19, 20
            int* cell_offset_array = new int[9]{ 0, 1, 2, 9, 10, 11, 18, 19, 20 };
            for (int cell_offset_index = 0; cell_offset_index < 9; cell_offset_index++) {
                int currentCell = boxsTopLeftIndex + cell_offset_array[cell_offset_index];

                for (int matching_cell_offset_index = cell_offset_index + 1; matching_cell_offset_index < 9; matching_cell_offset_index++) {
                    int matchingCell = boxsTopLeftIndex + cell_offset_array[matching_cell_offset_index];

                    if (solvedCell[currentCell] && solvedCell[matchingCell]) {
                        if (cellSolution[currentCell] == cellSolution[matchingCell]) {
                            return false;
                        }
                    }
                }
            }
        }
    }


    return true;
}

bool Sudoku::isSolved() {
    for (int i = 0; i < 81; i++) {
        if (!solvedCell[i]) {
            return false;
        }
    }
    return this->isValid();
}

int Sudoku::findLeastLongMarkupCell() {
    int leastMarkupSize = 10;
    int cellIndex = -1;
    for (int i = 0; i < 81; i++) {
        if (!solvedCell[i]) {
            int markupSize = 0;
            for (int num = 0; num < 9; num++) {
                if (markUps[i][num]) {
                    markupSize++;
                }
            }
            if (markupSize < leastMarkupSize) {
                cellIndex = i;
                leastMarkupSize = markupSize;
            }
        }
    }
    return cellIndex;
}

bool Sudoku::solver(int**& solution, int& _numberOfSolutions, bool root) {
    this->refreshMarkUps();
    while (this->solveSingleMarkups() || this->findAndEliminatePreemptiveSets() || this->searchForOneLineMarkup()) {
        this->refreshMarkUps();
    }
    if (this->isSolved()) {
        _numberOfSolutions++;
        if (root) {
            solution = new int*[1];
            solution[0] = new int[81];
            for (int i = 0; i < 81; i++) {
                solution[0][i] = cellSolution[i];
            }
        }
        else {
            cout << endl;
            int** tempSols = new int*[_numberOfSolutions];
            for (int i = 0; i < _numberOfSolutions - 1; i++) {
                tempSols[i] = new int[81];
                for (int j = 0; j < 81; j++) {
                    tempSols[i][j] = solution[i][j];
                }
            }
            tempSols[_numberOfSolutions - 1] = new int[81];
            for (int i = 0; i < 81; i++) {
                tempSols[_numberOfSolutions - 1][i] = cellSolution[i];
            }
            if (_numberOfSolutions > 1) {
                delete[] solution;
            }
            solution = tempSols;
        }
        return true;
    }
    else {
        if (this->isValid()) {
            int guessCell = this->findLeastLongMarkupCell();
            int guessedNumber = 0;
            bool foundSolution = false;
            while (guessedNumber < 9) { //  && !success
                while (guessedNumber < 9 && !markUps[guessCell][guessedNumber]) guessedNumber++;
                if (guessedNumber < 9 && markUps[guessCell][guessedNumber]){
                    solvedCell[guessCell] = true;
                    cellSolution[guessCell] = guessedNumber + 1;
                    Sudoku newSud(*this);
                    bool success = newSud.solver(solution, _numberOfSolutions, false);
                    if (_numberOfSolutions == 10) {
                        return true;
                    }
                    if (success) {
                        foundSolution = true;
                    }
                }
                guessedNumber++;
            }

            return foundSolution;
        }
        else {
            if (root){
                cout << "sudoku is NOT valid!" << endl;
            }
            return false;
        }
    }
}

int Sudoku::solveSudoku() {
    if(solver(mySolutions, numberOfSolutions, true)) {
        return this->numberOfSolutions;
    }
    else {
        return 0;
    }
}

Sudoku::~Sudoku() {
    delete solvedCell;
    delete cellSolution;
    delete[] markUps;
    if (numberOfSolutions > 0) {
        delete[] mySolutions;
    }
}
