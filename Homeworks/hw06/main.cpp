#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <string>
#include <vector>
#include <cassert>

#include "MersenneTwister.h"

#include "tile.h"
#include "location.h"
#include "board.h"


// this global variable is set in main.cpp and is adjustable from the command 
// line (you are not allowed to make your own global variables)
int GLOBAL_TILE_SIZE = 11;


// ==========================================================================
// Helper function that is called when an error in the command line
// arguments is detected.
void usage(int argc, char *argv[]) {
  std::cerr << "USAGE: " << std::endl;
  std::cerr << "  " << argv[0] << " <filename>  -board_dimensions <h> <w>" << std::endl;
  std::cerr << "  " << argv[0] << " <filename>  -board_dimensions <h> <w>  -all_solutions" << std::endl;
  std::cerr << "  " << argv[0] << " <filename>  -board_dimensions <h> <w>  -allow_rotations" << std::endl;
  std::cerr << "  " << argv[0] << " <filename>  -all_solutions  -allow_rotations" << std::endl;
  std::cerr << "  " << argv[0] << " <filename>  -tile_size <odd # >= 11>" << std::endl;
  exit(1);
}

// Helper function to determine whether a given location is already in a certain
// vector of locations
// ==========================================================================
bool in_solution(Location loc, const std::vector<Location> &locations)
{
  for (int i = 0; i < locations.size(); i++)
  {
    if (loc == locations[i]) return true;
  }

  return false;
}

// Helper function checks the edges of the tile to place and the tile it's 
// bordering to make sure the edges equal each other
// ==========================================================================
bool EdgesMatch(Board board, Tile* to_place, Location loc, 
  const std::vector<Location> &locations)
{
  // Four if statements to determine which direction the tile is placed
  // and then to check if the edge is the same
  if (in_solution(Location(loc.row-1,loc.column,0), locations) && 
    (loc.row-1 >= 0) && 
    (board.getTile(loc.row-1,loc.column)->getSouth() != to_place->getNorth()))
  {
    return false;
  }

  if (in_solution(Location(loc.row+1,loc.column,0), locations) && 
    (loc.row+1 < board.numRows()) &&
    (board.getTile(loc.row+1,loc.column)->getNorth() != to_place->getSouth()))
  {
    return false;
  }

  if (in_solution(Location(loc.row,loc.column-1,0), locations) && 
    (loc.column-1 >= 0) &&
    (board.getTile(loc.row,loc.column-1)->getEast() != to_place->getWest()))
  {
    return false;
  }

  if ((in_solution(Location(loc.row,loc.column+1,0), locations) && 
    loc.column+1 < board.numColumns()) &&
    (board.getTile(loc.row,loc.column+1)->getWest() != to_place->getEast()))
  {
    return false;
  }

  return true;
}

// Once a solution has been found by the recursion, this helper function checks
// to see if all of the edges of the board don't have a road going off of it,
// as well as making sure no roads go to a location where there is no tile, 
// indicated by the spot in the board being NULL
//=============================================================================
bool BaseCaseRoadCheck(Board &board, std::vector<Location> &locations, 
  std::vector<Tile*> &tiles)
{
  // Loops through all tiles on the board -> O(t)
  for (int i = 0; i < locations.size(); i++)
  {
    // Checks borders and the roads
    if ((locations[i].column == 0 && tiles[i]->getWest() == "road") ||
        (locations[i].row == 0 && tiles[i]->getNorth() == "road") ||
        (locations[i].column == (board.numColumns()-1) && 
          tiles[i]->getEast() == "road") ||
        (locations[i].row == (board.numRows()-1) && 
          tiles[i]->getSouth() == "road"))
    {
      return false;
    }

    // Following four if statements check to make sure a road isn't connecting 
    // to somewhere a tile doesn't exist

    if (locations[i].row != 0 && tiles[i]->getNorth() != "pasture" &&
        (board.getTile(locations[i].row-1,locations[i].column) == NULL))
    {
      return false;
    }

    if (locations[i].row != board.numRows()-1 && 
        tiles[i]->getSouth() != "pasture" &&
        (board.getTile(locations[i].row+1,locations[i].column) == NULL))
    {
      return false;
    }

    if (locations[i].column != 0 && tiles[i]->getWest() != "pasture" &&
        (board.getTile(locations[i].row,locations[i].column-1) == NULL))
    {
      return false;
    }

    if (locations[i].column != board.numColumns()-1 && 
        tiles[i]->getEast() != "pasture" &&
        (board.getTile(locations[i].row,locations[i].column+1) == NULL))
    {
      return false;
    }  
  }

  return true;
}

// Recursive function to find a possible solution given the tiles
// ==========================================================================
bool FindSolution(Board &board, std::vector<Tile*> &tiles, Location loc, 
  std::vector<Location> &locations, int size)
{
  // Start by pushing back the current location, and setting it on the board
  locations.push_back(loc);
  board.setTile(loc.row, loc.column, tiles[locations.size()-1]);

  //Base Case: if the locations vector size is equal to the size of the tiles
  //vector, we have found a solution
  if (locations.size() == size)
  {
    // Checks to see if the base case road check returns true, if it does
    // it's a solution
    if (BaseCaseRoadCheck(board,locations,tiles))
    {
      return true;
    }
    // Pop back the last tile and try again
    else
    {
      board.removeTile(locations[locations.size()-1].row, 
        locations[locations.size()-1].column);
      locations.pop_back();
      return false;
    }
  }
  
  // Loop through the tiles that are already on the board
  for (int t = locations.size(); t >= 0; t--)
  {
    // Next two loops check the possible positions to place the tile
    for (int i = locations[t-1].row - 1; i <= locations[t-1].row + 1; i++)
    {  
      for (int j = locations[t-1].column - 1; j <= locations[t-1].column + 1; j++)
      {
        //makes sure we skip the tile we are on
        if (i == locations[t-1].row && j == locations[t-1].column) continue;
        
        //if the location is already in the locations vector, then continue
        if (in_solution(Location(i,j,0), locations)) continue;

        // makes sure every point on the board we check is actually on the board
        if (i < 0 || i >= board.numRows()) continue;
        if (j < 0 || j >= board.numColumns()) continue;

        //skips positions that are diagonal to location we are on,
        //so that only max 4 positions are checked 
        if ((i == locations[t-1].row - 1 && j == locations[t-1].column - 1) ||
            (i == locations[t-1].row + 1 && j == locations[t-1].column - 1) ||
            (i == locations[t-1].row - 1 && j == locations[t-1].column + 1) ||
            (i == locations[t-1].row + 1 && j == locations[t-1].column + 1))
            continue; 

        bool road_check = true;
        bool edge_check = true;

        //checks to make sure tiles on the edge of the board don't have roads
        //going to places where they can't be connected with other tiles
        if ((j == 0 && tiles[locations.size()]->getWest() == "road") ||
            (i == 0 && tiles[locations.size()]->getNorth() == "road") ||
            (j == (board.numColumns()-1) && 
              tiles[locations.size()]->getEast() == "road") ||
            (i == (board.numRows()-1) && 
              tiles[locations.size()]->getSouth() == "road"))
        {
          road_check = false;
        }
        
        // Boolean for whether the edges match
        edge_check = EdgesMatch(board, tiles[locations.size()], 
          Location(i,j,0), locations);
        
        if (road_check && edge_check)
        {
          if (FindSolution(board, tiles, Location(i,j,0), locations, size))
          {
            return true;
          }
        }
      }
    }
  }

  // If we reach this point, we know that we cannot continue on with this tile
  // so remove it, pop it back from locations, and try again
  board.removeTile(locations[locations.size()-1].row, 
    locations[locations.size()-1].column);
  locations.pop_back();
  return false;
}

// ==========================================================================
void HandleCommandLineArguments(int argc, char *argv[], std::string &filename, 
                                int &rows, int &columns, bool &all_solutions,
                                bool &allow_rotations) 
{

  // must at least put the filename on the command line
  if (argc < 2) {
    usage(argc,argv);
  }
  filename = argv[1];

  // parse the optional arguments
  for (int i = 2; i < argc; i++) {
    if (argv[i] == std::string("-tile_size")) {
      i++;
      assert (i < argc);
      GLOBAL_TILE_SIZE = atoi(argv[i]);
      if (GLOBAL_TILE_SIZE < 11 || GLOBAL_TILE_SIZE % 2 == 0) {
        std::cerr << "ERROR: bad tile_size" << std::endl;
        usage(argc,argv);
      }
    } else if (argv[i] == std::string("-all_solutions")) {
      all_solutions = true;
    } else if (argv[i] == std::string("-board_dimensions")) {
      i++;
      assert (i < argc);
      rows = atoi(argv[i]);
      i++;
      assert (i < argc);
      columns = atoi(argv[i]);
      if (rows < 1 || columns < 1) {
        usage(argc,argv);
      }
    } else if (argv[i] == std::string("-allow_rotations")) {
      allow_rotations = true;
    } else {
      std::cerr << "ERROR: unknown argument '" << argv[i] << "'" << std::endl;
      usage(argc,argv);
    }
  }
}


// ==========================================================================
void ParseInputFile(int argc, char *argv[], const std::string &filename, 
  std::vector<Tile*> &tiles) {

  // open the file
  std::ifstream istr(filename.c_str());
  if (!istr) {
    std::cerr << "ERROR: cannot open file '" << filename << "'" << std::endl;
    usage(argc,argv);
  }
  assert (istr);

  // read each line of the file
  std::string token, north, east, south, west;
  while (istr >> token >> north >> east >> south >> west) {
    assert (token == "tile");
    Tile *t = new Tile(north,east,south,west);
    tiles.push_back(t);
  }
}

// ==========================================================================
bool IsTranslation(const Board &board, const std::vector<Location> &solution1, 
  const std::vector<Location> &solution2)
{
  // 4 vectors of the rows and columns of solution1 and the rows and columns of
  // solution2
  std::vector<int> rows1;
  std::vector<int> columns1;
  std::vector<int> rows2;
  std::vector<int> columns2;

  for (int k = 0; k < solution1.size(); k++)
  {
    rows1.push_back(solution1[k].row);
    columns1.push_back(solution1[k].column);
    rows2.push_back(solution2[k].row);
    columns2.push_back(solution2[k].column);
  }

  // two variables, for the difference in the rows of the rows for the two
  // solutions, and the difference in the columns
  int diff1 = rows2[0]-rows1[0];
  int diff2 = columns2[0]-columns1[0];

  // if the differences aren't the same throughout the solution, then we do not
  // have a translation
  for (int i = 0; i < rows1.size(); i++)
  {
    if ((rows2[i] - rows1[i] == diff1) && (columns2[i] - columns1[i] == diff2))
    {
      continue;
    }
    else
    {
      return false;
    }
  }

  return true;
}

// ==========================================================================
int main(int argc, char *argv[]) {

  std::string filename;
  int rows = -1;
  int columns = -1;
  bool all_solutions = false;
  bool allow_rotations = false;
  HandleCommandLineArguments(argc, argv, filename, rows, columns, all_solutions,
   allow_rotations);

  // load in the tiles
  std::vector<Tile*> tiles;
  ParseInputFile(argc,argv,filename,tiles);

  if (rows == -1 && columns == -1)
  {
    rows = tiles.size()/2;
    columns = tiles.size()/2;
  }
  // confirm the specified board is large enough
  if (rows < 1  ||  columns < 1  ||  rows * columns < tiles.size()) {
    std::cerr << "ERROR: specified board is not large enough" << rows 
              << "X" << columns << "=" << rows*columns << " " << tiles.size() 
              << std::endl;
    usage(argc,argv);
  }

  Board board(rows,columns);
  std::vector<Location> locations;
  int size = tiles.size();
  bool path_found = false;

  // if all solutions is entered in the command line...
  if (all_solutions)
  {
    // 2D vector of locations to store all the solutions
    std::vector<std::vector<Location> > all_solutions;
    // counter to display the number of solutions
    int c = 0;

    // nested loop to go through and check all locations of the board: O(hw)
    for (int row = 0; row < board.numRows(); row++)
    {
      for (int column = 0; column < board.numColumns(); column++)
      {
        if (FindSolution(board, tiles, Location(row, column, 0), locations, size)) 
        {
          c++;
          // push back the solution in all solutions
          all_solutions.push_back(locations);

          // if we have more than one solution in all solutions, check to see
          // if it's a translation, and if it is...
          if (c > 1 && IsTranslation(board, all_solutions[all_solutions.size()-2],
           all_solutions.back()))
          {
            // remove it from all solutions, clear the board, and continue
            locations.clear();
            board.ClearBoard();
            c--;
            all_solutions.pop_back();
            continue;
          }

          // else, output it
          std::cout << "Solution: ";
          for (int i = 0; i < locations.size(); i++) {
            std::cout << locations[i];
          }
          std::cout << std::endl;
          locations.clear();
          board.Print();
          board.ClearBoard();

          if (c > 3) {path_found = true; break;}
          continue;
        }

        if (path_found) break;
      }
    }

    std::cout << "Found " << c << " Solution(s)." << std::endl;
  }
  else
  {
    for (int row = 0; row < board.numRows(); row++)
    {
      for (int column = 0; column < board.numColumns(); column++)
      {
        if (FindSolution(board, tiles, Location(row, column, 0), locations, size)) 
        {
          path_found = true;
          break;
        }
      }
      if (path_found) break;
    }
    
    if (path_found)
    {
      // print the solution
      std::cout << "Solution: ";
      for (int i = 0; i < locations.size(); i++) {
        std::cout << locations[i];
      }
      std::cout << std::endl;
      // print the ASCII art board representation
      board.Print();
      std::cout << std::endl; 
    }
    else
    {
      std::cout << "No Solution." << std::endl;
    }
  }
  
  // delete the tiles
  for (int t = 0; t < tiles.size(); t++) {
    delete tiles[t];
  }
}
// ==========================================================================