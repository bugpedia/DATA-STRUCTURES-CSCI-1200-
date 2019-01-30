//File: kitchen.cpp

#include <iostream>
#include "kitchen.h"

// Default constructor

// Adds an Ingredient object to the member list "ingredients"
void Kitchen::addIngredient(string aName, int aNumOfUnits)
{
	list<Ingredient>::iterator itr = ingredients.begin();
	bool found = false;

	// Iterates through ingredients to check if we already have the 
	// ingredient we want to add
	while(itr != ingredients.end())
	{
		// If we do, then add to that ingredient the units passed through 
		// as a parameter, and set found to true
		if (aName == (*itr).getName())
		{
			(*itr).addUnits(aNumOfUnits);
			found = true;
		}
		
		itr++;
	}

	// If the ingredient does not exist, create it and push it back to
	// the member list "ingredients"
	if (!found)
	{
		ingredients.push_back(Ingredient(aName, aNumOfUnits));
	}
}

// Prints out the contents of the member list "ingredients"
void Kitchen::printIngredients(ostream &out)
{
	out << "In the kitchen:" << endl;

	// Sorts the ingredients using the "sort_ingredients_kitchen"
	// non-member function of the Ingredient class, which sorts
	// by units then by name if units are equal
	ingredients.sort(sort_ingredients_kitchen);

	list<Ingredient>::iterator itr = ingredients.begin();
	list<Ingredient>::iterator itr2;

	for(itr; itr != ingredients.end(); itr++)
	{
		// If we have only one unit of something, output "unit" with no "s"
		if ((*itr).getUnits() == 1)
		{
			out << "  " << (*itr).getUnits() << " unit of " 
				<< (*itr).getName() << endl;
		} 
		// If we have no units of something, erase it from the list
		else if ((*itr).getUnits() == 0)
		{
			itr = ingredients.erase(itr);
			// The iterator automatically moves forward when we call erase
			// so to avoid a segmentation fault, we decrease the iterator
			// which will then be increased again after the loop begins anew
			itr--;
		}
		//Else, we can output the info with "units" with an "s"
		else
		{
			out << "  " << (*itr).getUnits() << " units of " 
				 << (*itr).getName() << endl;
		}
	}
}

// Makes a recipe if there are enough ingredients and all exist in
// the kitchen, and subtracts the units from the ingredients in the
// kitchen once it has been made
void Kitchen::makeRecipe(Recipe aRecipe, ostream &out)
{
	// Calls the accessor from the Recipe class to get the list of 
	// ingredients needed for the inputted recipe
	list<Ingredient> ingredients_r = aRecipe.getIngredients();
	// Empty list that will later be filled with ingredients we have
	// in the kitchen 
	list<Ingredient> have_l;

	list<Ingredient>::iterator itr = ingredients.begin();
	list<Ingredient>::iterator itr_r = ingredients_r.begin();
	bool enough = true;
	// Empty list that will be filled with the ingredients that we don't
	// have enough units of, and how many units we are short by
	list<Ingredient> insufficient;
	int have = 0;

	for(itr_r; itr_r != ingredients_r.end(); itr_r++)
	{
		itr = ingredients.begin();

		while(itr != ingredients.end())
		{
			if ((*itr_r).getName() == (*itr).getName())
			{
				// Add one to the "have" counter, saying we have this ingredient
				have++;
				// Push it back to the list of ingredients we have
				have_l.push_back(*itr_r);
				// If we don't have enough units, set enough to be false, and push
				// back the ingredient and the units we need to the insufficient
				// list
				if ((*itr_r).getUnits() > (*itr).getUnits())
				{
					enough = false;
					int units_needed = (*itr_r).getUnits() - (*itr).getUnits();
					insufficient.push_back
						(Ingredient((*itr).getName(), units_needed));
				}
			}

			itr++;
		}
	}
	
	// If we have enough of everything, and have all the ingredients in the 
	// kitchen, we can make the recipe
	if (enough && have == ingredients_r.size())
	{
		out << "Made " << aRecipe.getName() << endl;

		itr = ingredients.begin();
		itr_r = ingredients_r.begin();

		// Loops through and subtracts from the kitchen ingredients the 
		// amount of units for each ingredient that we will use for making 
		// the recipes
		for(itr; itr != ingredients.end(); itr++)
		{
			for(itr_r; itr_r != ingredients_r.end(); itr_r++)
			{
				if ((*itr).getName() == (*itr_r).getName())
				{
					(*itr).subtractUnits((*itr_r).getUnits());
				}
			}

			itr_r = ingredients_r.begin();
		}
	}

	// However, if we have enough of everything we do have but simply don't
	// have all the ingredients in the kitchen needed to make the recipe, we
	// can't make it
	else if (enough && have != ingredients_r.size())
	{
		out << "Cannot make " << aRecipe.getName() << ", need to buy:" << endl;
		list<Ingredient>::iterator itr_h = have_l.begin();
		list<Ingredient>::iterator itr_recipe = ingredients_r.begin();
		list<Ingredient> not_have;
		
		// Iterates through the recipe ingredients, and then the list of what 
		// we have, and incrememnts a counter if we find the recipe ingredient
		// in what we have, however, if the counter is zero, then we can push  
		// back the ingredient in the recipe that doesn't exist in the "have" list
		while (itr_recipe != ingredients_r.end())
		{
			itr_h = have_l.begin();
			int c = 0;
			while (itr_h != have_l.end())
			{
				if ((*itr_h).getName() == (*itr_recipe).getName())
				{
					c++;	
				}

				itr_h++;
			}
			if (c == 0)
			{
				not_have.push_back(*itr_recipe);
			}

			itr_recipe++;
		}

		// Sorts the list of things we do not have
		not_have.sort(sort_ingredients);

		// Iterates through and outputs them nicely, using "unit" if we only
		// have one unit and "units" if we have more
		list<Ingredient>::iterator itr_n_h = not_have.begin();
		while (itr_n_h != not_have.end())
		{
			if ((*itr_n_h).getUnits() == 1)
			{
				out << "  " << (*itr_n_h).getUnits() << " unit of " 
						<< (*itr_n_h).getName() << endl;
			}
			else
			{
				out << "  " << (*itr_n_h).getUnits() << " units of " 
						<< (*itr_n_h).getName() << endl;
			}

			itr_n_h++;
		}

	}

	// If we execute this step, that means that we don't have enough of 
	// everything, and we still don't have all the ingredients in the 
	// kitchen
	else if (have != ingredients_r.size())
	{
		out << "Cannot make " << aRecipe.getName() << ", need to buy:" << endl;

		list<Ingredient>::iterator itr_h = have_l.begin();
		list<Ingredient>::iterator itr_recipe = ingredients_r.begin();
		list<Ingredient> not_have;
		
		// Iterates through and finds what we do not have
		while (itr_recipe != ingredients_r.end())
		{
			itr_h = have_l.begin();
			int c = 0;
			while (itr_h != have_l.end())
			{
				if ((*itr_h).getName() == (*itr_recipe).getName())
				{
					c++;	
				}

				itr_h++;
			}
			if (c == 0)
			{
				not_have.push_back(*itr_recipe);
			}

			itr_recipe++;
		}

		list<Ingredient>::iterator itr_i = insufficient.begin();
		
		// Pushes back what is insufficient to the list of what we don't have
		// so we can ease sorting		
		for (itr_i; itr_i != insufficient.end(); itr_i++)
		{
			not_have.push_back(*itr_i);
		}

		// Sorts the list alphabetically
		not_have.sort(sort_ingredients);

		// Outputs the info
		list<Ingredient>::iterator itr_n_h = not_have.begin();
		while (itr_n_h != not_have.end())
		{
			if ((*itr_n_h).getUnits() == 1)
			{
				out << "  " << (*itr_n_h).getUnits() << " unit of " 
						<< (*itr_n_h).getName() << endl;
			}
			else
			{
				out << "  " << (*itr_n_h).getUnits() << " units of " 
						<< (*itr_n_h).getName() << endl;
			}

			itr_n_h++;
		}
	}

	// Otherwise...we just have insufficient quantities, but have everything
	// the recipe says we need in the kitchen
	else
	{
		out << "Cannot make " << aRecipe.getName() << ", need to buy:" << endl;

		list<Ingredient>::iterator itr_i = insufficient.begin();
		
		// Loops through the list of what is in "insufficient" and outputs the
		// info
		for (itr_i; itr_i != insufficient.end(); itr_i++)
		{
			if ((*itr_i).getUnits() == 1)
			{
				out << "  " << (*itr_i).getUnits() << " unit of " 
						<< (*itr_i).getName() << endl;
			}
			else
			{
				out << "  " << (*itr_i).getUnits() << " units of " 
						<< (*itr_i).getName() << endl;
			}
		}
	}
}

// Goes through a list of recipes and checks to see if they can be made, 
// and if they can, it suggests that they be made
void Kitchen::recipeSuggestions(ostream &out, list<Recipe> &recipes)
{
	list<string> recipes_to_make;
	list<Recipe>::iterator itr_l_r = recipes.begin();
	list<Ingredient>::iterator itr = ingredients.begin();
	list<Ingredient> insufficient;
	int have = 0;

	// Iterating through the list of all recipies given...
	for(itr_l_r; itr_l_r != recipes.end(); itr_l_r++)
	{
		bool enough = true;

		list<Ingredient> ingredients_r = (*itr_l_r).getIngredients();
		list<Ingredient>::iterator itr_r = ingredients_r.begin();

		// We use the same logic that we used up in the makeRecipe function
		// to see if we can make the recipe
		for(itr_r; itr_r != ingredients_r.end(); itr_r++)
		{
			itr = ingredients.begin();

			while(itr != ingredients.end())
			{
				if ((*itr_r).getName() == (*itr).getName())
				{
					have++;

					if ((*itr_r).getUnits() > (*itr).getUnits())
					{
						enough = false;
						int units_needed = (*itr_r).getUnits() - (*itr).getUnits();
						insufficient.push_back(Ingredient((*itr).getName(),
						 	units_needed));
					}
				}

				itr++;
			}
		}

		// If we have enough and have all the ingredients, we can make it,
		// so we push it back to a list of "recipes_to_make"
		if(enough && have == ingredients_r.size())
		{
			recipes_to_make.push_back((*itr_l_r).getName());
		}

		have = 0;
	}

	// If there aren't any recipes to make...
	if (recipes_to_make.size() == 0)
	{
		out << "No recipes can be prepared" << endl;
	}
	// If there's only one... (outputs "Recipe")
	else if (recipes_to_make.size() == 1)
	{
		out << "Recipe that can be prepared:" << endl;
		list<string>::iterator itr = recipes_to_make.begin();
		out << "  " << (*itr) << endl;
	}
	// If there are 2+... (outputs "Recipes")
	else
	{
		out << "Recipes that can be prepared:" << endl;
		recipes_to_make.sort();

		list<string>::iterator itr = recipes_to_make.begin();
		while (itr != recipes_to_make.end())
		{
			out << "  " << (*itr) << endl;
			itr++;
		}
	}
}