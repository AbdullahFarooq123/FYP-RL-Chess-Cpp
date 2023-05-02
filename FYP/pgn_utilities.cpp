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

bool pgn_utilities::parse_pgn_move(string move, uint32_t& return_move, Player* current_player, bool white_turn)
{
	std::string pieces = "PNBRQK";
	std::string files = "abcdefgh";
	std::string ranks = "12345678";

	PieceName piece_to_move = NONE;
	PieceName promotion_piece_name = NONE;

	Positions source_square = OUT_OF_BOUNDS;
	Positions target_square = OUT_OF_BOUNDS;
	int file = -1;
	int rank = -1;
	move.erase(remove(move.begin(), move.end(), 'x'), move.end());
	move.erase(remove(move.begin(), move.end(), '#'), move.end());
	move.erase(remove(move.begin(), move.end(), '+'), move.end());
	if ('O' == move[0]) {
		source_square = white_turn ? e1 : e8;
		piece_to_move = KING;
		if (move == "O-O")
			target_square = white_turn ? g1 : g8;
		else if (move == "O-O-O")
			target_square = white_turn ? c1 : c8;
		else {
			std::cout << "Invalid move!" << std::endl;
			return false;
		}
		move = "";
	}
	else {

		if (move.find('=') != string::npos && (move.length() == 4 || move.length() == 5)) {
			piece_to_move = PAWN;
			int promotion_piece_index = pieces.find(move[move.length() - 1]);
			if (promotion_piece_index != string::npos) {
				promotion_piece_name = (PieceName)promotion_piece_index;
				move = move.substr(0, move.length() - 2);
				if (move.length() == 3) {
					int index_of_file = files.find(move[0]);
					if (index_of_file != std::string::npos) {
						file = index_of_file;
						move = move.substr(1, move.length());
					}
					else {
						int index_of_rank = ranks.find(move[0]);
						if (index_of_rank != std::string::npos) {
							rank = index_of_rank + 1;
							move = move.substr(1, move.length());
						}
					}
				}
			}
			else {
				std::cout << "Invalid move!" << std::endl;
				return false;
			}
		}
		if (move.length() == 2) {
			piece_to_move = PAWN;
		}
		else if (move.length() == 3) {
			int index_of_piece = pieces.find(move[0]);
			if (index_of_piece != std::string::npos) {
				piece_to_move = (PieceName)index_of_piece;
				move = move.substr(1, move.length());
			}
			else {
				int index_of_file = files.find(move[0]);
				if (index_of_file != std::string::npos) {
					piece_to_move = PAWN;
					file = index_of_file;
					move = move.substr(1, move.length());
				}
				else {
					std::cout << "Invalid move!" << std::endl;
					return false;
				}
			}
		}
		else if (move.length() == 4) {
			int index_of_piece = pieces.find(move[0]);
			if (index_of_piece != std::string::npos) {
				piece_to_move = (PieceName)index_of_piece;
				move = move.substr(1, move.length());
			}
			else {
				std::cout << "Invalid move!" << std::endl;
				return false;
			}
			int index_of_file = files.find(move[0]);
			if (index_of_file != std::string::npos) {
				file = index_of_file;
				move = move.substr(1, move.length());
			}
			else {
				int index_of_rank = ranks.find(move[0]);
				if (index_of_rank != std::string::npos) {
					rank = index_of_rank + 1;
					move = move.substr(1, move.length());
				}
			}
		}
		int index_of_target_squre = std::distance(str_positions, std::find(str_positions, str_positions + 64, move.substr(0, 2)));
		if (index_of_target_squre != 64)
			target_square = (Positions)index_of_target_squre;
		else {
			cout << "Invalid Move!" << endl;
			return false;
		}
	}

	for (uint32_t move : current_player->get_moves()) {
		PieceName current_piecename = (PieceName)Move::decode_move(move, MOVE_DECODE_ATTRIBUTES::PIECE_NAME);
		if (current_piecename == piece_to_move) {
			bool this_move = false;
			Positions my_target = (Positions)Move::decode_move(move, MOVE_DECODE_ATTRIBUTES::TARGET_SQUARE);
			if (source_square == OUT_OF_BOUNDS && file != -1 && promotion_piece_name == NONE) {
				int current_file = Move::decode_move(move, MOVE_DECODE_ATTRIBUTES::SOURCE_SQUARE) % 8;
				if (current_file == file && my_target == target_square)
					this_move = true;
			}
			else if (source_square == OUT_OF_BOUNDS && rank != -1 && promotion_piece_name == NONE) {
				Positions source_square = (Positions)Move::decode_move(move, MOVE_DECODE_ATTRIBUTES::SOURCE_SQUARE);
				int current_rank = 8 - (source_square / 8);
				if (current_rank == rank && my_target == target_square)
					this_move = true;
			}
			else if (source_square != OUT_OF_BOUNDS) {
				Positions my_source = (Positions)Move::decode_move(move, MOVE_DECODE_ATTRIBUTES::SOURCE_SQUARE);
				if (my_target == target_square && my_source == source_square) {
					this_move = true;
				}
			}
			else if (promotion_piece_name != NONE) {
				PieceName current_promotion_piecename = (PieceName)Move::decode_move(move, MOVE_DECODE_ATTRIBUTES::PROMOTION_PIECE_NAME);
				if (my_target == target_square && promotion_piece_name == current_promotion_piecename && (file == -1 && rank == -1))
					this_move = true;
				else if (file != -1) {
					int current_file = Move::decode_move(move, MOVE_DECODE_ATTRIBUTES::SOURCE_SQUARE) % 8;
					if (current_file == file && my_target == target_square && promotion_piece_name == current_promotion_piecename)
						this_move = true;
				}
				else if (rank != -1) {
					Positions source_square = (Positions)Move::decode_move(move, MOVE_DECODE_ATTRIBUTES::SOURCE_SQUARE);
					int current_rank = 8 - (source_square / 8);
					if (current_rank == rank && my_target == target_square && promotion_piece_name == current_promotion_piecename)
						this_move = true;
				}
			}
			else if (my_target == target_square)
				this_move = true;
			if (this_move) {
				return_move = move;
				return true;
			}
		}
	}
	cout << "Move " << move << " not found!" << endl;
	return false;
}
