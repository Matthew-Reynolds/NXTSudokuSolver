#include "solver.h"
/**
 *	All functions, Author: Matthew Reynolds
 *
 *	The level of commenting in this file falls
 *	short of the other files in the project
 *	simply due to time constraints. My apologies.
 */

short getBoxNumber(short row, short col){
	return (short)(col/3) + 3*(row/3);
}

short getBoxEntryCol(short box, short entry){
	return 3*(box%3) + (entry%3);
}

short getBoxEntryRow(short box, short entry){
	return 3*(box/3) + (entry/3);
}

bool checkValid(const Sudoku & sudoku){
	bool isValid = true;

	for(short major = 0; major < 9 && isValid; major++){
		short rowInstances[9] = {0,0,0,0,0,0,0,0,0};
		short colInstances[9] = {0,0,0,0,0,0,0,0,0};
		short boxInstances[9] = {0,0,0,0,0,0,0,0,0};

		// Iterate through every cell
		for(short minor = 0; minor < 9; minor++){
			short val = 0;
			if(val = sudoku[major][minor])
				rowInstances[val-1]++;
			if(val = sudoku[minor][major])
				colInstances[val-1]++;
			if(val = sudoku[getBoxEntryRow(major, minor)]
				[getBoxEntryCol(major, minor)])
			boxInstances[val-1]++;

		}

		// Everything should not appear, or appear exactly once
		for(short check = 0; check < 9 && isValid; check++){
			isValid &= (rowInstances[check] < 2);
			isValid &= (colInstances[check] < 2);
			isValid &= (boxInstances[check] < 2);
		}
	}

	return isValid;
}

bool checkSolved(const Sudoku & sudoku){
	bool isValid = true;

	for(short major = 0; major < 9 && isValid; major++){
		short rowInstances[9] = {0,0,0,0,0,0,0,0,0};
		short colInstances[9] = {0,0,0,0,0,0,0,0,0};
		short boxInstances[9] = {0,0,0,0,0,0,0,0,0};

		for(short minor = 0; minor < 9; minor++){
			short val = 0;

			if(val = sudoku[major][minor])
				rowInstances[val-1]++;
			if(val = sudoku[minor][major])
				colInstances[val-1]++;
			if(val = sudoku[getBoxEntryRow(major, minor)]
				[getBoxEntryCol(major, minor)])
			boxInstances[val-1]++;
		}


		// Everything should appear exactly once
		for(short check = 0; check < 9 && isValid; check++){
			isValid &= (rowInstances[check] == 1);
			isValid &= (colInstances[check] == 1);
			isValid &= (boxInstances[check] == 1);
		}
	}

	return isValid;
}

bool checkCol(short * potentialColVals, short * potentialRowVals,
							short * potentialBoxVals, Sudoku & sudoku){

	bool madeChange = false;

	// Iterate through each col, one at a time
	for(short col = 0; col < 9; col++){
		short numInstances[9] = {0,0,0,0,0,0,0,0,0};
		short possibleRow[9] = {0,0,0,0,0,0,0,0,0};

		// Iterate through every number 1-9, and get how many
		// spots this number can be placed in this column
		for(short row = 0; row < 9; row++){
			if(sudoku[row][col] < 1){
				short newBit = potentialColVals[col] &
				potentialRowVals[row] &
				potentialBoxVals[getBoxNumber(row, col)];

				for(short bit = 0; bit < 9; bit++){
					if(newBit & decToBit(bit+1)){
						numInstances[bit] ++;
						possibleRow[bit] = row;
					}
				}
			}
		}

		// Now that we've counted the possible locations, find all the
		// values that have only one possible place
		for(short curVal = 0; curVal < 9; curVal++){
			if(numInstances[curVal] == 1){
				madeChange = true;
				sudoku[possibleRow[curVal]][col] = curVal+1;

				potentialColVals[col] -= decToBit(curVal+1);
				potentialRowVals[possibleRow[curVal]] -= decToBit(curVal+1);
				potentialBoxVals[getBoxNumber(possibleRow[curVal], col)]
					-= decToBit(curVal+1);
			}
		}
	}

	return madeChange;
}

bool checkRow(short * potentialColVals, short * potentialRowVals,
							short * potentialBoxVals, Sudoku & sudoku){

	bool madeChange = false;

	// Iterate through each row, one at a time
	for(short row = 0; row < 9; row++){

		short numInstances[9] = {0,0,0,0,0,0,0,0,0};
		short possibleCol[9] = {0,0,0,0,0,0,0,0,0};

		// Iterate through every number 1-9, and get how many
		// spots this number can be placed in this row
		for(short col = 0; col < 9; col++){
			if(sudoku[row][col] < 1){
				short newBit = potentialColVals[col] &
				potentialRowVals[row] &
				potentialBoxVals[getBoxNumber(row, col)];

				for(short bit = 0; bit < 9; bit++){
					if(newBit & decToBit(bit+1)){
						numInstances[bit] ++;
						possibleCol[bit] = col;
					}
				}
			}
		}

		// Now that we've counted the possible locations, find all the
		// values that have only one possible place
		for(short curVal = 0; curVal < 9; curVal++){
			if(numInstances[curVal] == 1){
				madeChange = true;
				sudoku[row][possibleCol[curVal]] = curVal+1;

				potentialColVals[possibleCol[curVal]] -= decToBit(curVal+1);
				potentialRowVals[row] -= decToBit(curVal+1);
				potentialBoxVals[getBoxNumber(row, possibleCol[curVal])]
					-= decToBit(curVal+1);
			}
		}
	}

	return madeChange;
}

bool checkBox(short * potentialColVals, short * potentialRowVals,
							short * potentialBoxVals, Sudoku & sudoku){

	bool madeChange = false;

	// Iterate through each box, one at a time
	for(short box = 0; box < 9; box++){
		short numInstances[9] = {0,0,0,0,0,0,0,0,0};
		short possibleEntry[9] = {0,0,0,0,0,0,0,0,0};

		// Iterate through every entry 1-9, and get how many
		// spots this number can be placed in this row
		for(short entry = 0; entry < 9; entry++){
			if(sudoku[getBoxEntryRow(box, entry)][getBoxEntryCol(box, entry)] < 1){
				short newBit = potentialColVals[getBoxEntryCol(box, entry)] &
				potentialRowVals[getBoxEntryRow(box, entry)] &
				potentialBoxVals[box];

				for(short bit = 0; bit < 9; bit++){
					if(newBit & decToBit(bit+1)){
						numInstances[bit] ++;
						possibleEntry[bit] = entry;
					}
				}
			}
		}

		// Now that we've counted the possible locations, find all the
		// values that have only one possible place
		for(short curVal = 0; curVal < 9; curVal++){
			if(numInstances[curVal] == 1){
				madeChange = true;
				sudoku[getBoxEntryRow(box, possibleEntry[curVal])]
					[getBoxEntryCol(box, possibleEntry[curVal])]
					= curVal+1;

				potentialColVals[getBoxEntryCol(box, possibleEntry[curVal])]
					-= decToBit(curVal+1);
				potentialRowVals[getBoxEntryRow(box, possibleEntry[curVal])]
					-= decToBit(curVal+1);
				potentialBoxVals[box] -= decToBit(curVal+1);
			}
		}
	}

	return madeChange;
}


bool sudokuSolverInner(short * potentialColVals, short * potentialRowVals,
												short * potentialBoxVals, Sudoku & sudoku){

	// Make a copy of the current board state so that we can
	// revert if this recursion fails
	Sudoku saveBoard;
	short saveColVals[9];
	short saveRowVals[9];
	short saveBoxVals[9];
	copySudoku(sudoku, saveBoard);
	copyArray(potentialColVals, saveColVals, 9);
	copyArray(potentialRowVals, saveRowVals, 9);
	copyArray(potentialBoxVals, saveBoxVals, 9);

	// Make as many changes using logic as possible
	bool madeChange = false;
	do{
		madeChange = checkCol(potentialColVals, potentialRowVals, potentialBoxVals, sudoku);
		madeChange |= checkRow(potentialColVals, potentialRowVals, potentialBoxVals, sudoku);
		madeChange |= checkBox(potentialColVals, potentialRowVals, potentialBoxVals, sudoku);

	} while(madeChange);

	// Check if we've solved it
	// Note: Unrolled for performance increase.
	// The RobotC compiler isn't the greatest at optimizing

	//bool isSolved = checkSolved(sudoku);
	bool isSolved = ((potentialColVals[0] |
	potentialColVals[1] |
	potentialColVals[2] |
	potentialColVals[3] |
	potentialColVals[4] |
	potentialColVals[5] |
	potentialColVals[6] |
	potentialColVals[7] |
	potentialColVals[8]) == 0);

	// If logic can't get us any further and the puzzle still isn't solved, we
	// need to try a little bit of trial-&-error / backtracking.

	// Find the first empty cell
	bool gotCell = false;
	for(short row = 0; row < 9 && !isSolved && !gotCell; row++){
		for(short col = 0; col < 9 && !isSolved && !gotCell; col++){
			if(sudoku[row][col] == 0){
				gotCell = true;

				// Get the potential vals that can go here
				short potVals = potentialColVals[col] &
				potentialRowVals[row] &
				potentialBoxVals[getBoxNumber(row, col)];

				// For each potential val...
				for(short curVal = 1; curVal <= 9 && !isSolved; curVal++){
					if(decToBit(curVal) & potVals){

						// Sub in the val and RECURSION
						sudoku[row][col] = curVal;
						potentialRowVals[row] -= decToBit(curVal);
						potentialColVals[col] -= decToBit(curVal);
						potentialBoxVals[getBoxNumber(row, col)] -= decToBit(curVal);

						isSolved = sudokuSolverInner(potentialColVals, potentialRowVals,
																					potentialBoxVals, sudoku);

						// If it's solved, great! Exit. If not, reset this val and try the next one.
						if(!isSolved){
							sudoku[row][col] = 0;
							potentialColVals[col] += decToBit(curVal);
							potentialRowVals[row] += decToBit(curVal);
							potentialBoxVals[getBoxNumber(row, col)] += decToBit(curVal);
						}
					}
				}
			}
		}
	}

	if(!isSolved){
		copySudoku(saveBoard, sudoku);
		copyArray(saveColVals, potentialColVals, 9);
		copyArray(saveRowVals, potentialRowVals, 9);
		copyArray(saveBoxVals, potentialBoxVals, 9);

	}

	return isSolved;
}

bool sudokuSolver(Sudoku & sudoku){
	if(!checkValid(sudoku))
		return false;


	short potentialColVals[9] = {0b111111111, 0b111111111, 0b111111111,
		0b111111111, 0b111111111, 0b111111111,
		0b111111111, 0b111111111, 0b111111111};
	short potentialRowVals[9] = {0b111111111, 0b111111111, 0b111111111,
		0b111111111, 0b111111111, 0b111111111,
		0b111111111, 0b111111111, 0b111111111};
	short potentialBoxVals[9] = {0b111111111, 0b111111111, 0b111111111,
		0b111111111, 0b111111111, 0b111111111,
		0b111111111, 0b111111111, 0b111111111};

	// Determine which vals are not used in each row, col, and box
	for(short row = 0; row < 9; row++){
		for(short col = 0; col < 9; col++){
			short cellBit = decToBit(sudoku[row][col]);
			potentialColVals[col] -= cellBit;
			potentialRowVals[row] -= cellBit;
			potentialBoxVals[getBoxNumber(row, col)] -= cellBit;
		}
	}
	return sudokuSolverInner(potentialColVals,	potentialRowVals, potentialBoxVals, sudoku);
}

task main()
{
	Sudoku sudoku;
	setupBluetooth();

	displayCenteredTextLine(0, "Waiting for");
	displayCenteredTextLine(1, "connection...");

	// Wait until we are connected
	while(nBTCurrentStreamIndex < 0);
	eraseDisplay();
	displayCenteredTextLine(0, "Connected!");

	bool solved = false;
	while(nBTCurrentStreamIndex >= 0){


		displayCenteredTextLine(6, "Waiting for");
		displayCenteredTextLine(7, "next puzzle...");

		BT_Status status = receivePuzzle(sudoku, solved, -1);
		eraseDisplay();

		if(status == BT_SUCCESS){
			displayCenteredTextLine(0, "Recieved puzzle!");
			displayCenteredTextLine(2, "Solving...");

			time1[T1] = 0;
			solved = sudokuSolver(sudoku);
			displayCenteredTextLine(0, "Done (%ds)", time1[T1]/1000);
			if(solved)
				displayCenteredTextLine(2, "Solved. Sending...");
			else
				displayCenteredTextLine(2, "No sol. Sending...");
			status = sendPuzzle(sudoku, solved);
			if(status == BT_SUCCESS){
				displayCenteredTextLine(0, "Sent solution!");
				displayCenteredTextLine(2, "");
			}
			else{
				displayCenteredTextLine(0, "Error sending");
				displayCenteredTextLine(2, "solution:");
				displayCenteredTextLine(4, getStatusMessage(status));
			}
		}
		else{
			displayCenteredTextLine(0, "Error receiving puzzle");
			displayCenteredTextLine(2, getStatusMessage(status));
		}
	}
}
