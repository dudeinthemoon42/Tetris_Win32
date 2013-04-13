#ifndef _PIECE_H
#define _PIECE_H

#include <windows.h>

class Piece
{
	public:
		//constructors
		Piece();
		Piece(int width, COLORREF color);
		
		//functions
		void display(HDC hdc, int left, int top, int xShift, int yShift);
		
		void setActive(bool setto);
		void setExists(bool setto);
		void setScrub(bool setto);
		void setColor(COLORREF setto);
		COLORREF colorspawn();
		
		//accessors
		bool getActive();
		bool getExists();
		bool getScrub();
		COLORREF getColor();
	
	private:
		int myWidth;
		COLORREF myColor;
		
		bool myActive;
		bool myExists;
		
		bool myScrub;
};

#endif