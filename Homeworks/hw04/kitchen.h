//File: kitchen.h

//Include guards
#ifndef __kitchen_h_
#define __kitchen_h_

#include <iostream>
#include <list>
#include "recipe.h"

class Kitchen
{
public:
	//Constructor

	//Accessors
	void printIngredients(ostream &out);

	//Modifiers
	void addIngredient(string aName, int numUnits);

	//Member Functions
	void makeRecipe(Recipe aRecipe, ostream &out);
	void recipeSuggestions(ostream &out, list<Recipe> &recipes);

private:
	list<Ingredient> ingredients;
};

#endif