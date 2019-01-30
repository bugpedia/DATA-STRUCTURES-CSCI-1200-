//File: recipe.h

//Include guards
#ifndef __recipe_h_
#define __recipe_h_

#include <iostream>
#include <list>
#include "ingredient.h"

class Recipe
{
public:
	//Constructor
	Recipe(string aName);

	//Accessors
	string getName() const {return name;}
	list<Ingredient> getIngredients() const {return ingredients;}

	//Modifiers
	void addIngredient(string aName, int aNumOfUnits);

	//Member Functions
	void print(ostream &out);

private:
	string name;
	list<Ingredient> ingredients;
};

#endif