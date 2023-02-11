#pragma once
#include "game_pretty.h"
#include "Fen_utility.h"
#include "Player.h"
class Engine
{
public:
	Engine();
	~Engine();
	void run();
private:
	Player* white_player;
	Player* black_player;
	uint64_t board_state;
	uint32_t previous_move;
	bool white_turn;
};

