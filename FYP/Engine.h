#pragma once
#include<stack>
#include "game_pretty.h"
#include "Fen_utility.h"
#include "Player.h"
#include "Game_state.h"
class Engine
{
public:
	Engine();
	~Engine();
	void run();
protected:
	Player* white_player;
	Player* black_player;
	uint64_t board_state;
	uint32_t previous_move;
	std::stack<Game_state> prev_states;
	bool white_turn;
	bool make_move(string move, Player* current_player, Player* opponent_player);
	bool make_move(uint32_t move, Player* current_player, Player* opponent_player);
	void unmake_move();
	bool find_move(string move, uint32_t& move_found, Player* current_player);
};

