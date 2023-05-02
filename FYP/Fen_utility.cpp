#include "Fen_utility.h"

string Fen_utility::start_pos = "rnbqkbnr/pppppppp/8/8/8/8/PPPPPPPP/RNBQKBNR w KQkq - 0 1";
string Fen_utility::kiwipete = "r3k2r/p1ppqpb1/bn2pnp1/3PN3/1p2P3/2N2Q1p/PPPBBPPP/R3K2R w KQkq - ";
string Fen_utility::check_mate = "8/8/8/8/8/7k/7q/7K w - - 0 1";
string Fen_utility::stale_mate = "8/8/8/8/8/8/5q2/5k1K w - - 0 1";
string Fen_utility::insufficient_material_bishop = "8/b4k2/8/8/8/8/8/7K w - - 0 1";
string Fen_utility::insufficient_material_knight = "8/5k2/2n5/8/8/8/8/7K w - - 0 1";
string Fen_utility::insufficient_material_both_bishop = "8/b4k2/8/8/8/8/8/4B2K w - - 0 1";
string Fen_utility::insufficient_material_both_knight = "8/5k2/2n5/8/8/8/3N4/7K w - - 0 1";
string Fen_utility::insufficient_material_kings = "8/5k2/8/8/8/8/8/7K w - - 0 1";
unordered_map<string, int> Fen_utility::move_generation_test_cases{
	{"4k3/8/8/8/8/4P3/8/4K3 w - - 0 1", 6}, // pawn moves
	{ "4k3/8/8/8/2p1P3/2P1P3/8/4K3 w - - 0 1", 6 }, // pawn blocked by piece
	{ "4k3/8/8/8/8/3KP2r/8/8 w - - 0 1",7 }, // pawn pinned
	{ "4k3/8/8/8/8/8/4P3/3K4 w - - 0 1", 6 }, // double push
	{ "4k3/8/8/8/2p5/4P3/2P1P3/3K4 w - - 0 1",5 }, // pawn blocked by a piece
	{ "4k3/8/8/8/6b1/8/4P3/3K4 w - - 0 1",4 }, // pawn pinned
	{ "4k3/8/8/3P1p2/4P3/8/8/3K4 w - - 0 1",8 }, // pawn capture moves
	{ "4k3/8/8/8/8/5p2/3KP2r/8 w - - 0 1",7 },  // pawn pinned
	{ "3rk3/2P5/8/8/8/8/8/3K4 w - - 0 1",8 },  // pawn promotion
	{ "4k3/8/8/5pP1/8/8/3K4/8 w - f6 0 2",10 }, // en passant
	{ "3k4/4b3/8/1pP5/1K6/8/8/8 w - b6 0 3",5 }, //  en passant when pinned
	{ "4k3/1KP3r1/8/8/8/8/8/8 w - - 0 1",7 }, // pawn promotion when pinned
	{ "4k3/8/8/8/8/3B4/8/4K3 w - - 0 1",16 },  //Bishop moves
	{ "4k3/8/8/8/8/8/2K5/1B6 w - - 0 1",8 }, //Bishop blocked by any piece
	{ "4k3/8/8/8/8/3K4/p7/1B6 w - - 0 1",10 }, // bishop_capture_moves
	{ "4k3/8/8/8/8/8/8/4KB1r w - - 0 1",4 }, // bishop_pinned
	{ "4k3/8/8/8/8/8/2B5/4K2r w - - 0 1",3 }, // check
	{ "4k3/8/8/8/4N3/8/8/4K3 w - - 0 1",13 }, // knight_moves
	{ "4k3/8/8/8/8/3N4/8/4K3 w - - 0 1",12 }, // knight blocked by any piece
	{ "4k3/8/8/4p3/1n3r2/3N4/8/4K3 w - - 0 1",10 }, // knight_capture
	{ "4k3/8/8/8/5b2/4N3/3K4/8 w - - 0 1",7 },  // knight_pinned
	{ "4k3/8/8/8/3N1b2/8/3K4/8 w - - 0 1",6 }, // check
	{ "4k3/8/8/8/8/3Q4/8/3K4 w - - 0 1",29 }, // Queen_moves
	{ "4k3/8/8/8/8/3Q4/8/3K4 w - - 0 1",29 }, // Queen blocked by any piece
	{ "4k3/8/8/5n2/3p4/1b1Q4/8/3K4 w - - 0 1",6 }, //Queen_capture
	{ "4k3/8/8/8/8/8/8/2QK2r1 w - - 0 1",3 },  // check
	{ "1k6/8/q7/8/8/8/4Q3/5K2 w - - 0 1",8 },  //Queen_pinned
	{ "1k6/8/8/8/8/7b/6Q1/5K2 w - - 0 1",5 }, // Queen_pinned
	{ "4k3/8/8/8/8/8/8/R3K3 w - - 0 1",15 },  // rook_moves
	{ "4k3/8/8/8/8/8/8/3RK3 w - - 0 1",14 },  // rook blocked by any piece
	{ "4k3/8/8/8/8/1p6/8/1Rn1K3 w - - 0 1",8 }, //rook_capture
	{ "4k3/8/8/8/7b/8/5R2/4K3 w - - 0 1",4 },  // rook_pinned
	{ "4k3/8/8/8/7b/8/2R5/4K3 w - - 0 1",5 },  // check
	{ "4k3/8/8/8/8/8/8/R3K2R w KQ - 0 1",26 }, // castling
	{ "8/8/4k3/8/8/8/8/Rn2K2R w KQ - 1 1",22 }, // castling
	{ "4k3/8/8/8/8/3K4/8/8 w - - 0 1",8 }, // king_move
	{ "4k3/8/8/8/8/8/4P3/4K3 w - - 0 1",6 },  // king blocked by a piece
	{ "4k3/8/8/8/8/8/4p3/4K3 w - - 0 1",3 },  // king capture
	{ "8/8/4k3/8/8/8/8/R3K2R w - - 5 3",24 },   // castling when king is moved
	{ "r3k3/8/8/8/8/8/8/4K2R w Kq - 0 1",15 }, // castling
};

Fen_utility::Fen_utility(std::string fen)
{
	this->fen = fen;
}

Fen_utility::~Fen_utility()
{
}

uint64_t Fen_utility::getBitboard(Player_Side side, Piece piece)
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

Player_Side Fen_utility::get_player_turn()
{
	char side = tolower(split(fen, ' ')[1][0]);
	return side == 'b' ? Player_Side::BLACK : Player_Side::WHITE;
}

int Fen_utility::get_enpassant()
{
	string en_passant_square = split(fen, ' ')[3];
	if (en_passant_square != "-")
		return find(str_positions, str_positions + 64, en_passant_square) - str_positions;
	return OUT_OF_BOUNDS;
}

uint8_t Fen_utility::get_castle_rights()
{
	string castle_rights = split(fen, ' ')[2];
	int white_king_side_rights = 0;
	int white_queen_side_rights = 0;
	int black_king_side_rights = 0;
	int black_queen_side_rights = 0;
	if (castle_rights.find('k') != string::npos)black_king_side_rights = 1;
	if (castle_rights.find('K') != string::npos)white_king_side_rights = 1;
	if (castle_rights.find('q') != string::npos)black_queen_side_rights = 1;
	if (castle_rights.find('Q') != string::npos)white_queen_side_rights = 1;
	return Move::encode_castle_rights(white_king_side_rights, white_queen_side_rights, black_king_side_rights, black_queen_side_rights);
}

bool Fen_utility::getSideAndPieceBitmap(Player_Side side, Piece piece, char fen_char)
{
	if (side != Player_Side::BOTH || piece != Piece::ALL) {
		if (side == Player_Side::WHITE) {
			if (piece == Piece::ALL && isupper(fen_char))
				return true;
			else if ((piece != Piece::ALL) && ((int)fen_char == (int)piece))
				return true;
		}
		else if (side == Player_Side::BLACK) {
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

