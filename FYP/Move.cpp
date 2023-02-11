#include "Move.h"



Move::Move()
{
	this->move_index = 0;
}


Move::~Move()
{
}

uint32_t Move::encode_move(uint32_t source_square, uint32_t target_square, uint32_t piece_name, uint32_t promotion_piece_name, uint32_t capture_flag, uint32_t double_push_flag, uint32_t enpassant_flag, uint32_t castle_flag)
{
	return source_square | (target_square << 6) | (piece_name << 12) | (promotion_piece_name << 16) | (capture_flag << 20) | (double_push_flag << 21) | (enpassant_flag << 22) | (castle_flag << 23);
}

int Move::decode_move(uint32_t move, DECODE_ATTRIBUTES attribute_name)
{
	switch (attribute_name)
	{
	case SOURCE_SQUARE:
		return move & 0b111111;
	case TARGET_SQUARE:
		return (move >> 6) & 0b111111;
	case PIECE_NAME:
		return (move >> 12) & 0b1111;
	case PROMOTION_PIECE_NAME:
		return (move >> 16) & 0b1111;
	case CAPTURE_FLAG:
		return (move >> 20) & 0b1;
	case DOUBLE_PUSH_FLAG:
		return (move >> 21) & 0b1;
	case EN_PASSANT_FLAG:
		return (move >> 22) & 0b1;
	case CASTLE_FLAG:
		return (move >> 23) & 0b1;
	default:
		return 0;
	}
}

void Move::add_move(uint32_t move)
{
	moves[move_index++] = move;
}

void Move::reset_moves()
{
	this->move_index = 0;
}

int Move::get_move_count()
{
	return this->move_index;
}
Iterator Move::begin()
{
	return Iterator(moves);
}

Iterator Move::end()
{
	return Iterator(moves+move_index);
}



