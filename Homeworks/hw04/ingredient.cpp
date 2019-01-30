//File: ingredient.cpp

#include <iostream>
#include "ingredient.h"

// Constructor: Sets name passed through equal to member variable "name"
// and sets number of units passed to the member variable "numUnits"
Ingredient::Ingredient(string aName, int aNumOfUnits)
{
	name = aName; 
	numUnits = aNumOfUnits;
}

// Edits numUnits by adding to it the amount of units passed through
// as a parameter
void Ingredient::addUnits(int aNumofUnits)
{
	numUnits += aNumofUnits;
}

// Edits numUnits by subtracting from it the amount of units passed through
// as a parameter
void Ingredient::subtractUnits(int aNumofUnits)
{
	numUnits -= aNumofUnits;
}

// Sorts the ingredients alphabetically
bool sort_ingredients(const Ingredient &i1, const Ingredient &i2)
{
	return (i1.getName() < i2.getName()); 
}

// Sorts the ingredients for the kitchen by units and if the units are 
// equal, then alphabetically
bool sort_ingredients_kitchen(const Ingredient &i1, const Ingredient &i2)
{
	return (i1.getUnits() < i2.getUnits() || 
		(i1.getUnits() == i2.getUnits() && i1.getName() < i2.getName()));
}