#include <iostream> //stream class for input and output
#include <fstream> //stream class to read and write to and from files
#include <string>
#include <vector>
#include <cstdlib>

using namespace std;

/*Function replaces the character the user desires to be replaced with the charater 
the user inputs to replace it with*/
void replace(vector<string > &vec, char* initial_type, char* replaced_type)
{
	for (unsigned int i = 0; i < vec.size(); i++)
	{
		for (unsigned int j = 0; j < vec[i].size(); j++)
		{
			if(vec[i][j] == initial_type[0])
			{
				vec[i][j] = replaced_type[0];
			}
		}
	}
}

/*Function performs floodfill operation, replacing pixels of the same cluster based 
on the point and new character the user enters*/
void floodfill(vector<string> &vec, int r, int c, char* replaced_type, char original)
{
	if (vec[r][c] == '.') //Checks to make sure position entered isn't a background character
	{
		cerr << "Position entered not valid" << endl;
		return;
	}

	if (c != vec[r].size()-1 && vec[r][c+1] == original)
	{
		vec[r][c+1] = replaced_type[0];
		floodfill(vec, r, c+1, replaced_type, original);
	}

	if (c != 0 && vec[r][c-1] == original)
	{
		vec[r][c-1] = replaced_type[0];
		floodfill(vec, r, c-1, replaced_type, original);
	}

	if (r != vec.size()-1 && vec[r+1][c] == original)
	{
		vec[r+1][c] = replaced_type[0];
		floodfill(vec, r+1, c, replaced_type, original);
	}

	if (r != 0 && vec[r-1][c] == original)
	{
		vec[r-1][c] = replaced_type[0];
		floodfill(vec, r-1, c, replaced_type, original);	
	}
}

/*Function performes the dilation operation using a plus shaped structuring element, adding a 
pixel if one of the characters of the type the user entered is within the structuring element*/
void dilation(vector<string> &vec, vector<string> &vec_copy, char* type)
{
	for (int i = 0; i < vec.size(); i++)
	{
		for (int j = 0; j < vec[i].size(); j++)
		{	
			if (i == 0)
			{
				if (vec[i][j] == type[0] || vec[i+1][j] == type[0] || vec[i][j-1] == type[0] || vec[i][j+1] == type[0])
				{
					vec_copy[i][j] = type[0];
				}
			}

			else if (i == vec.size()-1)
			{
				if (vec[i][j] == type[0] || vec[i-1][j] == type[0] || vec[i][j-1] == type[0] || vec[i][j+1] == type[0])
				{
					vec_copy[i][j] = type[0];
				}
			}

			else if (vec[i][j] == type[0] || vec[i+1][j] == type[0] ||  vec[i][j+1] == type[0] || vec[i-1][j] == type[0] || vec[i][j-1] == type[0] )
			{
				vec_copy[i][j] = type[0];
			}	
		}
	}
}

/*Function performs the erosion operation using a plus shaped structuring element, replacing a pixel
of the type the user enters with a background character, also entered in by the user*/
void erosion(vector<string> &vec, vector<string> &vec_copy, char* type, char* replaced_type)
{
	for (int i = 0; i < vec.size(); i++)
	{
		for (int j = 0; j < vec[i].size(); j++)
		{	
			if (i == 0) //Prevents segmentation fault by making sure nothing less than 0 is attempted to be accessed
			{
				if (vec[i][j] == type[0] && vec[i+1][j] == type[0] && vec[i][j-1] == type[0] && vec[i][j+1] == type[0])
				{
					vec_copy[i][j] = type[0];
				}
				else
				{
					vec_copy[i][j] = replaced_type[0];
				}
			}

			else if (i == vec.size() - 1)
			{
				if (vec[i][j] == type[0] && vec[i-1][j] == type[0] && vec[i][j-1] == type[0] && vec[i][j+1] == type[0])
				{
					vec_copy[i][j] = type[0];
				}
				else
				{
					vec_copy[i][j] = replaced_type[0];
				}
			}
			else if (vec[i][j] == type[0] && vec[i+1][j] == type[0] && vec[i-1][j] == type[0] && vec[i][j+1] == type[0] && vec[i][j-1] == type[0] )
			{
				vec_copy[i][j] = type[0];
			}	
			else
			{
				vec_copy[i][j] = replaced_type[0];
			}
		}
	}
}
  
//Functoin loops through vector and writes it to an ofstream variable, passed in by reference
void output_file(vector<string> &vec, ofstream &out)
{
	for (int i = 0; i < vec.size(); i++)
	{
		out << vec[i] << "\n";
	}
}

/******** main code ****************************************************/

int main(int argc, char* argv[])
{
	ifstream input (argv[1]); 
	if (!input.is_open()) //checks to make sure file is opened
	{
		cerr << "Failed to open " << argv[1] << endl;
		return 1;
	}

	ofstream output(argv[2]);
	if(!output.is_open()) //checks to make sure file can be written to
	{
		cerr << "Can't open " << argv[2] << " to write." << endl;
		return 1;
	}

	vector<string> lines; 
	string var;

	while (input >> var) 
	{
		lines.push_back(var); //adds each line as an element to the vector lines		
	}

	if (string(argv[3]) == "replace") //user enters replace in command line
	{
		if (argc > 6) //error checking to make sure the right amount of arguments are given
		{
			cerr << "Too many arguments given" << endl;
			return 1;
		}

		replace(lines, argv[4], argv[5]);
		output_file(lines, output);	
	}

	if (string(argv[3]) == "floodfill") //user enters floodfill in command line
	{
		if (argc > 7)
		{
			cerr << "Too many arguments given" << endl;
			return 1;
		}

		int r = atoi(argv[4]); //converts command line input for row into an integer 
		int c = atoi(argv[5]); //converts command line input for column into an integer
		char original = lines[r][c];

		floodfill(lines, r, c, argv[6], original);
		lines[r][c] = argv[6][0]; //sets original point to the replaced type
		output_file(lines, output);
	}

	if (string(argv[3]) == "dilation")
	{
		vector<string> lines_copy(lines);

		if (argc > 6)
		{
			cerr << "Too many arguments given" << endl;
			return 1;
		}

		dilation(lines, lines_copy, argv[4]);
		output_file(lines_copy, output);
	}

	if (string(argv[3]) == "erosion")
	{
		vector<string> lines_copy2(lines);

		if (argc > 7)
		{
			cerr << "Too many arguments given" << endl;
			return 1;
		}

		erosion(lines, lines_copy2, argv[4], argv[5]);
		output_file(lines_copy2, output);
	}

	return 0;
}