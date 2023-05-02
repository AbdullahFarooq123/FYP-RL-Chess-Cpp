#pragma once
#include<iostream>
#include<algorithm>
#include"attribute_utilities.h"
#include "general_utilities.h"
#include "Move.h"
using namespace std;
class Fen_utility
{
public:
	Fen_utility(std::string fen);
	~Fen_utility();
	uint64_t getBitboard(Player_Side side=Player_Side::BOTH,Piece piece=Piece::ALL);
	Player_Side get_player_turn();
	int get_enpassant();
	uint8_t get_castle_rights();
public:
	static string start_pos;
	static string kiwipete;
	static string check_mate;
	static string stale_mate;
	static string insufficient_material_bishop;
	static string insufficient_material_knight;
	static string insufficient_material_both_bishop;
	static string insufficient_material_both_knight;
	static string insufficient_material_kings;
	static unordered_map<string, int> move_generation_test_cases;
private:
	std::string fen;
	bool getSideAndPieceBitmap(Player_Side, Piece, char);
};
