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
	static string kiwipete;
	static string check_mate;
	static string stale_mate;
	static string insufficient_material_bishop;
	static string insufficient_material_knight;
	static string insufficient_material_both_bishop;
	static string insufficient_material_both_knight;
	static string insufficient_material_kings;
	static string tricky_castle_move_pos_1;
	static string tricky_castle_move_pos_2;
	Fen_utility(std::string fen);
	~Fen_utility();
	uint64_t getBitboard(Side side=Side::BOTH,Piece piece=Piece::ALL);
	Side get_player_turn();
	int get_enpassant();
	int get_castle_rights();
	static string get_fen(uint64_t bitboard, uint64_t* white_pieces_state, uint64_t* black_pieces_state);
private:
	std::string fen;
	bool getSideAndPieceBitmap(Side, Piece, char);
};
