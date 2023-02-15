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
	bool make_move(string move, Player* current_player);
	bool make_move(uint32_t move, Player* current_player);
	void unmake_move();
	bool decode_player_move(string move, PieceName& piece_to_move, Positions& move_from, Positions& move_to, PieceName & promotion_piece);
	bool find_move(PieceName piece_to_move, Positions move_from, Positions move_to, uint32_t& move_found, PieceName& promotion_piece, Player* current_player);
};

