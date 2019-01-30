//File: tennis.cpp
//Implementation file for the Tennis class

#include <iostream>
#include <iomanip>
#include <string>
#include <vector>
#include "player.h"

// Constructor...initializes variables
Player::Player(string aFirstName, string aLastName)
{
	first_name_ = aFirstName;
	last_name_ = aLastName;
	match_wins_ = 0;
	match_losses_ = 0;
	game_wins_ = 0;
	game_losses_ = 0;
	comeback_wins_ = 0;
	pct_match_wins_ = 0;
	pct_game_wins_ = 0;
}

double Player::pct_match_wins()
{
	pct_match_wins_ = (match_wins_ / (double(match_wins_) + match_losses_));
	return pct_match_wins_;
}

double Player::pct_game_wins()
{
	pct_game_wins_ = (game_wins_ / (double(game_wins_) + game_losses_));
	return pct_game_wins_;
}

void Player::match_line(ostream &out, int len)
{
	out << first_name_ << " " << last_name_ << setw(len);
	out << match_wins_ << setw(5);
	out << match_losses_ << setw(5);
	out << setw(13) << fixed << setprecision(3) << pct_match_wins_ << endl;
}

void Player::game_line(ostream &out, int len)
{
	out << first_name_ << " " << last_name_ << setw(len);
	out << game_wins_ << setw(5);
	out << game_losses_ << setw(5);
	out << setw(13) << fixed << setprecision(3) << pct_game_wins_ << endl;
}

void Player::comeback_line(ostream &out, int len)
{
	out << first_name_ << " " << last_name_ << setw(len);
	out << comeback_wins_ << setw(4) << endl;
}

bool match_percentage(const Player& p1, const Player& p2)
{
	return (p1.getPctMatchWins() > p2.getPctMatchWins() ||
		(p1.getPctMatchWins() == p2.getPctMatchWins() && p1.getLastName() < p2.getLastName()) ||
		(p1.getPctMatchWins() == p2.getPctMatchWins() && p1.getLastName() == p2.getLastName() 
			&& p1.getFirstName() < p2.getFirstName()));
}

bool game_percentage(const Player& p1, const Player& p2)
{
	return (p1.getPctGameWins() > p2.getPctGameWins() || 
		(p1.getPctGameWins() == p2.getPctGameWins() && 	p1.getLastName() < p2.getLastName()) ||
		(p1.getPctGameWins() == p2.getPctGameWins() && p1.getLastName() == p2.getLastName() &&
			p1.getFirstName() < p2.getFirstName()));
}

bool comeback_sort(const Player& p1, const Player& p2)
{
	return (p1.getComebackWins() > p2.getComebackWins() || 
		(p1.getComebackWins() == p2.getComebackWins() && p1.getLastName() < p2.getLastName()) || 
		(p1.getComebackWins() == p2.getComebackWins() && p1.getLastName() == p2.getLastName() && 
			p1.getFirstName() < p1.getFirstName()));
}