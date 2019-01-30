//File: ingredient.h

//Include guards
#ifndef __ingredient_h_
#define __ingredient_h_

#include <iostream>
#include <list>

using namespace std;

class Ingredient
{
public:
	//Constructor
	Ingredient(string aName, int aNumOfUnits);

	//Accessors
	string getName() const {return name;}
	int getUnits() const {return numUnits;}

	//Modifiers
	void addUnits(int aNumOfUnits);
	void subtractUnits(int aNumOfUnits);

private:
	string name;
	int numUnits;
};

//Non-member sorting functions
bool sort_ingredients(const Ingredient &i1, const Ingredient &i2);
bool sort_ingredients_kitchen(const Ingredient &i1, const Ingredient &i2);
#endif