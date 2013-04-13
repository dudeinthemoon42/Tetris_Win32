#ifndef _BOARD_H
#define _BOARD_H

#include <windows.h>
#include "apmatrix.h"
#include "Piece.h"

class Board
{
	public:
		Board();
		Board(int numCols, int numRows, int cellW, int lef, int top);

		//functions
		void display(HDC hdc);
		void update();

		void spinRight();
		void moveleft();
		void moveright();
		void spawn(char type, COLORREF color);
		bool lose();

		char nextPiece(char p);
		COLORREF colorspawn();

		void scorePoints();
		int getpoints();

	private:
		int myCell;
		int myWidth;
		int myHeight;

		int myLeft;
		int myTop;

		int myPoints;

		apmatrix <Piece> myBoard;

};

#endif
