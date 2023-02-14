#include "Game_state.h"


Game_state::Game_state(Player_state white_state, Player_state black_state, uint64_t board_state, uint32_t previous_move, bool white_turn):white_state(white_state),black_state(black_state)
{
	this->board_state = board_state;
	this->previous_move = previous_move;
	this->white_turn = white_turn;
}

Game_state::~Game_state()
{
}
