#include "pgn_utilities.h"
pgn_utilities::pgn_utilities()
{
}

pgn_utilities::~pgn_utilities()
{
}
std::string pgn_utilities::encode_pgn(uint32_t move) {
	std::string pgn_converted = "";
	Positions source_square = (Positions)Move::decode_move(move, SOURCE_SQUARE);
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
	pgn_converted += std::string(1, piece_unicodes[1][piece_name]);
	pgn_converted += str_positions[source_square];
	if (capture_flag)
		pgn_converted += "x";
	pgn_converted += str_positions[target_square];
	if (promotion_piece_name != NONE)
		pgn_converted += "=" + std::string(1, piece_unicodes[1][promotion_piece_name]);
	return pgn_converted;
}

bool pgn_utilities::decode_pgn(std::string move)
{
	std::string pieces = "PNBRQK";
	if ('O' == move[0]) {
		if (move == "O-O");
		else if (move == "O-O-O");
		else {
			std::cout << "Invalid move!" << std::endl;
			return false;
		}
		return true;
	}
	int index_of_piece = pieces.find(move[0]);
	if (index_of_piece != std::string::npos)move = move.substr(1, move.length());
	else {
		std::cout << "Invalid move!" << std::endl;
		return false;
	}
	
	int index_of_move_from = std::distance(str_positions, std::find(str_positions, str_positions + 64, move.substr(0, 2)));
	if (index_of_move_from != 64)
		move = move.substr(2, move.length());
	else {
		std::cout << "Invalid move!" << std::endl;
		return false;
	}
	if (move.find('x') != std::string::npos)
		move = move.substr(1, move.length());
	int index_of_move_to = std::distance(str_positions, std::find(str_positions, str_positions + 64, move.substr(0, 2)));
	if (index_of_move_to != 64)
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
