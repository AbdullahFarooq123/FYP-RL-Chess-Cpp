#include "pgn_utilities.h"
pgn_utilities::pgn_utilities()
{
}

pgn_utilities::~pgn_utilities()
{
}
std::string pgn_utilities::encode_pgn(uint32_t move) {
	std::string pgn_converted = "";
	Positions target_square = (Positions)Move::decode_move(move, TARGET_SQUARE);
	PieceName piece_name = (PieceName)Move::decode_move(move, PIECE_NAME);
	PieceName promotion_piece_name = (PieceName)Move::decode_move(move, PROMOTION_PIECE_NAME);
	bool capture_flag = (bool)Move::decode_move(move, CAPTURE_FLAG);
	bool castle_flag = (bool)Move::decode_move(move, CASTLE_FLAG);
	if (castle_flag) {
		if (target_square == g8 || target_square == g1)
			return "O-O";
		else
			return "O-O-O";
	}
	if (piece_name != PAWN)
		pgn_converted += std::string(1,piece_unicodes[1][piece_name]);
	if (capture_flag)
		pgn_converted += "#";
	pgn_converted += str_positions[target_square];
	if (promotion_piece_name != NONE)
		pgn_converted += "=" + std::string(1, piece_unicodes[1][promotion_piece_name]);
	return pgn_converted;
}

bool pgn_utilities::decode_pgn(std::string move)
{
	std::string pieces = "PNBRQK";
	if ('O' == move[0]) {
		if ((move.find("O-O") != std::string::npos));
		else if (move.find("O-O-O") != std::string::npos);
		else {
			std::cout << "Invalid move!" << std::endl;
			return false;
		}
		return true;
	}
	if (move[0] == 'P');
	else {
		int index_of_piece = pieces.find(move[0]);
		if (index_of_piece != std::string::npos);
		else {
			std::cout << "Invalid move!" << std::endl;
			return false;
		}
		move = move.substr(1, move.length());
	}
	if (move.find('#') != std::string::npos)
		move = move.substr(1, move.length());
	int index_of_move = std::distance(str_positions, std::find(str_positions, str_positions + 64, move.substr(0, 2)));
	if (index_of_move != 64)
		move = move.substr(2, move.length());
	else {
		std::cout << "Invalid move!" << std::endl;
		return false;
	}
	if (move.length() == 2) {
		int index_of_piece = pieces.find(toupper(move[1]));
		if (index_of_piece != std::string::npos)
			move = move.substr(2, move.length());
		else {
			std::cout << "Invalid move!" << std::endl;
			return false;
		}
	}
	return move.length() == 0;
}
