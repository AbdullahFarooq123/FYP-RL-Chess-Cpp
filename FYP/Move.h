#pragma once
#include<iostream>
#include"attribute_utilities.h"
#include"Iterator.h"
enum MOVE_DECODE_ATTRIBUTES {
	SOURCE_SQUARE,
	TARGET_SQUARE,
	PIECE_NAME,
	PROMOTION_PIECE_NAME,
	CAPTURE_FLAG,
	DOUBLE_PUSH_FLAG,
	EN_PASSANT_FLAG,
	CASTLE_FLAG
};
enum CHECK_DECODE_ATTRIBUTES {
	CHECK,
	DOUBLE_CHECK,
	BOTH_CHECK,
	KNIGHT_CHECK,
	ATTACKER_POSITION,
	ATTACKER_PIECE_NAME
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
	static int decode_move(uint32_t move, MOVE_DECODE_ATTRIBUTES attribute_name);
	static unsigned int encode_check_flag(unsigned int check_count, 
										  unsigned int knight_flags, 
										  unsigned int attacker_position, 
										  unsigned int attacker_piece_name);
	static unsigned int decode_check_flag(unsigned int check_flag, CHECK_DECODE_ATTRIBUTES attribute_name);
	void add_move(uint32_t move);
	void reset_moves();
	int get_move_count();
	Iterator begin();
	Iterator end();
private:
	uint32_t moves[256];
	int move_index;
};


