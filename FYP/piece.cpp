#include "piece.h"

piece::piece(PieceName name, Side color, uint64_t position, uint64_t* board_state) {
	this->piece_color = color;
	this->piece_name = name;
	this->piece_position = position;
	this->board_state = board_state;
	this->piece_moves = 0;
	this->piece_moved = false;
}


piece::~piece()
{
}

void piece::move_piece(uint64_t move) {
	*this->board_state &= ~(1ull << (int)log2(this->piece_position));
	*this->board_state |= 1ull << (int)log2(move);
	*this->board_state &= ~(1ull << (int)log2(this->piece_position));
	*this->board_state |= 1ull << (int)log2(move);
}
bool piece::has_moved() {
	return this->piece_moved;
}
uint64_t piece::get_position() {
	return this->piece_position;
}
