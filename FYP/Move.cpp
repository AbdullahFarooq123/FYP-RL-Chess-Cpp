#include "Move.h"
#include"pgn_utilities.h"

Move::Move()
{
	this->length = 0;
}

Move::~Move()
{
}

void Move::add_move(uint32_t move)
{
	move_map[pgn_utilities::encode_pgn(move)] = move;
	moves[length++] = move;
}

void Move::reset_moves()
{
	move_map.clear();
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

uint32_t Move::get_move_from_map(std::string move_key)
{
	return this->move_map[move_key];
}



