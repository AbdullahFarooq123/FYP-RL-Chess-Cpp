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
	bool make_move(string move, Player* current_player);
	bool decode_player_move(string move, PieceName& piece_to_move, Positions& move_from, Positions& move_to);
	bool find_move(PieceName piece_to_move, Positions move_from, Positions move_to, uint32_t move_found, Player* current_player);
};

