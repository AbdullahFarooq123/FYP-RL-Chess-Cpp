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

int Move::decode_move(uint32_t move, MOVE_DECODE_ATTRIBUTES attribute_name)
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

unsigned int Move::encode_check_flag(unsigned int check_count, unsigned int knight_attack, unsigned int attacker_position, unsigned int attacker_piece_name)
{
	return (check_count) | (knight_attack << 2) | (attacker_position << 3) | (attacker_piece_name << 8);
}

unsigned int Move::decode_check_flag(unsigned int check_flag, CHECK_DECODE_ATTRIBUTES attribute_name)
{
	switch (attribute_name)
	{
	case CHECK:
		return check_flag & 0b1;
	case DOUBLE_CHECK:
		return (check_flag>>1) & 0b1;
	case BOTH_CHECK:
		return check_flag & 0b11;
	case KNIGHT_CHECK:
		return (check_flag>>3) & 0b1;
	case ATTACKER_POSITION:
		return (check_flag>>4) & 0b111111;
	case ATTACKER_PIECE_NAME:
		return (check_flag>>10) & 0b111;
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
	return Iterator(moves + move_index);
}


