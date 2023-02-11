#pragma once
#include "attribute_utilities.cpp"
#include "pre_calculation_utilities.h"
class piece
{
public:
	piece(PieceName, Side, uint64_t, uint64_t*);
	~piece();
	virtual void generate_moves() = 0;
	virtual void generate_attack() = 0;
	void move_piece(uint64_t);
	bool has_moved();
	uint64_t get_position();
protected:
	PieceName piece_name;
	Side piece_color;
	uint64_t piece_position;
	uint64_t piece_moves;
	uint64_t piece_attacks;
	uint64_t *board_state;
	bool piece_moved;
};

