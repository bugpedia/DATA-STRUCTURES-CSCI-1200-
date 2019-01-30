//
// PROVIDED CODE FOR HOMEWORK 4: GROCERY LISTS
// 
// You may use none, a little, or all of this, as you choose, but we
// strongly urge you to examine it carefully.  You may modify this
// code as you wish to complete the assignment.


#include <cassert>
#include <cstdlib>
#include <iostream>
#include <fstream>

#include "recipe.h"
#include "kitchen.h"


// Helper functions
void readRecipe(std::istream &istr, std::ostream &ostr, std::list<Recipe> &recipes);
void addIngredients(std::istream &istr, std::ostream &ostr, Kitchen &kitchen);
void printRecipe(std::istream &istr, std::ostream &ostr, std::list<Recipe> &recipes);
void makeRecipe(std::istream &istr, std::ostream &ostr, const std::list<Recipe> &recipes, Kitchen &kitchen);
//void recipeSuggestions(std::ostream &ostr, const std::list<Recipe> &recipes, const Kitchen &kitchen);

// The main loop parses opens the files for I/O & parses the input
int main(int argc, char* argv[]) {

  // Check the number of arguments.
  if (argc != 3) {
    std::cerr << "Usage: " << argv[0] << " in-file out-file\n";
    return 1;
  }
  // Open and test the input file.
  std::ifstream istr(argv[1]);
  if (!istr) {
    std::cerr << "Could not open " << argv[1] << " to read\n";
    return 1;
  }
  // Open and test the output file.
  std::ofstream ostr(argv[2]);
  if (!ostr) {
    std::cerr << "Could not open " << argv[2] << " to write\n";
    return 1;
  }

  // the kitchen & recipe list
  Kitchen kitchen;
  std::list<Recipe> recipes;

  // some variables to help with parsing
  char c;
  while (istr >> c) {
    if (c == 'r') {
      // READ A NEW RECIPE
      readRecipe(istr,ostr,recipes);

    } else if (c == 'a') {
      // ADD INGREDIENTS TO THE KITCHEN
      addIngredients(istr,ostr,kitchen);
      
    } else if (c == 'p') {
      // PRINT A PARTICULAR RECIPE
      printRecipe(istr,ostr,recipes);

    } else if (c == 'm') {
      // MAKE SOME FOOD
      makeRecipe(istr,ostr,recipes,kitchen);

    } else if (c == 'k') {
      // PRINT THE CONTENTS OF THE KITCHEN
      kitchen.printIngredients(ostr);

    } else if (c == 's') {
      // SUGGEST ALL RECIPES THAT CAN BE MADE INDIVIDUALLY FROM THE
      //   CURRENT CONTENTS OF THE KITCHEN
      kitchen.recipeSuggestions(ostr,recipes);

    } else if (c == 'd') {
      // EXTRA CREDIT: SUGGEST THE COLLECTION OF RECIPES THAT CAN BE
      // MADE TOGETHER THAT USE THE MAXIMUM NUMBER OF UNITS OF
      // INGREDIENTS
    } else {
      std::cerr << "unknown character: " << c << std::endl;
      exit(0);
    }
  }  
}

void readRecipe(std::istream &istr, std::ostream &ostr, std::list<Recipe> &recipes) {
  int units;
  std::string name, name2;  
  istr >> name;

  list<Recipe>::iterator itr = recipes.begin();

  // Loops through the existing recipes to check if it already exists, and then
  // exits the loop
  for (itr; itr != recipes.end(); itr++)
  {
    if ((*itr).getName() == name)
    {
      ostr << "Recipe for " << name << " already exists" << endl;

      while(1){
        istr >> units;
        if (units == 0) break;
        istr >> name;
      }
      return;
    }
  }

  // build the new recipe assuming it doesn't already exist
  Recipe r(name);
  while (1) {
    istr >> units;
    if (units == 0) break;
    assert (units > 0);
    istr >> name2;
    r.addIngredient(name2,units);
  }
  // add it to the list
  recipes.push_back(r);
  ostr << "Recipe for " << name << " added" << std::endl;
}


void addIngredients(std::istream &istr, std::ostream &ostr, Kitchen &kitchen) {
  int units;
  std::string name;
  int count = 0;
  while (1) {
    istr >> units;
    if (units == 0) break;
    assert (units > 0);
    istr >> name;
    // add the ingredients to the kitchen
    kitchen.addIngredient(name, units);
    count++;
  }

  // Depending on if 1 ingredient was added or if more than one were,
  // either "ingredient" or "ingredients" will be outputted
  if (count == 1)
  {
    ostr << count << " ingredient added to kitchen" << std::endl;
  }
  else{
    ostr << count << " ingredients added to kitchen" << std::endl;

  }
}


void printRecipe(std::istream &istr, std::ostream &ostr, std::list<Recipe> &recipes) {
  std::string name;
  istr >> name;
  list<Recipe>::iterator itr = recipes.begin();
  bool found = false;

  // Loops through list of recipes and outputs the info to make the recipe,
  // setting found to true if it was found
  for (itr; itr != recipes.end(); itr++)
  {
    if ((*itr).getName() == name)
    {
      (*itr).print(ostr);
      found = true;
    }
  }

  // And if the recipe was not found...
  if (!found)
  {
    ostr << "No recipe for " << name << endl;
  }
}


void makeRecipe(std::istream &istr, std::ostream &ostr, const std::list<Recipe> &recipes, Kitchen &kitchen) {
  std::string name;
  istr >> name;
  list<Recipe>::const_iterator itr = recipes.begin();
  list<Recipe>::const_iterator itr_make;
  bool found = false;

  // Checks list of recipes to see if we have it to make it, setting 
  // found to true
  for (itr; itr != recipes.end(); itr++)
  {
    if ((*itr).getName() == name)
    {
      found = true;
      // Sets itr_make to be the recipe that we will make 
      itr_make = itr;
    }
  }

  // If we don't find it, output that it cannot be made and return, exiting
  // the function
  if (!found)
  {
    ostr << "Don't know how to make " << name << endl;
    return;
  }

  // Else...we can make it, so we call makeRecipe, a member function of the 
  // Kitchen class, using itr_make and our ostream variable
  kitchen.makeRecipe((*itr_make), ostr);
}

/*
void recipeSuggestions(std::ostream &ostr, const std::list<Recipe> &recipes, const Kitchen &kitchen) {











}*/
 