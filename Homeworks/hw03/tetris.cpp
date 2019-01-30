//File: tetris.cpp

#include <iostream>
#include "tetris.h"

using namespace std;

//Constructor, takes in a width 
Tetris::Tetris(int aWidth)
{
	width = aWidth;

	// Dynamically creates heights array to size of 
	// width entered
	heights = new int[width];
	// Sets all values in heights array to zero
	for (int j = 0; j < width; j++)
	{
		heights[j] = 0;
	}

	// Dynamically creates data array as a 2D array
	data = new char*[width];
	// Sets each array in data array to NULL
	for (int i = 0; i < width; i++)
	{
		data[i] = NULL;
	}
}

// Function adds piece inputted on board, taking in as input
// a piece, the rotation of the piece, and where it should be
// placed
void Tetris::add_piece(char piece, int rotation, int placement)
{
	// Switch case created with piece, with cases for each of the 
	// 7 possible pieces, and a default case at the end if the piece
	// entered is invalid. For each piece, all possible cases are taken
	// into account. Logic to adding a piece in all cases is similar.
	switch(piece)
	{
		// Piece I only has two rotations, 0, which is eqivalent to 
		// 180 and 90, which is equivalent to 270
		case 'I' :
			if (rotation == 0 || rotation == 180)
			{
				if (data[placement] == NULL)
				{
					data[placement] = new char[4];
					heights[placement] = 4;

					for (int i = 0; i < heights[placement]; i++)
					{
						data[placement][i] = piece;
					}
				}

				else
				{
					char* temp = new char[heights[placement] + 4];

					for (int i = 0; i < heights[placement]; i++)
					{
						temp[i] = data[placement][i];
					}

					for (int i = heights[placement]; 
						i < heights[placement] + 4; i++)
					{
						temp[i] = piece;
					}

					heights[placement] += 4;

					delete [] data[placement];
					data[placement] = temp;
					temp = NULL;
				}
			}

			if (rotation == 90 || rotation == 270)
			{
				if (data[placement] == NULL && data[placement+1] == NULL 
					&& data[placement+2] == NULL && data[placement+3] == NULL)
				{
					data[placement] = new char[1];
					data[placement+1] = new char[1];
					data[placement+2] = new char[1];
					data[placement+3] = new char[1];
					
					heights[placement] = 1;
					heights[placement+1] = 1;
					heights[placement+2] = 1;
					heights[placement+3] = 1;
					
					data[placement][0] = piece;
					data[placement+1][0] = piece;
					data[placement+2][0] = piece;
					data[placement+3][0] = piece;
				}

				else
				{
					int h = 0;
					for (int i = placement; i < placement + 4; i++)
					{
						if (heights[i] > h)
						{
							h = heights[i];
						}
					}

					for (int i = placement; i < placement + 4; i++)
					{
						char* temp = new char[h+1];
						if (data[i] != NULL)
						{
							if (heights[i] != h)
							{
								for (int j = heights[i]; j < h; j++)
								{
									temp[j] = ' ';
								}
							}

							for (int j = 0; j < h; j++)
							{
								if (heights[i] < h && j >= heights[i])
								{
									temp[j] = ' ';
								}
								else
								{
									temp[j] = data[i][j];
								}
								
							}

							temp[h] = piece;
						}
						else
						{
							for (int j = 0; j < h; j++)
							{
								temp[j] = ' ';
							}

							temp[h] = piece;

						}
						
						heights[i] = h+1;
						delete [] data[i];
						data[i] = temp;
						temp = NULL;
					}
				}
			}
			break;

		// Case O has only one possible case for rotation.
		case 'O' :
			//Creates new arrays if the arrays that the circle is being
			//added to is NULL
			if (data[placement] == NULL && data[placement + 1] == NULL)
			{
				data[placement] = new char[2];
				data[placement+1] = new char[2];

				heights[placement] = 2;
				heights[placement+1] = 2;
				
				data[placement][0] = piece;
				data[placement][1] = piece;
				data[placement+1][0] = piece;
				data[placement+1][1] = piece;
			}
			//If not...
			else
			{
				//Find max height of arrays that circle will be added to
				int h = 0;
				for (int i = placement; i < placement + 2; i++)
				{
					if (heights[i] > h)
					{
						h = heights[i];
					}
				}

				//Loop through the two arrays, create new temp arrays, and 
				//add two to their heights, then make the two new slots
				//equal to the piece 'O' and set the old array equal to the
				//new temp array, deleting the old one before doing this
				//and adjusting the height accordingly
				for (int i = placement; i < placement + 2; i++)
				{
					char* temp = new char[h+2];
					if (data[i] != NULL)
					{
						if (heights[i] != h)
						{
							for (int j = heights[i]; j < h; j++)
							{
								temp[j] = ' ';
							}
						}

						for (int j = 0; j < heights[i]; j++)
						{
							temp[j] = data[i][j];
						}

						temp[h] = piece;
						temp[h+1] = piece;
					}

					else
					{
						for (int j = 0; j < h; j++)
						{
							temp[j] = ' ';
						}

						temp[h] = piece;
						temp[h+1] = piece;
					}
					
					heights[i] = h+2;
					delete [] data[i];
					data[i] = temp;
					temp = NULL;
				}
			}

			break;

		//'T' has four cases, as all rotations are different.
		case 'T' :
			if (rotation == 0)
			{
				if (placement >= width - 2)
				{
					cerr << "Placement entered is invalid." << endl;
					break;
				}

				if (data[placement] == NULL && data[placement+1] == NULL 
					&& data[placement+2] == NULL)
				{
					data[placement] = new char[2];
					data[placement+1] = new char[2];
					data[placement+2] = new char[2];

					heights[placement] = 2;
					heights[placement+1] = 2;
					heights[placement+2] = 2;

					data[placement][0] = ' ';
					data[placement][1] = piece;
					data[placement+1][0] = piece;
					data[placement+1][1] = piece;
					data[placement+2][0] = ' ';
					data[placement+2][1] = piece;
				}
				else
				{
					int h = 0;
					for (int i = placement; i < placement + 3; i++)
					{
						if (heights[i] > h)
						{
							h = heights[i];
						}
					}

					if (heights[placement+1] < h)
					{
						for (int i = placement; i < placement + 3; i++)
						{
							char* temp = new char[h+1];
							if (data[i] != NULL)
							{
								if (heights[i] != h)
								{
									for (int j = heights[i]; j < h; j++)
									{
										temp[j] = ' ';
									}
								}

								for (int j = 0; j < heights[i]; j++)
								{
									temp[j] = data[i][j];
								}

								temp[h] = piece;

								if (i == placement + 1)
								{
									temp[h-1] = piece;
								}
							}
							else
							{
								for (int j = 0; j < h; j++)
								{
									temp[j] = ' ';
								}

								temp[h] = piece;

								if (i == placement + 1)
								{
									temp[h-1] = piece;
								}
							}
							
							heights[i] = h+1;
							delete [] data[i];
							data[i] = temp;
							temp = NULL;
						}
					}
					else
					{
						for (int i = placement; i < placement + 3; i++)
						{
							char* temp = new char[h+2];
							if (data[i] != NULL)
							{
								if (heights[i] != h)
								{
									for (int j = heights[i]; j < h; j++)
									{
										temp[j] = ' ';
									}
								}

								for (int j = 0; j < heights[i]; j++)
								{
									temp[j] = data[i][j];
								}

								temp[h+1] = piece;

								if (i == placement + 1)
								{
									temp[h] = piece;
								}
								else
								{
									temp[h] = ' ';
								}
							}
							else
							{
								for (int j = 0; j < h; j++)
								{
									temp[j] = ' ';
								}

								temp[h+1] = piece;

								if (i == placement + 1)
								{
									temp[h] = piece;
								}
								else
								{
									temp[h] = ' ';
								}
							}
							
							heights[i] = h+2;
							delete [] data[i];
							data[i] = temp;
							temp = NULL;
						}
					}
				}
			}

			if (rotation == 270)
			{
				if (data[placement] == NULL && data[placement+1] == NULL)
				{
					data[placement] = new char[3];
					data[placement+1] = new char[2];

					heights[placement] = 3;
					heights[placement+1] = 2;

					data[placement][0] = piece;
					data[placement][1] = piece;
					data[placement][2] = piece;
					data[placement+1][0] = ' ';
					data[placement+1][1] = piece;
				}
				else
				{
					int h = 0;
					for (int i = placement; i < placement + 3; i++)
					{
						if (heights[i] > h)
						{
							h = heights[i];
						}
					}

					if (heights[placement] < h)
					{
						char* temp = new char[h+2];
						if (data[placement] != NULL)
						{
							for (int j = 0; j < heights[placement]; j++)
							{
								temp[j] = data[placement][j];
							}
						}
						else
						{
							for (int j = 0; j < heights[placement]; j++)
							{
								temp[j] = ' ';
							}
						}

						temp[h-1] = piece;
						temp[h] = piece;
						temp[h+1] = piece;

						heights[placement] = h+2;
						delete [] data[placement];
						data[placement] = temp;
						temp = NULL;

						temp = new char[h+1];
						if (data[placement+1] != NULL)
						{
							for (int j = 0; j < heights[placement+1]; j++)
							{
								temp[j] = data[placement+1][j];
							}
						}
						else
						{
							for (int j = 0; j < heights[placement+1]; j++)
							{
								temp[j] = ' ';
							}
						}

						temp[h] = piece;

						heights[placement+1] = h+1;
						delete [] data[placement+1];
						data[placement+1] = temp;
						temp = NULL;
					}
					else
					{
						char* temp = new char[h+3];
						if (data[placement] != NULL)
						{
							for (int j = 0; j < heights[placement]; j++)
							{
								temp[j] = data[placement][j];
							}
						}
						else
						{
							for (int j = 0; j < heights[placement]; j++)
							{
								temp[j] = ' ';
							}
						}

						temp[h] = piece;
						temp[h+1] = piece;
						temp[h+2] = piece;

						heights[placement] = h+3;
						delete [] data[placement];
						data[placement] = temp;
						temp = NULL;

						temp = new char[h+2];
						if (data[placement+1] != NULL)
						{
							for (int j = 0; j < heights[placement+1]; j++)
							{
								temp[j] = data[placement+1][j];
							}
						}
						else
						{
							for (int j = 0; j < heights[placement+1]; j++)
							{
								temp[j] = ' ';
							}
						}

						temp[h] = ' ';
						temp[h+1] = piece;

						heights[placement+1] = h+2;
						delete [] data[placement+1];
						data[placement+1] = temp;
						temp = NULL;
					}
				}
			}

			if (rotation == 180)
			{
				if (placement >= width - 2)
				{
					cerr << "Placement entered is invalid." << endl;
					break;
				}

				if (data[placement] == NULL && data[placement+1] == NULL 
					&& data[placement+2] == NULL)
				{
					data[placement] = new char[1];
					data[placement+1] = new char[2];
					data[placement+2] = new char[1];

					heights[placement] = 1;
					heights[placement+1] = 2;
					heights[placement+2] = 1;

					data[placement][0] = piece;
					data[placement+1][0] = piece;
					data[placement+1][1] = piece;
					data[placement+2][0] = piece;
				}
				else
				{
					int h = 0;
					for (int i = placement; i < placement + 3; i++)
					{
						if (heights[i] > h)
						{
							h = heights[i];
						}
					}

					for (int i = placement; i < placement + 3; i++)
					{
						if(i == placement + 1)
						{
							char* temp = new char[h+2];
							if (data[i] != NULL)
							{
								if (heights[i] != h)
								{
									for (int j = heights[i]; j < h; j++)
									{
										temp[j] = ' ';
									}
								}

								for (int j = 0; j < heights[i]; j++)
								{
									temp[j] = data[i][j];
								}

								temp[h] = piece;
								temp[h+1] = piece;
							}
							else
							{
								for (int j = 0; j < h; j++)
								{
									temp[j] = ' ';
								}

								temp[h] = piece;
								temp[h+1] = piece;
							}
							
							heights[i] = h+2;
							delete [] data[i];
							data[i] = temp;
							temp = NULL;
						}
						else
						{
							char* temp = new char[h+1];
							if (data[i] != NULL)
							{
								if (heights[i] != h)
								{
									for (int j = heights[i]; j < h; j++)
									{
										temp[j] = ' ';
									}
								}

								for (int j = 0; j < heights[i]; j++)
								{
									temp[j] = data[i][j];
								}

								temp[h] = piece;
							}
							else
							{
								for (int j = 0; j < h; j++)
								{
									temp[j] = ' ';
								}

								temp[h] = piece;
							}
							
							heights[i] = h+1;
							delete [] data[i];
							data[i] = temp;
							temp = NULL;
						}
					}
				}
			}

			if (rotation == 90)
			{
				if (data[placement] == NULL && data[placement+1] == NULL)
				{
					data[placement] = new char[2];
					data[placement+1] = new char[3];

					heights[placement] = 2;
					heights[placement+1] = 3;

					data[placement][0] = ' ';
					data[placement][1] = piece;
					data[placement+1][0] = piece;
					data[placement+1][1] = piece;
					data[placement+1][2] = piece;
				}
				else
				{
					int h = 0;
					for (int i = placement; i < placement + 1; i++)
					{
						if (heights[i] > h)
						{
							h = heights[i];
						}
					}

					if (heights[placement+1] < h)
					{
						char* temp = new char[h+2];
						if (data[placement+1] != NULL)
						{
							for (int j = 0; j < heights[placement+1]; j++)
							{
								temp[j] = data[placement+1][j];
							}
						}
						else
						{
							for (int j = 0; j < heights[placement+1]; j++)
							{
								temp[j] = ' ';
							}
						}

						temp[h-1] = piece;
						temp[h] = piece;
						temp[h+1] = piece;

						heights[placement+1] = h+2;
						delete [] data[placement+1];
						data[placement+1] = temp;
						temp = NULL;

						temp = new char[h+1];
						if (data[placement] != NULL)
						{
							for (int j = 0; j < heights[placement]; j++)
							{
								temp[j] = data[placement][j];
							}
						}
						else
						{
							for (int j = 0; j < heights[placement]; j++)
							{
								temp[j] = ' ';
							}
						}

						temp[h] = piece;

						heights[placement] = h+1;
						delete [] data[placement];
						data[placement] = temp;
						temp = NULL;
					}
					else
					{
						char* temp = new char[h+3];
						if (data[placement+1] != NULL)
						{
							for (int j = 0; j < heights[placement+1]; j++)
							{
								temp[j] = data[placement+1][j];
							}
						}
						else
						{
							for (int j = 0; j < heights[placement+1]; j++)
							{
								temp[j] = ' ';
							}
						}

						temp[h] = piece;
						temp[h+1] = piece;
						temp[h+2] = piece;

						heights[placement+1] = h+3;
						delete [] data[placement+1];
						data[placement+1] = temp;
						temp = NULL;

						temp = new char[h+2];
						if (data[placement] != NULL)
						{
							for (int j = 0; j < heights[placement]; j++)
							{
								temp[j] = data[placement][j];
							}
						}
						else
						{
							for (int j = 0; j < heights[placement]; j++)
							{
								temp[j] = ' ';
							}
						}

						temp[h] = ' ';
						temp[h+1] = piece;

						heights[placement] = h+2;
						delete [] data[placement];
						data[placement] = temp;
						temp = NULL;
					}
				}
			}
			break;

		//For letters 'Z' and 'S', 0 and 180 are equal cases, and same with 
		//90 and 270
		case 'Z' :
			if (rotation == 0 || rotation == 180)
			{
				if (placement >= width - 2)
				{
					cerr << "Placement entered is invalid." << endl;
					break;
				}

				if (data[placement] == NULL && data[placement+1] == NULL 
					&& data[placement+2] == NULL)
				{
					data[placement] = new char[2];
					data[placement+1] = new char[2];
					data[placement+2] = new char[1];

					heights[placement] = 2;
					heights[placement+1] = 2;
					heights[placement+2] = 1;

					data[placement][0] = ' ';
					data[placement][1] = piece;
					data[placement+1][0] = piece;
					data[placement+1][1] = piece;
					data[placement+2][0] = piece;
				}
				else
				{
					int h = 0;
					for (int i = placement; i < placement + 3; i++)
					{
						if (heights[i] > h)
						{
							h = heights[i];
						}
					}

					if (heights[placement+1] < h && heights[placement+2] < h)
					{
						char* temp = new char[h+1];
						if (heights[placement] != h)
						{
							for (int j = heights[placement]; j < h; j++)
							{
								temp[j] = ' ';
							}
						}

						for (int j = 0; j < heights[placement]; j++)
						{
							temp[j] = data[placement][j];
						}

						temp[h] = piece;
					
						heights[placement] = h+1;
						delete [] data[placement];
						data[placement] = temp;
						temp = NULL;

						temp = new char[h+1];
						for (int j = heights[placement+1]; j < h; j++)
						{
							temp[j] = ' ';
						}

						for (int j = 0; j < heights[placement+1]; j++)
						{
							temp[j] = data[placement+1][j];
						}

						temp[h-1] = piece;
						temp[h] = piece;

						heights[placement+1] = h+1;
						delete [] data[placement+1];
						data[placement+1] = temp;
						temp = NULL;

						temp = new char[h];
						for (int j = heights[placement+2]; j < h; j++)
						{
							temp[j] = ' ';
						}

						for (int j = 0; j < heights[placement+2]; j++)
						{
							temp[j] = data[placement+2][j];
						}

						temp[h-1] = piece;

						heights[placement+2] = h;
						delete [] data[placement+2];
						data[placement+2] = temp;
						temp = NULL;
					}
					else
					{
						for (int i = placement; i < placement + 2; i++)
						{
							char* temp = new char[h+2];
							if (data[i] != NULL)
							{
								if (heights[i] != h)
								{
									for (int j = heights[i]; j < h; j++)
									{
										temp[j] = ' ';
									}
								}

								for (int j = 0; j < heights[i]; j++)
								{
									temp[j] = data[i][j];
								}

								temp[h+1] = piece;

								if (i == placement + 1)
								{
									temp[h] = piece;
								}
								else
								{
									temp[h] = ' ';
								}
							}
							else
							{
								for (int j = 0; j < h; j++)
								{
									temp[j] = ' ';
								}

								temp[h+1] = piece;

								if (i == placement + 1)
								{
									temp[h] = piece;
								}
								else
								{
									temp[h] = ' ';
								}
							}
							
							heights[i] = h+2;
							delete [] data[i];
							data[i] = temp;
							temp = NULL;
						}

						char* temp = new char[h+1];
						if (data[placement+2] != NULL)
						{
							if (heights[placement+2] != h)
							{
								for (int j = heights[placement+2]; j < h; j++)
								{
									temp[j] = ' ';
								}
							}

							for (int j = 0; j < heights[placement+2]; j++)
							{
								temp[j] = data[placement+2][j];
							}

							temp[h] = piece;
						}
						else
						{
							for (int j = 0; j < h; j++)
							{
								temp[j] = ' ';
							}

							temp[h] = piece;
						}
						
						heights[placement+2] = h+1;
						delete [] data[placement+2];
						data[placement+2] = temp;
						temp = NULL;
					}
				}
			}

			if (rotation == 90 || rotation == 270)
			{
				if (data[placement] == NULL && data[placement+1] == NULL)
				{
					data[placement] = new char[2];
					data[placement+1] = new char[3];

					heights[placement] = 2;
					heights[placement+1] = 3;

					data[placement][0] = piece;
					data[placement][1] = piece;
					data[placement+1][0] = ' ';
					data[placement+1][1] = piece;
					data[placement+1][2] = piece;
				}
				else
				{
					int h = 0;
					for (int i = placement; i < placement + 3; i++)
					{
						if (heights[i] > h)
						{
							h = heights[i];
						}
					}

					if (heights[placement] < h)
					{
						char* temp = new char[h+1];
						if (data[placement] != NULL)
						{
							for (int j = 0; j < heights[placement]; j++)
							{
								temp[j] = data[placement][j];
							}
						}
						else
						{
							for (int j = 0; j < heights[placement]; j++)
							{
								temp[j] = ' ';
							}
						}

						temp[h-1] = piece;
						temp[h] = piece;

						heights[placement] = h+1;
						delete [] data[placement];
						data[placement] = temp;
						temp = NULL;

						temp = new char[h+2];
						if (data[placement+1] != NULL)
						{
							for (int j = 0; j < heights[placement+1]; j++)
							{
								temp[j] = data[placement+1][j];
							}
						}
						else
						{
							for (int j = 0; j < heights[placement+1]; j++)
							{
								temp[j] = ' ';
							}
						}

						temp[h] = piece;
						temp[h+1] = piece;

						heights[placement+1] = h+2;
						delete [] data[placement+1];
						data[placement+1] = temp;
						temp = NULL;
					}
					else
					{
						char* temp = new char[h+2];
						if (data[placement] != NULL)
						{
							for (int j = 0; j < heights[placement]; j++)
							{
								temp[j] = data[placement][j];
							}
							if (heights[placement] < h)
							{
								for (int j = heights[placement]; j < h; j++)
								{
									temp[j] = ' ';
								}
							}
						}
						else
						{
							for (int j = 0; j < heights[placement]; j++)
							{
								temp[j] = ' ';
							}
						}

						temp[h] = piece;
						temp[h+1] = piece;

						heights[placement] = h+2;
						delete [] data[placement];
						data[placement] = temp;
						temp = NULL;

						temp = new char[h+3];
						if (data[placement+1] != NULL)
						{
							for (int j = 0; j < heights[placement+1]; j++)
							{
								temp[j] = data[placement+1][j];
							}

							if(heights[placement+1] < h)
							{
								for (int j = heights[placement+1]; j < h; j++)
								{
									temp[j] = ' ';
								}
							}
						}
						else
						{
							for (int j = 0; j < heights[placement+1]; j++)
							{
								temp[j] = ' ';
							}
						}

						temp[h] = ' ';
						temp[h+1] = piece;
						temp[h+2] = piece;

						heights[placement+1] = h+3;
						delete [] data[placement+1];
						data[placement+1] = temp;
						temp = NULL;
					}
				}	
			}

			break;

		case 'S' :
			if (rotation == 0 || rotation == 180)
			{
				if (placement >= width - 2)
				{
					cerr << "Placement entered is invalid." << endl;
					break;
				}

				if (data[placement] == NULL && data[placement+1] == NULL 
					&& data[placement+2] == NULL)
				{
					data[placement] = new char[1];
					data[placement+1] = new char[2];
					data[placement+2] = new char[2];

					heights[placement] = 1;
					heights[placement+1] = 2;
					heights[placement+2] = 2;

					data[placement][0] = piece;
					data[placement+1][0] = piece;
					data[placement+1][1] = piece;
					data[placement+2][1] = ' ';
					data[placement+2][1] = piece;
				}
				else
				{
					int h = 0;
					for (int i = placement; i < placement + 3; i++)
					{
						if (heights[i] > h)
						{
							h = heights[i];
						}
					}

					if (heights[placement] < h && heights[placement+1] < h)
					{
						char* temp = new char[h+1];
						if (heights[placement+2] != h)
						{
							for (int j = heights[placement+2]; j < h; j++)
							{
								temp[j] = ' ';
							}
						}

						for (int j = 0; j < heights[placement+2]; j++)
						{
							temp[j] = data[placement+2][j];
						}

						temp[h] = piece;
					
						heights[placement+2] = h+1;
						delete [] data[placement+2];
						data[placement+2] = temp;
						temp = NULL;

						temp = new char[h+1];
						for (int j = heights[placement+1]; j < h; j++)
						{
							temp[j] = ' ';
						}

						for (int j = 0; j < heights[placement+1]; j++)
						{
							temp[j] = data[placement+1][j];
						}

						temp[h-1] = piece;
						temp[h] = piece;

						heights[placement+1] = h+1;
						delete [] data[placement+1];
						data[placement+1] = temp;
						temp = NULL;

						temp = new char[h];
						for (int j = heights[placement]; j < h; j++)
						{
							temp[j] = ' ';
						}

						for (int j = 0; j < heights[placement]; j++)
						{
							temp[j] = data[placement][j];
						}

						temp[h-1] = piece;

						heights[placement] = h;
						delete [] data[placement];
						data[placement] = temp;
						temp = NULL;
					}
					else
					{
						for (int i = placement + 1; i < placement + 3; i++)
						{
							char* temp = new char[h+2];
							if (data[i] != NULL)
							{
								if (heights[i] != h)
								{
									for (int j = heights[i]; j < h; j++)
									{
										temp[j] = ' ';
									}
								}

								for (int j = 0; j < heights[i]; j++)
								{
									temp[j] = data[i][j];
								}

								temp[h+1] = piece;

								if (i == placement + 1)
								{
									temp[h] = piece;
								}
								else
								{
									temp[h] = ' ';
								}
							}
							else
							{
								for (int j = 0; j < h; j++)
								{
									temp[j] = ' ';
								}

								temp[h+1] = piece;

								if (i == placement + 1)
								{
									temp[h] = piece;
								}
								else
								{
									temp[h] = ' ';
								}
							}
							
							heights[i] = h+2;
							delete [] data[i];
							data[i] = temp;
							temp = NULL;
						}

						char* temp = new char[h+1];
						if (data[placement] != NULL)
						{
							if (heights[placement] != h)
							{
								for (int j = heights[placement]; j < h; j++)
								{
									temp[j] = ' ';
								}
							}

							for (int j = 0; j < heights[placement]; j++)
							{
								temp[j] = data[placement][j];
							}

							temp[h] = piece;
						}
						else
						{
							for (int j = 0; j < h; j++)
							{
								temp[j] = ' ';
							}

							temp[h] = piece;
						}
						
						heights[placement] = h+1;
						delete [] data[placement];
						data[placement] = temp;
						temp = NULL;
					}
				}
			}

			if (rotation == 90 || rotation == 270)
			{
				if (data[placement] == NULL && data[placement+1] == NULL)
				{
					data[placement] = new char[3];
					data[placement+1] = new char[2];

					heights[placement] = 3;
					heights[placement+1] = 2;

					data[placement][0] = ' ';
					data[placement][1] = piece;
					data[placement][2] = piece;
					data[placement+1][0] = piece;
					data[placement+1][1] = piece;
				}
				else
				{
					int h = 0;
					for (int i = placement; i < placement + 3; i++)
					{
						if (heights[i] > h)
						{
							h = heights[i];
						}
					}

					if (heights[placement+1] < h)
					{
						char* temp = new char[h+1];
						if (data[placement+1] != NULL)
						{
							for (int j = 0; j < heights[placement+1]; j++)
							{
								temp[j] = data[placement+1][j];
							}
						}
						else
						{
							for (int j = 0; j < heights[placement+1]; j++)
							{
								temp[j] = ' ';
							}
						}

						temp[h-1] = piece;
						temp[h] = piece;

						heights[placement+1] = h+1;
						delete [] data[placement+1];
						data[placement+1] = temp;
						temp = NULL;

						temp = new char[h+2];
						if (data[placement] != NULL)
						{
							for (int j = 0; j < heights[placement]; j++)
							{
								temp[j] = data[placement][j];
							}
						}
						else
						{
							for (int j = 0; j < heights[placement]; j++)
							{
								temp[j] = ' ';
							}
						}

						temp[h] = piece;
						temp[h+1] = piece;

						heights[placement] = h+2;
						delete [] data[placement];
						data[placement] = temp;
						temp = NULL;
					}
					else
					{
						char* temp = new char[h+2];
						if (data[placement+1] != NULL)
						{
							for (int j = 0; j < heights[placement+1]; j++)
							{
								temp[j] = data[placement+1][j];
							}
						}
						else
						{
							for (int j = 0; j < heights[placement+1]; j++)
							{
								temp[j] = ' ';
							}
						}

						temp[h] = piece;
						temp[h+1] = piece;

						heights[placement+1] = h+2;
						delete [] data[placement+1];
						data[placement+1] = temp;
						temp = NULL;

						temp = new char[h+3];
						if (data[placement] != NULL)
						{
							for (int j = 0; j < heights[placement]; j++)
							{
								temp[j] = data[placement][j];
							}
							for (int j = heights[placement]; j < h; j++)
							{
								temp[j] = ' ';
							}
						}
						else
						{
							for (int j = 0; j < heights[placement]; j++)
							{
								temp[j] = ' ';
							}
						}

						temp[h] = ' ';
						temp[h+1] = piece;
						temp[h+2] = piece;

						heights[placement] = h+3;
						delete [] data[placement];
						data[placement] = temp;
						temp = NULL;
					}
				}	
			}
			
			break;

		//Cases L and J both have 4 rotation cases
		case 'L' :
			if (rotation == 0)
			{
				if (data[placement] == NULL && data[placement+1] == NULL)
				{
					data[placement] = new char[3];
					data[placement+1] = new char[1];

					heights[placement] = 3;
					heights[placement+1] = 1;

					data[placement][0] = piece;
					data[placement][1] = piece;
					data[placement][2] = piece;
					data[placement+1][0] = piece;
				}
				else
				{
					int h = 0;
					for (int i = placement; i < placement + 2; i++)
					{
						if (heights[i] > h)
						{
							h = heights[i];
						}
					}

					char* temp = new char[h+3];
					if (data[placement] != NULL)
					{
						for (int j = 0; j < heights[placement]; j++)
						{
							temp[j] = data[placement][j];
						}
						for (int j = heights[placement]; j < h; j++)
						{
							temp[j] = ' ';
						}
					}
					else
					{
						for (int j = 0; j < heights[placement]; j++)
						{
							temp[j] = ' ';
						}
					}

					temp[h] = piece;
					temp[h+1] = piece;
					temp[h+2] = piece;

					heights[placement] = h+3;
					delete [] data[placement];
					data[placement] = temp;
					temp = NULL;

					temp = new char[h+1];
					if (data[placement+1] != NULL)
					{
						for (int j = 0; j < heights[placement+1]; j++)
						{
							temp[j] = data[placement+1][j];
						}
					}
					else
					{
						for (int j = 0; j < heights[placement+1]; j++)
						{
							temp[j] = ' ';
						}
					}

					temp[h] = piece;

					heights[placement+1] = h+1;
					delete [] data[placement+1];
					data[placement+1] = temp;
					temp = NULL;
				}	
			}

			if (rotation == 270)
			{

				if (placement >= width - 2)
				{
					cerr << "Placement entered is invalid." << endl;
					break;
				}

				if (data[placement] == NULL && data[placement+1] == NULL 
					&& data[placement+2] == NULL)
				{
					data[placement] = new char[1];
					data[placement+1] = new char[1];
					data[placement+2] = new char[2];

					heights[placement] = 1;
					heights[placement+1] = 1;
					heights[placement+2] = 2;

					data[placement][0] = piece;
					data[placement+1][0] = piece;
					data[placement+2][0] = piece;
					data[placement+2][1] = piece;
				}
				else
				{
					int h = 0;
					for (int i = placement; i < placement + 3; i++)
					{
						if (heights[i] > h)
						{
							h = heights[i];
						}
					}

					char* temp = new char[h+1];
					if (heights[placement] != h)
					{
						for (int j = heights[placement]; j < h; j++)
						{
							temp[j] = ' ';
						}
					}

					for (int j = 0; j < heights[placement]; j++)
					{
						temp[j] = data[placement][j];
					}

					temp[h] = piece;
				
					heights[placement] = h+1;
					delete [] data[placement];
					data[placement] = temp;
					temp = NULL;

					temp = new char[h+1];
					for (int j = heights[placement+1]; j < h; j++)
					{
						temp[j] = ' ';
					}

					for (int j = 0; j < heights[placement+1]; j++)
					{
						temp[j] = data[placement+1][j];
					}

					temp[h] = piece;

					heights[placement+1] = h+1;
					delete [] data[placement+1];
					data[placement+1] = temp;
					temp = NULL;

					temp = new char[h+2];
					for (int j = heights[placement+2]; j < h; j++)
					{
						temp[j] = ' ';
					}

					for (int j = 0; j < heights[placement+2]; j++)
					{
						temp[j] = data[placement+2][j];
					}

					temp[h] = piece;
					temp[h+1] = piece;

					heights[placement+2] = h+2;
					delete [] data[placement+2];
					data[placement+2] = temp;
					temp = NULL;
				}
			}

			if (rotation == 180)
			{
				if (data[placement] == NULL && data[placement+1] == NULL)
				{
					data[placement] = new char[3];
					data[placement+1] = new char[3];

					heights[placement] = 3;
					heights[placement+1] = 3;

					data[placement][0] = ' ';
					data[placement][1] = ' ';
					data[placement][2] = piece;
					data[placement+1][0] = piece;
					data[placement+1][1] = piece;
					data[placement+1][2] = piece;
				}
				else
				{
					int h = 0;
					for (int i = placement; i < placement + 3; i++)
					{
						if (heights[i] > h)
						{
							h = heights[i];
						}
					}

					if (heights[placement+1] < h-1)
					{
						char* temp = new char[h+1];
						if (data[placement+1] != NULL)
						{
							for (int j = 0; j < heights[placement+1]; j++)
							{
								temp[j] = data[placement+1][j];
							}
							for (int j = heights[placement+1]; j < h; j++)
							{
								temp[j] = ' ';
							}
						}
						else
						{
							for (int j = 0; j < heights[placement+1]; j++)
							{
								temp[j] = ' ';
							}
						}

						temp[h-2] = piece;
						temp[h-1] = piece;
						temp[h] = piece;

						heights[placement+1] = h+1;
						delete [] data[placement+1];
						data[placement+1] = temp;
						temp = NULL;

						temp = new char[h+1];
						if (data[placement] != NULL)
						{
							for (int j = 0; j < heights[placement]; j++)
							{
								temp[j] = data[placement][j];
							}
						}
						else
						{
							for (int j = 0; j < heights[placement]; j++)
							{
								temp[j] = ' ';
							}
						}

						temp[h] = piece;

						heights[placement] = h+1;
						delete [] data[placement];
						data[placement] = temp;
						temp = NULL;
					}
					else if (heights[placement+1] < h)
					{
						char* temp = new char[h+2];
						if (data[placement+1] != NULL)
						{
							for (int j = 0; j < heights[placement+1]; j++)
							{
								temp[j] = data[placement+1][j];
							}
						}
						else
						{
							for (int j = 0; j < heights[placement+1]; j++)
							{
								temp[j] = ' ';
							}
						}

						temp[h-1] = piece;
						temp[h] = piece;
						temp[h+1] = piece;

						heights[placement+1] = h+2;
						delete [] data[placement+1];
						data[placement+1] = temp;
						temp = NULL;

						temp = new char[h+2];
						if (data[placement] != NULL)
						{
							for (int j = 0; j < heights[placement]; j++)
							{
								temp[j] = data[placement][j];
							}
						}
						else
						{
							for (int j = 0; j < heights[placement]; j++)
							{
								temp[j] = ' ';
							}
						}

						temp[h] = ' ';
						temp[h+1] = piece;

						heights[placement] = h+2;
						delete [] data[placement];
						data[placement] = temp;
						temp = NULL;
					}
					else
					{
						char* temp = new char[h+3];
						if (data[placement+1] != NULL)
						{
							for (int j = 0; j < heights[placement+1]; j++)
							{
								temp[j] = data[placement+1][j];
							}
							for (int j = heights[placement+1]; j < h; j++)
							{
								temp[j] = ' ';
							}
						}
						else
						{
							for (int j = 0; j < heights[placement+1]; j++)
							{
								temp[j] = ' ';
							}
						}

						temp[h] = piece;
						temp[h+1] = piece;
						temp[h+2] = piece;

						heights[placement+1] = h+3;
						delete [] data[placement+1];
						data[placement+1] = temp;
						temp = NULL;

						temp = new char[h+3];
						if (data[placement] != NULL)
						{
							for (int j = 0; j < heights[placement]; j++)
							{
								temp[j] = data[placement][j];
							}
							for (int j = heights[placement]; j < h; j++)
							{
								temp[j] = ' ';
							}
						}
						else
						{
							for (int j = 0; j < heights[placement]; j++)
							{
								temp[j] = ' ';
							}
						}

						temp[h] = ' ';
						temp[h+1] = ' ';
						temp[h+2] = piece;

						heights[placement] = h+3;
						delete [] data[placement];
						data[placement] = temp;
						temp = NULL;
					}
				}	
			}

			if (rotation == 90)
			{
				if (placement >= width - 2)
				{
					cerr << "Placement entered is invalid." << endl;
					break;
				}

				if (data[placement] == NULL && data[placement+1] == NULL 
					&& data[placement+2] == NULL)
				{
					data[placement] = new char[2];
					data[placement+1] = new char[2];
					data[placement+2] = new char[2];

					heights[placement] = 2;
					heights[placement+1] = 2;
					heights[placement+2] = 2;

					data[placement][0] = piece;
					data[placement][0] = piece;
					data[placement+1][0] = ' ';
					data[placement+1][1] = piece;
					data[placement+2][0] = ' ';
					data[placement+2][1] = piece;
				}
				else
				{
					int h = 0;
					for (int i = placement; i < placement + 3; i++)
					{
						if (heights[i] > h)
						{
							h = heights[i];
						}
					}

					if (heights[placement] < h && (heights[placement+1] == h ||
						heights[placement+2] == h))
					{
						char* temp = new char[h+1];
						if (heights[placement] != h)
						{
							for (int j = heights[placement]; j < h; j++)
							{
								temp[j] = ' ';
							}
						}

						for (int j = 0; j < heights[placement]; j++)
						{
							temp[j] = data[placement][j];
						}

						temp[h-1] = piece;
						temp[h] = piece;
					
						heights[placement] = h+1;
						delete [] data[placement];
						data[placement] = temp;
						temp = NULL;

						temp = new char[h+1];
						for (int j = heights[placement+1]; j < h; j++)
						{
							temp[j] = ' ';
						}

						for (int j = 0; j < heights[placement+1]; j++)
						{
							temp[j] = data[placement+1][j];
						}

						temp[h] = piece;

						heights[placement+1] = h+1;
						delete [] data[placement+1];
						data[placement+1] = temp;
						temp = NULL;

						temp = new char[h+1];
						for (int j = heights[placement+2]; j < h; j++)
						{
							temp[j] = ' ';
						}

						for (int j = 0; j < heights[placement+2]; j++)
						{
							temp[j] = data[placement+2][j];
						}

						temp[h] = piece;

						heights[placement+2] = h+1;
						delete [] data[placement+2];
						data[placement+2] = temp;
						temp = NULL;
					}
					else
					{
						char* temp = new char[h+2];
						if (heights[placement] != h)
						{
							for (int j = heights[placement]; j < h; j++)
							{
								temp[j] = ' ';
							}
						}

						for (int j = 0; j < heights[placement]; j++)
						{
							temp[j] = data[placement][j];
						}

						temp[h] = piece;
						temp[h+1] = piece;

						heights[placement] = h+2;
						delete [] data[placement];
						data[placement] = temp;
						temp = NULL;

						temp = new char[h+2];
						for (int j = heights[placement+1]; j < h; j++)
						{
							temp[j] = ' ';
						}

						for (int j = 0; j < heights[placement+1]; j++)
						{
							temp[j] = data[placement+1][j];
						}

						temp[h] = ' ';
						temp[h+1] = piece;

						heights[placement+1] = h+2;
						delete [] data[placement+1];
						data[placement+1] = temp;
						temp = NULL;

						temp = new char[h+2];
						for (int j = heights[placement+2]; j < h; j++)
						{
							temp[j] = ' ';
						}

						for (int j = 0; j < heights[placement+2]; j++)
						{
							temp[j] = data[placement+2][j];
						}

						temp[h] = ' ';
						temp[h+1] = piece;

						heights[placement+2] = h+2;
						delete [] data[placement+2];
						data[placement+2] = temp;
						temp = NULL;
					}
				}
			}

			break;
		
		case 'J' :
			if (rotation == 0)
			{
				if (data[placement] == NULL && data[placement+1] == NULL)
				{
					data[placement+1] = new char[3];
					data[placement] = new char[1];

					heights[placement+1] = 3;
					heights[placement] = 1;

					data[placement+1][0] = piece;
					data[placement+1][1] = piece;
					data[placement+1][2] = piece;
					data[placement][0] = piece;
				}
				else
				{
					int h = 0;
					for (int i = placement; i < placement + 2; i++)
					{
						if (heights[i] > h)
						{
							h = heights[i];
						}
					}

					char* temp = new char[h+3];
					if (data[placement+1] != NULL)
					{
						for (int j = 0; j < heights[placement+1]; j++)
						{
							temp[j] = data[placement+1][j];
						}
						for (int j = heights[placement+1]; j < h; j++)
						{
							temp[j] = ' ';
						}
					}
					else
					{
						for (int j = 0; j < heights[placement+1]; j++)
						{
							temp[j] = ' ';
						}
					}

					temp[h] = piece;
					temp[h+1] = piece;
					temp[h+2] = piece;

					heights[placement+1] = h+3;
					delete [] data[placement+1];
					data[placement+1] = temp;
					temp = NULL;

					temp = new char[h+1];
					if (data[placement] != NULL)
					{
						for (int j = 0; j < heights[placement]; j++)
						{
							temp[j] = data[placement][j];
						}
						for (int j = heights[placement]; j < h; j++)
						{
							temp[j] = ' ';
						}
					}
					else
					{
						for (int j = 0; j < heights[placement]; j++)
						{
							temp[j] = ' ';
						}
					}

					temp[h] = piece;

					heights[placement] = h+1;
					delete [] data[placement];
					data[placement] = temp;
					temp = NULL;
				}
			}

			if (rotation == 90)
			{
				if (placement >= width - 2)
				{
					cerr << "Placement entered is invalid." << endl;
					break;
				}

				if (data[placement] == NULL && data[placement+1] == NULL 
					&& data[placement+2] == NULL)
				{
					data[placement] = new char[2];
					data[placement+1] = new char[1];
					data[placement+2] = new char[1];

					heights[placement] = 2;
					heights[placement+1] = 1;
					heights[placement+2] = 1;

					data[placement][0] = piece;
					data[placement][1] = piece;	
					data[placement+1][0] = piece;
					data[placement+2][0] = piece;
				}
				else
				{
					int h = 0;
					for (int i = placement; i < placement + 3; i++)
					{
						if (heights[i] > h)
						{
							h = heights[i];
						}
					}

					char* temp = new char[h+2];
					if (heights[placement] != h)
					{
						for (int j = heights[placement]; j < h; j++)
						{
							temp[j] = ' ';
						}
					}

					for (int j = 0; j < heights[placement]; j++)
					{
						temp[j] = data[placement][j];
					}

					temp[h] = piece;
					temp[h+1] = piece;
				
					heights[placement] = h+2;
					delete [] data[placement];
					data[placement] = temp;
					temp = NULL;

					temp = new char[h+1];
					for (int j = heights[placement+1]; j < h; j++)
					{
						temp[j] = ' ';
					}

					for (int j = 0; j < heights[placement+1]; j++)
					{
						temp[j] = data[placement+1][j];
					}

					temp[h] = piece;

					heights[placement+1] = h+1;
					delete [] data[placement+1];
					data[placement+1] = temp;
					temp = NULL;

					temp = new char[h+1];
					for (int j = heights[placement+2]; j < h; j++)
					{
						temp[j] = ' ';
					}

					for (int j = 0; j < heights[placement+2]; j++)
					{
						temp[j] = data[placement+2][j];
					}

					temp[h] = piece;

					heights[placement+2] = h+1;
					delete [] data[placement+2];
					data[placement+2] = temp;
					temp = NULL;
				}
			}

			if (rotation == 180)
			{	
				if (data[placement] == NULL && data[placement+1] == NULL)
				{
					data[placement] = new char[3];
					data[placement+1] = new char[3];

					heights[placement] = 3;
					heights[placement+1] = 3;

					data[placement][0] = piece;
					data[placement][1] = piece;
					data[placement][2] = piece;
					data[placement+1][0] = ' ';
					data[placement+1][1] = ' ';
					data[placement+1][2] = piece;
				}
				else
				{
					int h = 0;
					for (int i = placement; i < placement + 3; i++)
					{
						if (heights[i] > h)
						{
							h = heights[i];
						}
					}

					if (heights[placement] < h-1)
					{
						char* temp = new char[h+1];
						if (data[placement] != NULL)
						{
							for (int j = 0; j < heights[placement]; j++)
							{
								temp[j] = data[placement][j];
							}
							for (int j = heights[placement]; j < h; j++)
							{
								temp[j] = ' ';
							}
						}
						else
						{
							for (int j = 0; j < heights[placement]; j++)
							{
								temp[j] = ' ';
							}
							for (int j = heights[placement]; j < h; j++)
							{
								temp[j] = ' ';
							}
						}

						temp[h-2] = piece;
						temp[h-1] = piece;
						temp[h] = piece;

						heights[placement] = h+1;
						delete [] data[placement];
						data[placement] = temp;
						temp = NULL;

						temp = new char[h+1];
						if (data[placement+1] != NULL)
						{
							for (int j = 0; j < heights[placement+1]; j++)
							{
								temp[j] = data[placement+1][j];
							}
							for (int j = heights[placement+1]; j < h; j++)
							{
								temp[j] = ' ';
							}
						}
						else
						{
							for (int j = 0; j < heights[placement+1]; j++)
							{
								temp[j] = ' ';
							}
							for (int j = heights[placement+1]; j < h; j++)
							{
								temp[j] = ' ';
							}
						}

						temp[h] = piece;

						heights[placement+1] = h+1;
						delete [] data[placement+1];
						data[placement+1] = temp;
						temp = NULL;
					}
					else if (heights[placement] < h)
					{
						char* temp = new char[h+2];
						if (data[placement] != NULL)
						{
							for (int j = 0; j < heights[placement]; j++)
							{
								temp[j] = data[placement][j];
							}
							for (int j = heights[placement]; j < h; j++)
							{
								temp[j] = ' ';
							}
						}
						else
						{
							for (int j = 0; j < heights[placement]; j++)
							{
								temp[j] = ' ';
							}
						}

						temp[h-1] = piece;
						temp[h] = piece;
						temp[h+1] = piece;

						heights[placement] = h+2;
						delete [] data[placement];
						data[placement] = temp;
						temp = NULL;

						temp = new char[h+2];
						if (data[placement+1] != NULL)
						{
							for (int j = 0; j < heights[placement+1]; j++)
							{
								temp[j] = data[placement+1][j];
							}
							for (int j = heights[placement+1]; j < h; j++)
							{
								temp[j] = ' ';
							}
						}
						else
						{
							for (int j = 0; j < heights[placement+1]; j++)
							{
								temp[j] = ' ';
							}
						}

						temp[h] = ' ';
						temp[h+1] = piece;

						heights[placement+1] = h+2;
						delete [] data[placement+1];
						data[placement+1] = temp;
						temp = NULL;
					}
					else
					{
						char* temp = new char[h+3];
						if (data[placement] != NULL)
						{
							for (int j = 0; j < heights[placement]; j++)
							{
								temp[j] = data[placement][j];
							}
							for (int j = heights[placement]; j < h; j++)
							{
								temp[j] = ' ';
							}
						}
						else
						{
							for (int j = 0; j < heights[placement]; j++)
							{
								temp[j] = ' ';
							}
						}

						temp[h] = piece;
						temp[h+1] = piece;
						temp[h+2] = piece;

						heights[placement] = h+3;
						delete [] data[placement];
						data[placement] = temp;
						temp = NULL;

						temp = new char[h+3];
						if (data[placement+1] != NULL)
						{
							for (int j = 0; j < heights[placement+1]; j++)
							{
								temp[j] = data[placement+1][j];
							}
							for (int j = heights[placement+1]; j < h; j++)
							{
								temp[j] = ' ';
							}
						}
						else
						{
							for (int j = 0; j < heights[placement+1]; j++)
							{
								temp[j] = ' ';
							}
						}

						temp[h] = ' ';
						temp[h+1] = ' ';
						temp[h+2] = piece;

						heights[placement+1] = h+3;
						delete [] data[placement+1];
						data[placement+1] = temp;
						temp = NULL;
					}
				}	
			}

			if (rotation == 270)
			{

				if (placement >= width - 2)
				{
					cerr << "Placement entered is invalid." << endl;
					break;
				}

				if (data[placement] == NULL && data[placement+1] == NULL 
					&& data[placement+2] == NULL)
				{
					data[placement] = new char[2];
					data[placement+1] = new char[2];
					data[placement+2] = new char[2];

					heights[placement] = 2;
					heights[placement+1] = 2;
					heights[placement+2] = 2;

					data[placement][0] = ' ';
					data[placement][0] = piece;
					data[placement+1][0] = ' ';
					data[placement+1][1] = piece;
					data[placement+2][0] = piece;
					data[placement+2][1] = piece;
				}
				else
				{
					int h = 0;
					for (int i = placement; i < placement + 3; i++)
					{
						if (heights[i] > h)
						{
							h = heights[i];
						}
					}

					if (heights[placement+2] < h && (heights[placement+1] == h ||
						heights[placement] == h))
					{
						char* temp = new char[h+1];
						if (heights[placement+2] != h)
						{
							for (int j = heights[placement+2]; j < h; j++)
							{
								temp[j] = ' ';
							}
						}

						for (int j = 0; j < heights[placement+2]; j++)
						{
							temp[j] = data[placement+2][j];
						}

						temp[h-1] = piece;
						temp[h] = piece;
					
						heights[placement+2] = h+1;
						delete [] data[placement+2];
						data[placement+2] = temp;
						temp = NULL;

						temp = new char[h+1];
						for (int j = heights[placement+1]; j < h; j++)
						{
							temp[j] = ' ';
						}

						for (int j = 0; j < heights[placement+1]; j++)
						{
							temp[j] = data[placement+1][j];
						}

						temp[h] = piece;

						heights[placement+1] = h+1;
						delete [] data[placement+1];
						data[placement+1] = temp;
						temp = NULL;

						temp = new char[h+1];
						for (int j = heights[placement]; j < h; j++)
						{
							temp[j] = ' ';
						}

						for (int j = 0; j < heights[placement]; j++)
						{
							temp[j] = data[placement][j];
						}

						temp[h] = piece;

						heights[placement] = h+1;
						delete [] data[placement];
						data[placement] = temp;
						temp = NULL;
					}
					else
					{
						char* temp = new char[h+2];
						if (heights[placement+2] != h)
						{
							for (int j = heights[placement+2]; j < h; j++)
							{
								temp[j] = ' ';
							}
						}

						for (int j = 0; j < heights[placement+2]; j++)
						{
							temp[j] = data[placement+2][j];
						}

						temp[h] = piece;
						temp[h+1] = piece;

						heights[placement+2] = h+2;
						delete [] data[placement+2];
						data[placement+2] = temp;
						temp = NULL;

						temp = new char[h+2];
						for (int j = heights[placement+1]; j < h; j++)
						{
							temp[j] = ' ';
						}

						for (int j = 0; j < heights[placement+1]; j++)
						{
							temp[j] = data[placement+1][j];
						}

						temp[h] = ' ';
						temp[h+1] = piece;

						heights[placement+1] = h+2;
						delete [] data[placement+1];
						data[placement+1] = temp;
						temp = NULL;

						temp = new char[h+2];
						for (int j = heights[placement]; j < h; j++)
						{
							temp[j] = ' ';
						}

						for (int j = 0; j < heights[placement]; j++)
						{
							temp[j] = data[placement][j];
						}

						temp[h] = ' ';
						temp[h+1] = piece;

						heights[placement] = h+2;
						delete [] data[placement];
						data[placement] = temp;
						temp = NULL;
					}
				}
			}

			break;

		default:
			cout << "Invalid piece entered" << endl;
	}
}

// Function returns the maximum height value in the array
// "heights"
int Tetris::get_max_height() const
{
	int max = 0;

	for (int i = 0; i < width; i++)
	{
		if (heights[i] > max)
		{
			max = heights[i];
		}
	}

	return max;
}

// Function counts the number of squares on the board filled
// by a piece and returns the value
int Tetris::count_squares() const
{
	int squares = 0;

	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < heights[i]; j++)
		{
			if (data[i][j] != ' ')
			{
				squares++;
			}
		}
	}
	return squares;
}

// Function removes a row off the board if the entire row is 
// filled, returning a score value equal to the number of 
// rows removed
int Tetris::remove_full_rows() const
{
	int min_h = get_max_height();

	//Finds the minimum height...don't need to loop above it
	//because no row above the minimum height can possibly
	//have its whole row filled
	for (int i = 0; i < width; i++)
	{
		if (heights[i] < min_h)
		{
			min_h = heights[i];
		} 
	}

	//Array to store how many pieces are in each row, to handle
	//multirow cases
	int c[min_h];
	int score = 0;

	for (int i = 0; i < min_h; i++)
	{
		c[i] = 0;

		for (int j = 0; j < width; j++)
		{
			if (data[j][i] != ' ')
			{
				c[i]++;
			}
		}
	}

	//Loop through the array created above
	for(int i = 0; i < min_h; i++)
	{
		//If the row in question is full...
		if (c[i] == width)
		{
			//Loop through all existing arrays
			for (int k = 0; k < width; k++)
			{	
				//Create a new temp array wiht height one smaller
				//than previously
				char* temp = new char[heights[k]-1];

				//Set old data values to new temp array below row deleted
				for (int l = 0; l < i; l++)
				{					
					if (l > heights[k] - 2)
					{
						continue;
					}

					temp[l] = data[k][l];
				}

				//Do the same for the rows above
				for (int m = i+1; m < heights[k]; m++)
				{
					temp[m-1] = data[k][m];
				}
				
				int d = 0;
				for (int n = 0; n < heights[k] - 1; n++)
				{
					if (temp[n] != ' ')
					{
						d++;
					}
				}

				if (d != 0)
				{
					heights[k]--;
					delete [] data[k];
					data[k] = temp;
					temp = NULL;
				}
				else
				{
					heights[k] = 0;
					delete [] data[k];
					data[k] = temp;
					temp = NULL;
				}
			}

			//Add one to the score if the row was deleted
 	 		score++;

 	 		if (i != min_h-1 && c[i+1] == width)
 	 		{
 	 			i--;
 	 			min_h--;
 	 		}
		}
	}

	return score;
}

// Function adds a new column on the left of the board, 
// updating the heights array and adding a new NULL array
// to the 2D data array
void Tetris::add_left_column()
{
	int* temp_h = new int[width+1];

	temp_h[0] = 0;
	for (int i = 0; i < width; i++)
	{
		temp_h[i+1] = heights[i];
	}

	delete [] heights;
	heights = temp_h;
	temp_h = NULL;

	char** temp_d = new char*[width+1];

	temp_d[0] = NULL;
	for (int i = 0; i < width; i++)
	{
		temp_d[i+1] = data[i];
	}

	delete [] data;
	data = temp_d;
	temp_d = NULL;

	//Adds one to the width
	width++;
}

// Function adds a new column on the right of the board, 
// updating the heights array and adding a new NULL array
// to the 2D data array
void Tetris::add_right_column()
{
	int* temp_h = new int[width+1];

	temp_h[width] = 0;
	for (int i = 0; i < width; i++)
	{
		temp_h[i] = heights[i];
	}

	delete [] heights;
	heights = temp_h;
	temp_h = NULL;

	char** temp_d = new char*[width+1];

	temp_d[width] = NULL;
	for (int i = 0; i < width; i++)
	{
		temp_d[i] = data[i];
	}
	
	delete [] data;
	data = temp_d;
	temp_d = NULL;

	//Adds one to the width
	width++;
}

// Function removes a column from the left of the board, 
// regardless of what is in it, and updates the heights 
// array and the 2D data array
void Tetris::remove_left_column()
{
	int* temp_h = new int[width-1];

	for (int i = 0; i < width - 1; i++)
	{
		temp_h[i] = heights[i+1];
	}

	delete [] heights;
	heights = temp_h;
	temp_h = NULL;

	char** temp_d = new char*[width-1];

	for (int i = 0; i < width - 1; i++)
	{
		temp_d[i] = data[i+1];
	}

	delete [] data[0];
	delete [] data;
	data = temp_d;
	temp_d = NULL;

	//Removes one from the width
	width--;
}

// Function removes a column from the right of the board, 
// regardless of what is in it, and updates the heights 
// array and the 2D data array
void Tetris::remove_right_column()
{
	int* temp_h = new int[width-1];

	for (int i = 0; i < width - 1; i++)
	{
		temp_h[i] = heights[i];
	}

	delete [] heights;
	heights = temp_h;
	temp_h = NULL;

	char** temp_d = new char*[width-1];

	for (int i = 0; i < width - 1; i++)
	{
		temp_d[i] = data[i];
	}

	delete [] data[width-1];
	delete [] data;
	data = temp_d;
	temp_d = NULL;

	//Removes one from the width
	width--;
}

// Function deletes all dynamically allocated data on the heap,
// ensuring that there are no memory leaks by the end of the 
// program's run
void Tetris::destroy() const
{
	delete [] heights;

	for (int i = 0; i < width; i++)
	{
		delete [] data[i];
	}

	delete [] data;
}