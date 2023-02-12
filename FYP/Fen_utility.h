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
	static string start_pos;
	static string tricky_castle_move_pos_1;
	static string tricky_castle_move_pos_2;
	Fen_utility(std::string fen);
	~Fen_utility();
	uint64_t getBitboard(Side side=Side::BOTH,Piece piece=Piece::ALL);
	Side get_player_turn();
	int get_enpassant();
	int get_castle_rights();
private:
	std::string fen;
	bool getSideAndPieceBitmap(Side, Piece, char);
};
