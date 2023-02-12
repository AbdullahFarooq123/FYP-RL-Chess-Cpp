#include "Fen_utility.h"


string Fen_utility::start_pos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
string Fen_utility::tricky_castle_move_pos_1 = "8/8/8/7k/6q1/8/4B2P/4K2R w K - 0 11";
string Fen_utility::tricky_castle_move_pos_2 ="8/8/8/8/7k/6q1/4B1PP/4K2R w K - 0 11";



Fen_utility::Fen_utility(std::string fen)
{
	this->fen = fen;
}

Fen_utility::~Fen_utility()
{
}

uint64_t Fen_utility::getBitboard(Side side, Piece piece)
{
	string piece_fen = split(fen, ' ')[0];
	uint64_t bitboard = 0ull;
	int row = 0, col = 0;
	for (char c : piece_fen) {
		if (c == '/') {
			++row;
			col = 0;
		}
		else if (isdigit(c)) {
			col += c - '0';
		}
		else {
			if (this->getSideAndPieceBitmap(side, piece, c))
				bitboard |= (1ull << (row * 8 + col));
			++col;
		}
	}
	return bitboard;
}

Side Fen_utility::get_player_turn()
{
	char side = tolower(split(fen, ' ')[1][0]);
	return side == 'b' ? Side::BLACK : Side::WHITE;
}

int Fen_utility::get_enpassant()
{
	string en_passant_square = split(fen, ' ')[3];
	if (en_passant_square != "-")
		return find(str_positions, str_positions + 64, en_passant_square) - str_positions;
	return OUT_OF_BOUNDS;
}

int Fen_utility::get_castle_rights()
{
	string castle_rights = split(fen, ' ')[2];
	int white_king_side_rights = 0;
	int white_queen_side_rights = 0;
	int black_king_side_rights = 0;
	int black_queen_side_rights = 0;
	if (castle_rights.find('k') != string::npos)black_king_side_rights =1;
	if (castle_rights.find('K') != string::npos)white_king_side_rights = 1;
	if (castle_rights.find('q') != string::npos)black_queen_side_rights = 1;
	if (castle_rights.find('Q') != string::npos)white_queen_side_rights = 1;
	return Move::encode_castle_rights(white_king_side_rights,white_queen_side_rights,black_king_side_rights,black_queen_side_rights);
}

bool Fen_utility::getSideAndPieceBitmap(Side side, Piece piece, char fen_char)
{
	if (side != Side::BOTH || piece != Piece::ALL) {
		if (side == Side::WHITE) {
			if (piece == Piece::ALL && isupper(fen_char))
				return true;
			else if ((piece != Piece::ALL) && ((int)fen_char == (int)piece))
				return true;
		}
		else if (side == Side::BLACK) {
			if (piece == Piece::ALL && islower(fen_char))
				return true;
			else if ((piece != Piece::ALL) && ((int)fen_char == (int)piece))
				return true;
		}
		else {
			if (((int)toupper(fen_char) == (int)piece) || ((int)tolower(fen_char) == (int)piece))
				return true;
		}
	}
	else
		return true;
	return false;
}

