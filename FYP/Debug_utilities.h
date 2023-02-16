#pragma once
#include <unordered_map>
#include<iostream>
#include"Engine.h"

class Debug_utilities:protected Engine
{
public:
	Debug_utilities(string fen_string);
	~Debug_utilities();
	void run_make_unmake_debug();
	void engine_normal_run();
	int perft_debug(int depth);
	void perform_custom_perft(int depth);
	void perform_move_generation_debug(int times);
	void print_board_for_fen();
	void print_bitboard_for_fen();
	void print_bitboard_for_piece(PieceName name);

private:
	std::unordered_map<std::string, int> move_generations;
	void get_move_attr_count(Move moves);
	void custom_perft_debug(int depth);
	int nodes = 0;
	int captures = 0;
	int enpassants = 0;
	int castles = 0;
	int promotions = 0;

};

