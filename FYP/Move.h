#pragma once
#include<iostream>
#include"attribute_utilities.h"
#include"Iterator.h"
enum DECODE_ATTRIBUTES {
	SOURCE_SQUARE,
	TARGET_SQUARE,
	PIECE_NAME,
	PROMOTION_PIECE_NAME,
	CAPTURE_FLAG,
	DOUBLE_PUSH_FLAG,
	EN_PASSANT_FLAG,
	CASTLE_FLAG
};
class Move
{
public:
	Move();
	~Move();
	static uint32_t encode_move(uint32_t source_square, 
								uint32_t target_square, 
								uint32_t piece_name, 
								uint32_t promotion_piece_name, 
								uint32_t capture_flag, 
								uint32_t double_push_flag, 
								uint32_t enpassant_flag, 
								uint32_t castle_flag);
	static int decode_move(uint32_t move, DECODE_ATTRIBUTES attribute_name);
	void add_move(uint32_t move);
	void reset_moves();
	int get_move_count();
	Iterator begin();
	Iterator end();
private:
	uint32_t moves[256];
	int move_index;
};


