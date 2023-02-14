#pragma once
#include<algorithm>
#include"attribute_utilities.h"
class Player_state
{
public:
	Player_state(uint64_t player_state, uint64_t* player_pieces_state , bool my_turn, uint32_t castling_rights, int enpassant_square);
	~Player_state();
	uint64_t player_state;
	uint64_t* player_pieces_state;
	bool my_turn;
	uint32_t castling_rights;
	int enpassant_square;
};

