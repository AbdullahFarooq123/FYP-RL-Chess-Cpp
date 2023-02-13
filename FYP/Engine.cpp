#include "Engine.h"
using namespace std;

Engine::Engine()
{
	Fen_utility fen = Fen_utility("rn1qkbnr/pp4pp/3pb3/2p2p2/3p1P2/NBQ4N/PPPPB1PP/R3K2R w KQkq - 0 1");
	this->board_state = fen.getBitboard();
	int enpassant_position = fen.get_enpassant();
	uint64_t white_state = fen.getBitboard(Side::WHITE);
	uint64_t black_state = fen.getBitboard(Side::BLACK);
	int castle_rights = fen.get_castle_rights();
	this->previous_move = 0;
	this->white_turn = fen.get_player_turn();
	uint64_t white_player_bitboards[6];
	uint64_t black_player_bitboards[6];
	Piece white_pieces[6] = { P,N,B,R,Q,K };
	Piece black_pieces[6] = { p,n,b,r,q,k };
	for (int piece = 0; piece < 6; piece++) {
		white_player_bitboards[piece] = fen.getBitboard(Side::WHITE, white_pieces[piece]);
		black_player_bitboards[piece] = fen.getBitboard(Side::BLACK, black_pieces[piece]);
	}
	white_player = new Player(Side::WHITE, &this->board_state, white_state, white_player_bitboards, enpassant_position, &this->previous_move, castle_rights);
	black_player = new Player(Side::BLACK, &this->board_state, black_state, black_player_bitboards, enpassant_position, &this->previous_move, castle_rights);
	white_player->set_opponent_player(black_player);
	black_player->set_opponent_player(white_player);
	init_precalculation_utilities();
}

Engine::~Engine()
{
	delete white_player;
	delete black_player;
}

void Engine::run()
{
	printAsciiBitboard(this->board_state, *white_player, *black_player);
	Player* current_player = this->white_turn ? white_player : black_player;
	current_player->generate_moves();
	current_player->print_moves();
	this->white_turn = !white_turn;
}

//bool Engine::make_move(string move, Player* current_player)
//{
//	PieceName piece_to_move = NONE;
//	Positions move_from = OUT_OF_BOUNDS;
//	Positions move_to = OUT_OF_BOUNDS;
//	uint32_t player_move = 0ul;
//	if (!decode_player_move(move, piece_to_move, move_from, move_to))return false;
//	if (!find_move(piece_to_move, move_from, move_to, player_move, current_player))return false;
//
//}
//
//bool Engine::decode_player_move(string move, PieceName& piece_to_move, Positions& move_from, Positions& move_to)
//{
//	string pieces = "pkbrqk";
//	switch (move.length()) {
//	case 2:
//	{
//		piece_to_move = PAWN;
//		int index_of_move = std::distance(str_positions, std::find(str_positions, str_positions + 64, (move[0] + move[1])));
//		if (index_of_move != 64)
//			move_to = (Positions)index_of_move;
//		else return false;
//	}
//	break;
//	case 4:
//	{
//		piece_to_move = PAWN;
//		int index_of_move_from = std::distance(str_positions, std::find(str_positions, str_positions + 64, (move[0] + move[1])));
//		int index_of_move_to = std::distance(str_positions, std::find(str_positions, str_positions + 64, (move[2] + move[3])));
//		if (index_of_move_from != 64)
//			move_from = (Positions)index_of_move_from;
//		else return false;
//		if (index_of_move_to != 64)
//			move_to = (Positions)index_of_move_to;
//		else return false;
//	}
//	break;
//	case 3:
//	{
//		int index_of_piece = pieces.find(move[0]);
//		if (index_of_piece != string::npos)
//			piece_to_move = (PieceName)index_of_piece;
//		else return false;
//	}
//	break;
//	case 5:
//	{
//		int index_of_piece = pieces.find(move[0]);
//		if (index_of_piece != string::npos)
//			piece_to_move = (PieceName)index_of_piece;
//		else return false;
//		piece_to_move = PAWN;
//		int index_of_move_from = std::distance(str_positions, std::find(str_positions, str_positions + 64, (move[0] + move[1])));
//		int index_of_move_to = std::distance(str_positions, std::find(str_positions, str_positions + 64, (move[2] + move[3])));
//		if (index_of_move_from != 64)
//			move_from = (Positions)index_of_move_from;
//		else return false;
//		if (index_of_move_to != 64)
//			move_to = (Positions)index_of_move_to;
//		else return false;
//	}
//	break;
//	default:
//		return false;
//	}
//	return true;
//}
//
//bool Engine::find_move(PieceName piece_to_move, Positions move_from, Positions move_to, uint32_t move_found, Player * current_player)
//{
//	bool move_already_found = false;
//
//	for (uint32_t move : current_player->get_moves()) {
//		PieceName move_piece_name = (PieceName)Move::decode_move(move, MOVE_DECODE_ATTRIBUTES::PIECE_NAME);
//		if (move_piece_name == piece_to_move) {
//			Positions move_piece_from = (Positions)Move::decode_move(move, MOVE_DECODE_ATTRIBUTES::SOURCE_SQUARE);
//			Positions move_piece_to = (Positions)Move::decode_move(move, MOVE_DECODE_ATTRIBUTES::TARGET_SQUARE);
//			if (move_piece_to == move_to) {
//				if (move_from == OUT_OF_BOUNDS || move_from == move_piece_from) {
//					if (move_already_found)return false;
//					move_already_found = true;
//					move_found = move;
//				}
//			}
//		}
//	}
//	return true;
//}
