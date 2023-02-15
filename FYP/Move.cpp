#include "Move.h"



Move::Move()
{
	this->length = 0;
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
//attacker-piece-name  attacker-position   knight-check  double-check  check 
//    000                 000000               0             0           0
unsigned int Move::encode_check_flag(unsigned int check_count, unsigned int knight_attack, unsigned int attacker_position, unsigned int attacker_piece_name)
{
	return (check_count) | (knight_attack << 2) | (attacker_position << 3) | (attacker_piece_name << 9);
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
		return (check_flag>>2) & 0b1;
	case ATTACKER_POSITION:
		return (check_flag>>3) & 0b111111;
	case ATTACKER_PIECE_NAME:
		return (check_flag>>9) & 0b111;
	default:
		return 0;
	}
}

unsigned int Move::encode_castle_rights(unsigned int W_O_O, unsigned int W_O_O_O, unsigned int B_O_O, unsigned int B_O_O_O)
{
	return W_O_O | (W_O_O_O<<1) | (B_O_O << 2) | (B_O_O_O<<3);
}

unsigned int Move::decode_castle_rights(unsigned int castle_rights, CASTLE_DECODE_ATTRIBUTES attribute_name)
{
	switch (attribute_name)
	{
	case WHITE_KING_SIDE:
		return castle_rights & 0b1;
	case WHITE_QUEEN_SIDE:
		return (castle_rights >> 1) & 0b1;
	case BLACK_KING_SIDE:
		return (castle_rights >> 2) & 0b1;
	case BLACK_QUEEN_SIDE:
		return (castle_rights >> 3) & 0b1;
	case WHITE_BOTH:
		return castle_rights & 0b11;
	case BLACK_BOTH:
		return (castle_rights >> 2) & 0b11;
	default:
		return 0;
	}
}

void Move::add_move(uint32_t move)
{
	moves[length++] = move;
}

void Move::reset_moves()
{
	this->length = 0;
}

int Move::get_length()
{
	return this->length;
}
Iterator Move::begin()
{
	return Iterator(moves);
}

Iterator Move::end()
{
	return Iterator(moves + length);
}

uint32_t Move::operator[](int index)
{
	if (this->length&&index<length)
		return moves[index];
	return -1;
}



