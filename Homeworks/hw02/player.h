//File: player.h
//Header file for the Player class 

#ifndef __player_h_ // Include guards
#define __player_h_

#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

class Player
{
public:
	//Constructors
	Player(string aFirstName, string aLastName);
	
	//Accessors
	const string& getFirstName() const {return first_name_;}
	const string& getLastName() const {return last_name_;}
	int getMatchWins() const {return match_wins_;}
	int getMatchLosses() const {return match_losses_;}
	int getGameWins() const {return game_wins_;}
	int getGameLosses() const {return game_losses_;}
	int getComebackWins() const {return comeback_wins_;}
	double getPctMatchWins() const {return pct_match_wins_;}
	double getPctGameWins() const {return pct_game_wins_;}

	//Modifiers
	void addMatchWin() {match_wins_ ++;}
	void addMatchLoss() {match_losses_ ++;}
	void addGameWins(int gw) {game_wins_ += gw;}
	void addGameLosses(int gl) {game_losses_ += gl;}
	void addComebackWin() {comeback_wins_ ++;}

	//Member Functions
	double pct_match_wins();
	double pct_game_wins();
	void match_line(ostream &out, int len);
	void game_line(ostream &out, int len);
	void comeback_line(ostream &out, int len);

private:
	string first_name_;
	string last_name_;
	int match_wins_;
	int match_losses_;
	int game_wins_;
	int game_losses_;
	int comeback_wins_;
	double pct_match_wins_;
	double pct_game_wins_;
};

//Sorting Functions
bool match_percentage(const Player& p1, const Player& p2);
bool game_percentage(const Player& p1, const Player& p2);
bool comeback_sort(const Player& p1, const Player& p2);

#endif