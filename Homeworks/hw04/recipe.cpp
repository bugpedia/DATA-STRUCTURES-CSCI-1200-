//File: kitchen.cpp

#include <iostream>
#include "recipe.h"

// Constructor: Sets the name passed through as the member variable
// "name" 
Recipe::Recipe(string aName)
{
	name = aName;
}

// When addIngredient is called, it pushes back an Ingredient object
// to the member list of Ingredient objects
void Recipe::addIngredient(string aName, int aNumOfUnits)
{
	ingredients.push_back(Ingredient(aName, aNumOfUnits));
}

// Function iterates through the list of ingredients necessary to create
// a recipe and outputs them, after sorting
void Recipe::print(ostream &out)
{
	out << "To make " << name << ", mix together:" << endl;

  // Calls "sort_ingredients", a non-member sort function of the Ingredient
  // class
	ingredients.sort(sort_ingredients);

  // Creating a const_iterator to go through all values of the list
	list<Ingredient>::const_iterator itr = ingredients.begin();

  // Looping through with a while loop
	while (itr != ingredients.end())
	{
    // Outputs "unit" if the units required to make this recipe equals
    // 1, otherwise it outputs "units"
		if ((*itr).getUnits() == 1)
		{
			out << "  " << (*itr).getUnits() << " unit of " 
				<< (*itr).getName() << endl;
		}
		else
		{
			out << "  " << (*itr).getUnits() << " units of " 
				<< (*itr).getName() << endl;
		}
    
    // Incrememt the iterator
		itr++;
	}
}