#include "Board.h"
#include "Piece.h"
#include "apmatrix.h"
#include <time.h>

//non-member function

unsigned time_seed(); //this function properly seeds the generator by avoiding a possible issue with converting
//a type time_t to an unsigned int (because time_t is a restricted type) - note that it is not a huge issue but has slight probability of the problem

Board::Board()
{
	myLeft = 0;
	myTop = 0;

	myWidth = 10;
	myHeight = 10;
	myBoard.resize(myWidth, myHeight);

	myCell = 20;

	for (int i = 0; i < myWidth; i++)
	{
		for (int j = 0; j < myHeight; j++)
		{
			myBoard[i][j] = Piece(myCell, RGB(255, 0, 255));
			myBoard[i][j].setExists(false);
			myBoard[i][j].setActive(false);
		}
	}
}

Board::Board(int numCols, int numRows, int cellW, int lef, int top)
{
	myLeft = lef;
	myTop = top;

	myWidth = numCols;
	myHeight = numRows;
	myBoard.resize(myWidth, myHeight);

	myCell = cellW;


	for (int i = 0; i < myWidth; i++)
	{
		for (int j = 0; j < myHeight; j++)
		{
			myBoard[i][j] = Piece(myCell, RGB(255, 0, 255));
			//myBoard[i][j].setExists(false);
			//myBoard[i][j].setActive(false);

			/*if (i==6 && (j == 10 || j == 11))
			{
				myBoard[i][j].setExists(true);
				myBoard[i][j].setActive(true);
			}
			else*/
			{
				myBoard[i][j].setExists(false);
				myBoard[i][j].setActive(false);
			}
			/*
			if (j == myHeight / 4)
			{
				myBoard[i][j] = Piece(myCell, RGB(0, 255, 0));
				myBoard[i][j].setExists(true);
				myBoard[i][j].setActive(false);
			}*/

		}
	}
}


//functions
void Board::display(HDC hdc)
{
	for (int i = 0; i < myWidth; i++)
	{
		for (int j = 0; j < myHeight; j++)
		{
			myBoard[i][j].display(hdc, i, myHeight - j - 1, myLeft, myTop);
		}
	}
}

void Board::update()
{
	//check if should be updated

	bool update = true;

	for (int i = 0; i < myWidth; i++)
	{
		for (int j = 1; j < myHeight; j++)
		{
			if (myBoard[i][j].getActive() && myBoard[i][j-1].getExists() && !myBoard[i][j-1].getActive())
			{
				update = false;
				break;
			}
		}
		if (myBoard[i][0].getActive()) //if the piece has reached the bottom boundary
		{
			update = false;
			break;
		}
	}


	if (update)
	{
		for (int i = 0; i < myWidth; i++)
		{
			for (int j = 0; j < myHeight - 1; j++)
			{
				if (myBoard[i][j+1].getActive()) //if the piece above is active
			     {
			      	myBoard[i][j] = myBoard[i][j+1]; //then the piece below, actually being checked, is now painted
			      	myBoard[i][j].setScrub(true);
			      	myBoard[i][j+1].setExists(false);
			      	myBoard[i][j+1].setScrub(true);
			     }
			}
			/*if (myBoard[i][j].getActive())
			{
				myBoard[i][j] = Piece(myCell, RGB(0,0,0));
				myBoard[i][j].setExists(false);
				myBoard[i][j].setScrub(true);
			}*/
		}

	}
	else
	{
		for (int i = 0; i < myWidth; i++)
		{
			for (int j = 0; j < myHeight; j++)
			{
				myBoard[i][j].setActive(false);
			}
		}
	}


	/*
	for (int i = 0; i < myWidth; i++)
	{
		for (int j = 0; j < myHeight-1; j++)
		{
			myBoard[i][j] = myBoard[i][j+1];

		}
	}*/

}

void Board::spinRight()
{
	//check if there's anything to rotate
	bool rot = false;
	int cy = 0; //center of piece - y coordinate
	int yc = 0;

	int cx = 0; //center of piece - x coordinate
	int xc = 0;

	int avx;
	int avy;

	for (int i = 0; i < myWidth; i++)
	{
		for (int j = 0; j < myHeight; j++)
		{
			if (myBoard[i][j].getActive())
			{
				rot = true;//if there are any active pieces, then try to rotate them
				cy += j;
				yc++;

				cx += i; //x and y centers are equal to their position in matrix
				xc++;
			}
		}
	}
	//get center
	int numP = 0;
	apmatrix <int> live (0, 4);

	if (rot)
	{
		avy = cy/yc; //center of structure divided by the number of structures
		avx = cx/xc;
		//see if it can rotate
		for (int i = 0; i < myWidth && rot; i++)
		{
			for (int j = 0; j < myHeight && rot; j++)
			{
				if (myBoard[i][j].getActive())//piece is active
				{
					numP++;
					live.resize(numP, 4);
					//find new location
					int newx, newy;
					newx = (j-avy)*-1 + avx; //*-1 makes the negative number positive, then add avx
					newy = (i-avx) + avy;
					live[numP-1][0] = i;
					live[numP-1][1] = j;
					live[numP-1][2] = newx;
					live[numP-1][3] = newy;
					if (newx < 0 || newx >= myWidth ||
						newy < 0 || newy >= myHeight ||
						(myBoard[newx][newy].getExists() && !myBoard[newx][newy].getActive())
						)
					{
						rot = false;
					}
				}
			}
		}

		if (rot)//if it's all okay to rotate, go ahead and do it.
		{
			COLORREF color;
			color = myBoard[live[0][0]][live[0][1]].getColor();

			for (int k = 0; k<numP; k++)
			{
				myBoard[live[k][0]][live[k][1]].setExists(false);
			}
			for (int k = 0; k<numP; k++)
			{
				myBoard[live[k][2]][live[k][3]] = Piece(myCell, color);
				myBoard[live[k][2]][live[k][3]].setExists(true);
				myBoard[live[k][2]][live[k][3]].setActive(true);
			}
			for (int k = 0; k<numP; k++)
			{
				myBoard[live[k][0]][live[k][1]].setScrub(true);
				myBoard[live[k][2]][live[k][3]].setScrub(true);
			}

		}
	}

	//rotate
}
void Board::moveleft()
 {

  bool update = true;

  for (int i = 1; i < myWidth; i++)
  {
   for (int j = 0; j < myHeight; j++)
   {
    if (myBoard[i][j].getActive() && myBoard[i-1][j].getExists() && !myBoard[i-1][j].getActive())
    {
     update = false;
     break;
    }

    if (myBoard[0][j].getActive()) //if it's next to the boundary, it can't move left off screen
    {
     update = false;
     break;
    }
   }
  }


  if (update)
  {
   for (int j = 0; j < myHeight; j++)
   {
    for (int i = 1; i < myWidth; i++)
    {
     if (myBoard[i][j].getActive())
     {
      myBoard[i-1][j] = myBoard[i][j];
      myBoard[i-1][j].setScrub(true);
      myBoard[i][j].setExists(false);
      myBoard[i][j].setScrub(true);
     }
    }
    if (myBoard[myWidth-1][j].getActive()) //not sure as to the purpose of this loop
	{
		myBoard[myWidth-1][j] = Piece(myCell, RGB(0,0,0));
		myBoard[myWidth-1][j].setExists(false);
		myBoard[myWidth-1][j].setScrub(true);
	}
   }

  }
 }

 void Board::moveright()
 {
  bool update = true;

   for (int i = myWidth-2; i >= 0; i--)
   {
    for (int j = 0; j < myHeight; j++)
    {
     if (myBoard[i][j].getActive() && myBoard[i+1][j].getExists() && !myBoard[i+1][j].getActive())
     {
      update = false;
      break;
     }

     if (myBoard[myWidth-1][j].getActive()) //can't move a piece to the right if next to the border..
     {
      update = false;
      break;
     }
    }

   }


   if (update)
   {
    for (int j = 0; j < myHeight; j++)
    {
     for (int i = myWidth-2; i >= 0; i--)
     {
     if (myBoard[i][j].getActive())
     {
      myBoard[i+1][j] = myBoard[i][j];
      myBoard[i+1][j].setScrub(true);
      myBoard[i][j].setExists(false);
      myBoard[i][j].setScrub(true);
     }
     }
     if (myBoard[0][j].getActive())
	{
		myBoard[0][j] = Piece(myCell, RGB(0,0,0));
		myBoard[0][j].setExists(false);
		myBoard[0][j].setScrub(true);
	}
    }

   }

 }

void Board::spawn(char type, COLORREF color)
{
 bool check = true;

 for(int i = 0; i < myWidth; i++)
 {
  for(int j = 0; j < myHeight; j++)
  {
   if(myBoard[i][j].getActive())
    check = false;
  }
 } //check entire board to see if there's an active piece

  if(check)
 {

  if(type == 'o')
  {
   int i = myWidth / 2;
   int j = myHeight - 2;
   if(!myBoard[i][j].getExists() || !myBoard[i][j-1].getExists())
   {
    color = myBoard[i][j].colorspawn();
    myBoard[i][j] = Piece(myCell, color);
    myBoard[i][j].setActive(true);
    myBoard[i][j].setExists(true);
    myBoard[i][j].setScrub(true);

    myBoard[i+1][j] = Piece(myCell, color);
    myBoard[i+1][j].setActive(true);
    myBoard[i+1][j].setExists(true);
    myBoard[i+1][j].setScrub(true);

    myBoard[i][j+1] = Piece(myCell, color);
    myBoard[i][j+1].setActive(true);
    myBoard[i][j+1].setExists(true);
    myBoard[i][j+1].setScrub(true);

    myBoard[i+1][j+1] = Piece(myCell, color);
    myBoard[i+1][j+1].setActive(true);
    myBoard[i+1][j+1].setExists(true);
    myBoard[i+1][j+1].setScrub(true);
   }
  }
  if(type == 'l')
  {
   int i = myWidth / 2;
   int j = myHeight - 4;
   if(!myBoard[i][j].getExists() || !myBoard[i][j-1].getExists())
   {
    color = myBoard[i][j].colorspawn();
     myBoard[i][j] = Piece(myCell, color);
     myBoard[i][j].setActive(true);
     myBoard[i][j].setExists(true);
     myBoard[i][j].setScrub(true);

     myBoard[i+1][j] = Piece(myCell, color);
      myBoard[i+1][j].setActive(true);
     myBoard[i+1][j].setExists(true);
     myBoard[i+1][j].setScrub(true);

     myBoard[i][j+1] = Piece(myCell, color);
     myBoard[i][j+1].setActive(true);
     myBoard[i][j+1].setExists(true);
     myBoard[i][j+1].setScrub(true);

     myBoard[i][j+2] = Piece(myCell, color);
     myBoard[i][j+2].setActive(true);
     myBoard[i][j+2].setExists(true);
     myBoard[i][j+2].setScrub(true);
    }
   }
  if (type == 'j')
  {
   int  i = myWidth / 2;
   int j= myHeight - 3;
   if(!myBoard[i][j].getExists() || !myBoard[i][j-1].getExists())
   {
    color = myBoard[i][j].colorspawn();
     myBoard[i][j] = Piece(myCell, color);
     myBoard[i][j].setActive(true);
     myBoard[i][j].setExists(true);
     myBoard[i][j].setScrub(true);

     myBoard[i-1][j] = Piece(myCell, color);
     myBoard[i-1][j].setActive(true);
     myBoard[i-1][j].setExists(true);
     myBoard[i-1][j].setScrub(true);

     myBoard[i][j+1] = Piece(myCell, color);
     myBoard[i][j+1].setActive(true);
     myBoard[i][j+1].setExists(true);
     myBoard[i][j+1].setScrub(true);

     myBoard[i][j+2] = Piece(myCell, color);
     myBoard[i][j+2].setActive(true);
     myBoard[i][j+2].setExists(true);
     myBoard[i][j+2].setScrub(true);
    }
   }
  if (type == 'i')
  {
   int i = myWidth / 2;
   int j = myHeight - 4;
   if(!myBoard[i][j].getExists() || !myBoard[i][j-1].getExists())
   {
    color = myBoard[i][j].colorspawn();
      myBoard[i][j] = Piece(myCell, color);
      myBoard[i][j].setActive(true);
      myBoard[i][j].setExists(true);
      myBoard[i][j].setScrub(true);

      myBoard[i][j+1] = Piece(myCell, color);
      myBoard[i][j+1].setActive(true);
      myBoard[i][j+1].setExists(true);
      myBoard[i][j+1].setScrub(true);

      myBoard[i][j+2] = Piece(myCell, color);
      myBoard[i][j+2].setActive(true);
      myBoard[i][j+2].setExists(true);
      myBoard[i][j+2].setScrub(true);

      myBoard[i][j+3] = Piece(myCell, color);
      myBoard[i][j+3].setActive(true);
      myBoard[i][j+3].setExists(true);
      myBoard[i][j+3].setScrub(true);
     }
    }
  if (type == 't')
  {
   int i = myWidth / 2;
   int j = myHeight - 3;
   if(!myBoard[i][j].getExists() || !myBoard[i][j-1].getExists())
   {
    color = myBoard[i][j].colorspawn();
      myBoard[i][j] = Piece(myCell, color);
      myBoard[i][j].setActive(true);
      myBoard[i][j].setExists(true);
      myBoard[i][j].setScrub(true);

      myBoard[i+1][j] = Piece(myCell, color);
      myBoard[i+1][j].setActive(true);
      myBoard[i+1][j].setExists(true);
      myBoard[i+1][j].setScrub(true);

      myBoard[i-1][j] = Piece(myCell, color);
      myBoard[i-1][j].setActive(true);
      myBoard[i-1][j].setExists(true);
      myBoard[i-1][j].setScrub(true);

      myBoard[i][j+1] = Piece(myCell, color);
      myBoard[i][j+1].setActive(true);
      myBoard[i][j+1].setExists(true);
      myBoard[i][j+1].setScrub(true);
     }
    }
    if (type == 'z')
  {
   int i = myWidth / 2;
   int j = myHeight - 2;
   if(!myBoard[i][j].getExists() || !myBoard[i][j-1].getExists())
   {
    color = myBoard[i][j].colorspawn();
      myBoard[i+1][j] = Piece(myCell, color);
      myBoard[i+1][j].setActive(true);
      myBoard[i+1][j].setExists(true);
      myBoard[i+1][j].setScrub(true);

      myBoard[i][j+1] = Piece(myCell, color);
      myBoard[i][j+1].setActive(true);
      myBoard[i][j+1].setExists(true);
      myBoard[i][j-1].setScrub(true);

      myBoard[i-1][j+1] = Piece(myCell, color);
      myBoard[i-1][j+1].setActive(true);
      myBoard[i-1][j+1].setExists(true);
      myBoard[i-1][j+1].setScrub(true);

      myBoard[i][j] = Piece(myCell, color);
      myBoard[i][j].setActive(true);
      myBoard[i][j].setExists(true);
      myBoard[i][j].setScrub(true);
     }
    }
    if (type == 's')
  {
   int i = myWidth / 2;
   int j = myHeight - 2;
   if(!myBoard[i][j].getExists() || !myBoard[i][j-1].getExists())
   {
    color = myBoard[i][j].colorspawn();
      myBoard[i][j] = Piece(myCell, color);
      myBoard[i][j].setActive(true);
      myBoard[i][j].setExists(true);
      myBoard[i][j].setScrub(true);

      myBoard[i-1][j] = Piece(myCell, color);
      myBoard[i-1][j].setActive(true);
      myBoard[i-1][j].setExists(true);
      myBoard[i-1][j].setScrub(true);

      myBoard[i][j+1] = Piece(myCell, color);
      myBoard[i][j+1].setActive(true);
      myBoard[i][j+1].setExists(true);
      myBoard[i][j+1].setScrub(true);

      myBoard[i+1][j+1] = Piece(myCell, color);
      myBoard[i+1][j+1].setActive(true);
      myBoard[i+1][j+1].setExists(true);
      myBoard[i+1][j+1].setScrub(true);
     }
    }
 }
}

bool Board::lose()
 {
      for (int i = 0; i < myWidth; i++)
       {
        if (myBoard[i][myHeight - 1].getExists() && !myBoard[i][myHeight-1].getActive())//and is not active
            return false;
       }
 }

char Board::nextPiece(char p)
{
	bool next = false;
	char tor;
	for (int i = 0; i < myWidth && !next; i++)
	{
		for (int j = 0; j < myHeight && !next; j++)
		{
			if (myBoard[i][j].getActive())
				next = true;
		}
	}
	if (next)
	{
	    srand(time(NULL));
	    int i = rand() % 7;
	    //int prev = i; //the previous value
	    //int next;
		/*do{
            i = rand() % 7;  //try to get a proper random number
            next = i;
            //there is always an even distribution of numbers given by rand(),
            //such that any new number generated by rand() has probability 1/P of being
            //a number within range, also known as "uniform distribution"
            //instead of trying to circumvent the distribution, I have decided to work with it until a number falls in range

		}while((i < 0 || i > 7) &&  next == prev); //this way we never have pieces outside of the range, and the pieces never duplicate*/

		switch (i)
		{
			case 0:
				tor = 'o';
				break;
			case 1:
				tor = 'l';
				break;
			case 2:
				tor = 'j';
				break;
			case 3:
				tor = 'i';
				break;
			case 4:
				tor = 't';
				break;
			case 5:
				tor = 'z';
				break;
			case 6:
				tor = 's';
				break;
		}
	}
	else
		tor = p;

	return tor;
}

void Board::scorePoints()
{
    bool check = true;
    COLORREF colorcheck1, colorcheck2, colorcheck3, colorcheck4;
    for(int i = 0; i < myWidth; i++)
    {
        for(int j = 0; j < myHeight; j++)
        {
            if(myBoard[i][j].getActive())
                check = false;
        }
    }

    if(check)
    {
        for(int i = 0; i < myWidth - 4; i++) //mywidth - 4 so that we can check successive pieces from i up to but no greater than i+4, which if it were the
        //rightmost piece, i=16 and i+4=20, which is mywidth, therefore we won't check any pieces beyond the right border line
        {
            for(int j = 0; j < myHeight; j++)
            {
                colorcheck1 = myBoard[i][j].getColor();
                colorcheck2 = myBoard[i+1][j].getColor();
                colorcheck3 = myBoard[i+2][j].getColor();
                colorcheck4 = myBoard[i+3][j].getColor();

                if(colorcheck1 == colorcheck2)
                    if(colorcheck2 == colorcheck3)
                        if(colorcheck3 == colorcheck4)
                        {
                            myBoard[i][j].setExists(false);
                            myBoard[i][j].setScrub(true);
                            myBoard[i+1][j].setExists(false);
                            myBoard[i+1][j].setScrub(true);
                            myBoard[i+2][j].setExists(false);
                            myBoard[i+2][j].setScrub(true);
                            myBoard[i+3][j].setExists(false);
                            myBoard[i+3][j].setScrub(true);

                            myPoints+=100;
                        }
            }
        }
    }
}

int Board::getpoints()
{
    return myPoints;
}

unsigned time_seed()
{
    time_t now = time ( NULL ); //get current time - it could be some random number like 266344, measured in seconds
    unsigned char *p = (unsigned char *)&now; //properly convert time_t * to unsigned char *
    unsigned seed = 0;
    size_t i;
    for ( i = 0; i < sizeof now; i++ )
        seed = seed * ( UCHAR_MAX + 2U ) + p[i]; //hash the bytes
        //hashing is using the key relative to some number
    return seed;
 }
