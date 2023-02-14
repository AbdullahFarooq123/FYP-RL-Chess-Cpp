#pragma once
#include<iostream>
#include"Player_state.h"
class Game_state
{
public:
	Game_state(Player_state white_state, Player_state black_state, uint64_t board_state, uint32_t previous_move);
	~Game_state();
	Player_state white_state;
	Player_state black_state;
	uint64_t board_state;
	uint32_t previous_move;
};

