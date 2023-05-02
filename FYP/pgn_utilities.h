#pragma once
#include<iostream>
#include"Move.h"
#include"Player.h"
using namespace std;
class pgn_utilities
{
public:
	pgn_utilities();
	~pgn_utilities();
	static std::string encode_pgn(uint32_t move);
	static bool decode_pgn(std::string move);
	static bool parse_pgn_move(std::string move, uint32_t& return_move, Player* current_player, bool white_turn);
};


