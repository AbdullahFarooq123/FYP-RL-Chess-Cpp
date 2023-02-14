#include "Player_state.h"
#include"attribute_utilities.h"


Player_state::Player_state(uint64_t player_state, uint64_t* player_pieces_state, bool my_turn, uint32_t castling_rights, int enpassant_square)
{
	this->player_state = player_state;
	this->player_pieces_state = player_pieces_state;
	this->my_turn = my_turn;
	this->castling_rights = castling_rights;
	this->enpassant_square = enpassant_square;
}

Player_state::~Player_state()
{
}
