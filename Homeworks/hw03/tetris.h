//File: tetris.h

//Include guards
#ifndef __tetris_h_
#define __tetris_h_

#include <iostream>

using namespace std;

class Tetris
{
public:
	//Constructor
	Tetris(int aWidth);

	//Accessors
	int get_width() const {return width;}

	//Modifiers
	void add_piece(char piece, int rotation, int placement);

	//Member Functions
	void print() const;
	int get_max_height() const;
	int count_squares() const;
	int remove_full_rows() const;
	void add_left_column();
	void add_right_column();
	void remove_left_column();
	void remove_right_column();
	void destroy() const;


private:
	//Private member variables
	int width;
	int* heights;
	char** data;

};

#endif