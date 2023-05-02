#pragma once
#include <unordered_map>
#include<iostream>
#include"Engine.h"

class Debug_utilities:protected Engine
{
public:
	Debug_utilities(string fen_string=Fen_utility::start_pos);
	~Debug_utilities();
	void run_make_unmake_debug();
	void engine_normal_run();
	int perft_debug(int depth);
	void run_pgn_game(vector<string> game, int game_no);
	void debug_move_generation(int moves_tested);
private:
	string fen_string;
};

