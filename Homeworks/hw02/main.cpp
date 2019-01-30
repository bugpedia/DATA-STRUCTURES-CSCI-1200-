//File: main.cpp

#include <fstream>
#include <cstdlib>
#include <algorithm>
#include "player.h"

using namespace std;

// Parses a string that represents a set (i.e., "6-3") by breaking the
// string into two substrings and converting those strings to
// integers, which are returned via call-by-reference parameters
void parse_set(string &set, int &games_won, int &games_lost) 
{
	int i = set.find('-');
	games_won = atoi(set.substr(0,i).c_str());
	games_lost = atoi(set.substr(i+1,set.size()-i-1).c_str());
}

// Reads file containing information about tennis matches into a 
// vector of vector of strings, making sure one line is a vector
void parse_file(ifstream &input, vector<vector<string> > &vec)
{
	vector<string> temp;
	string w_first, w_last, d, l_first, l_last;
	string g1, g2, g3, g4, g5;

	// Since every line has at least the name of the winner and loser
	// with a "d." in between, these strings are pushed back to the 
	// vector of strings initialized as "temp"
	while (input >> w_first >> w_last >> d >> l_first >> l_last)
	{
		temp.push_back(w_first);
		temp.push_back(w_last);
		temp.push_back(d);
		temp.push_back(l_first);
		temp.push_back(l_last);

		// A minimum of three games are played in a best of 5 tennis match
		while (input >> g1 >> g2 >> g3) 
		{
			temp.push_back(g1);
			temp.push_back(g2);
			temp.push_back(g3);

			// If the winner won the first 3 sets, push back temp to the larger vector and
			// go to the next line in the file
			if ((g1[0]-'0' > g1[2]-'0') && (g2[0]-'0' > g2[2]-'0') && (g3[0]-'0' > g3[2]-'0'))
			{
				vec.push_back(temp);
				temp.clear();
				break;
			}

			input >> g4;
			temp.push_back(g4);

			// If the winner has won 3 sets, then push back temp to the larger vector
			if (((g4[0]-'0' > g4[2]-'0') && (g1[0]-'0' > g1[2]-'0') && (g2[0]-'0' > g2[2]-'0'))|| 
				((g4[0]-'0' > g4[2]-'0') && (g2[0]-'0' > g2[2]-'0') && (g3[0]-'0' > g3[2]-'0')) ||
				((g4[0]-'0' > g4[2]-'0') && (g1[0]-'0' > g1[2]-'0') && (g3[0]-'0' > g3[2]-'0')))
			{
				vec.push_back(temp);
				temp.clear();
				break;
			}

			// Else, if it took 5 sets, input the 5th game and push back temp to the
			// larger vector, and move to the next line
			input >> g5;
			temp.push_back(g5);
			vec.push_back(temp);
			temp.clear();
			break;
		}
	}
}

// Uses the file data already parsed into a vector of vector of stirngs to declare 
// Player objects, pushed back into a vector of Player objects
void add_player(vector<Player> &players, vector<vector<string> > &file_data)
{
	// Creates the first two Player objects from the first line of data
	players.push_back(Player(file_data[0][0], file_data[0][1]));
	players.push_back(Player(file_data[0][3], file_data[0][4])); 

	// Uses class member functions to set the games won and lost for 
	// these first two players
	for (int i = 5; i < file_data[0].size(); i++)
	{
		int games_won = 0;
		int games_lost = 0;
		parse_set(file_data[0][i], games_won, games_lost);
		players[0].addGameWins(games_won);
		players[0].addGameLosses(games_lost);
		players[1].addGameWins(games_lost);
		players[1].addGameLosses(games_won);
	}

	players[0].addMatchWin();
	players[1].addMatchLoss();

	for (int i = 1; i < file_data.size(); i++)
	{
		bool found1 = false; // Initializes boolean values for if player exists to be false
		bool found2 = false;

		for (int j = 0; j < players.size(); j++)
		{
			if(file_data[i][0] == players[j].getFirstName() && file_data[i][1] == players[j].getLastName())
			{
				players[j].addMatchWin();

				// Deals with 3rd statistic, seeing if the player who won lost his first two sets
				if (file_data[i][5][0]-'0' < file_data[i][5][2]-'0' && file_data[i][6][0]-'0' < file_data[i][6][2]-'0')
				{
					players[j].addComebackWin();
				}

				// Adds amount of games won and lost to the player
				for (int k = 5; k < file_data[i].size(); k++)
				{
					int games_won = 0;
					int games_lost = 0;
					parse_set(file_data[i][k], games_won, games_lost);
					players[j].addGameWins(games_won);
					players[j].addGameLosses(games_lost);
				}

				// Sets boolean to true so that player isn't added to the Player vector again
				found1 = true;
			}
			
			// Same as above if statement, but for player who lost
			if(file_data[i][3] == players[j].getFirstName() && file_data[i][4] == players[j].getLastName())
			{
				players[j].addMatchLoss();

				for (int k = 5; k < file_data[i].size(); k++)
				{
					int games_won = 0;
					int games_lost = 0;
					parse_set(file_data[i][k], games_won, games_lost);
					players[j].addGameWins(games_lost);
					players[j].addGameLosses(games_won);
				}

				found2 = true;
			}
		}

		// If game winner was not found in the list of players...
		if (found1 == false)
		{
			players.push_back(Player(file_data[i][0], file_data[i][1]));
			players[players.size()-1].addMatchWin();

			if (file_data[i][5][0]-'0' < file_data[i][5][2]-'0' && file_data[i][6][0]-'0' < file_data[i][6][2]-'0')
			{
				players[players.size()-1].addComebackWin();
			}

			for (int k = 5; k < file_data[i].size(); k++)
			{
				int games_won = 0;
				int games_lost = 0;
				parse_set(file_data[i][k], games_won, games_lost);
				players[players.size()-1].addGameWins(games_won);
				players[players.size()-1].addGameLosses(games_lost);
			}
		}

		// If game loser was not found in the list of players...
		if (found2 == false)
		{
			players.push_back(Player(file_data[i][3], file_data[i][4]));
			players[players.size()-1].addMatchLoss();

			for (int k = 5; k < file_data[i].size(); k++)
			{
				int games_won = 0;
				int games_lost = 0;
				parse_set(file_data[i][k], games_won, games_lost);
				players[players.size()-1].addGameWins(games_lost);
				players[players.size()-1].addGameLosses(games_won);
			}
		}
	}
}

// Function outputs to a file the header for the table
void table_header(string s, ofstream &output, int len)
{
	output << s << " STATISTICS" << endl;
	output << "Player" << setw(len) << "W"<< setw(5);
	output << "L" << setw(5) << setw(13) << "percentage" << endl;
}

int main(int argc, char* argv[])
{

	if (argc != 3)
	{
		cerr << "Too many command line arguments" << endl;
		return 1;
	}

	ifstream input (argv[1]); 
	if (!input.is_open()) // Checks to make sure file is opened
	{
		cerr << "Failed to open " << argv[1] << endl;
		return 1;
	}

	ofstream output (argv[2]);
	if(!output.is_open()) // Checks to make sure file can be written to
	{
		cerr << "Can't open " << argv[2] << " to write." << endl;
		return 1;
	}

	vector<Player> players;
	vector<vector<string> > file_data;

	parse_file(input, file_data);
	add_player(players, file_data);

	int len = 0; //initializes name length variable to 0

	for (int i = 0; i < players.size(); i++)
	{
		players[i].pct_match_wins(); // Calculates percentages of match wins for all players
		players[i].pct_game_wins(); // Calculates percentages of game wins for all players

		// Finds max name length out of all players and sets it to len
		if ((players[i].getFirstName().size() + players[i].getLastName().size()) > len)
		{
			len = players[i].getFirstName().size() + players[i].getLastName().size();
		}
	}

	sort(players.begin(), players.end(), match_percentage);

	table_header("MATCH", output, len);
	for (int i = 0; i < players.size(); i++)
	{
		int new_len = len-(players[i].getFirstName().size())-(players[i].getLastName().size()-5);
		players[i].match_line(output, new_len);
	}

	sort(players.begin(), players.end(), game_percentage);

	output << endl;
	table_header("GAME", output, len);
	for (int i = 0; i < players.size(); i++)
	{
		int new_len = len-(players[i].getFirstName().size())-(players[i].getLastName().size()-5);
		players[i].game_line(output, new_len);
	}

	sort(players.begin(), players.end(), comeback_sort);

	for (int i = 0; i < players.size(); i++)
	{
		if (players[i].getComebackWins() > 0)
		{
			// Ensures comeback statistics only shows up if there is a player with a comeback win
			if (i == 0) 
			{
				output << endl;
				output << "COMEBACK STATISTICS" << endl;
				output << "Player" << setw(len) << "W"<< setw(5) << endl;
			}

			int new_len = len-(players[i].getFirstName().size())-(players[i].getLastName().size()-5);
			players[i].comeback_line(output, new_len);
		}
	}

	return 0; 
}