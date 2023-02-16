#include "Engine.h"
#include "pgn_utilities.h"

using namespace std;

Engine::Engine(string fen_string)
{
	Fen_utility fen = Fen_utility(fen_string);
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
	white_player = new Player(Side::WHITE, &this->board_state, white_state, white_player_bitboards, white_turn ? enpassant_position : OUT_OF_BOUNDS, &this->previous_move, castle_rights);
	black_player = new Player(Side::BLACK, &this->board_state, black_state, black_player_bitboards, !white_turn ? enpassant_position : OUT_OF_BOUNDS, &this->previous_move, castle_rights);
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
	
	while (true) {
		printAsciiBitboard(this->board_state, *white_player, *black_player);
		Player* current_player = this->white_turn ? white_player : black_player;
		Player* opponent_player = !this->white_turn ? white_player : black_player;
		current_player->generate_moves();
		if (is_game_over(current_player,opponent_player))
			break;
		current_player->print_moves();
		bool correct_move = false;
		string move;
		do {
			cout << (this->white_turn ? "WHITE's" : "BLACK's") << " TURN!\n";
			cout << "Please enter your move : ";
			cin >> move;
		} while (!(make_move(move, current_player,opponent_player)));
		white_turn = !white_turn;
		system("cls");
	}
}

bool Engine::make_move(string move, Player * current_player, Player * opponent_player)
{
	uint32_t player_move = 0ul;
	if (!find_move(move, player_move, current_player)) return false;
	return this->make_move(player_move,current_player,opponent_player);
}

bool Engine::make_move(uint32_t move, Player * current_player, Player* opponent_player)
{
	Player_state white_state = Player_state(this->white_player->get_player_state(), this->white_player->get_deep_copy_pieces(), this->white_player->get_castling_rights(), *this->white_player->get_enpassant_square());
	Player_state black_state = Player_state(this->black_player->get_player_state(), this->black_player->get_deep_copy_pieces(), this->black_player->get_castling_rights(), *this->white_player->get_enpassant_square());
	this->prev_states.push(Game_state(white_state, black_state, this->board_state, this->previous_move, this->white_turn));
	Positions target_square = (Positions)Move::decode_move(move, MOVE_DECODE_ATTRIBUTES::TARGET_SQUARE);
	Positions source_square = (Positions)Move::decode_move(move, MOVE_DECODE_ATTRIBUTES::SOURCE_SQUARE);
	bool castle_flag = (bool)Move::decode_move(move, CASTLE_FLAG);
	bool capture_flag = (bool)Move::decode_move(move, CAPTURE_FLAG);
	bool en_passant_flag = (bool)Move::decode_move(move, EN_PASSANT_FLAG);
	uint64_t* opponent_pieces = opponent_player->get_player_pieces();
	if (capture_flag) {
		uint64_t capture_mask = bitmask(target_square);
		bool found = false;
		for (int piece_name = PAWN; piece_name <= QUEEN; piece_name++) {
			uint64_t* opponent_piece_mask = &opponent_pieces[piece_name];
			if (*opponent_piece_mask & capture_mask) {
				*opponent_piece_mask &= ~capture_mask;
				opponent_player->remove_piece_from_player_state(capture_mask);
				found = true;
				break;
			}
		}
		if (!found)return false;
	}
	else if (en_passant_flag) {
		uint64_t move_bitmask = bitmask(target_square);
		uint64_t enpassant_opponent_piece_mask = this->white_turn ? move_bitmask << 8 : move_bitmask >> 8;
		bool found = false;
		uint64_t* opponent_piece_mask = &opponent_pieces[PAWN];
		if (*opponent_piece_mask & enpassant_opponent_piece_mask) {
			*opponent_piece_mask &= ~enpassant_opponent_piece_mask;
			opponent_player->remove_piece_from_player_state(enpassant_opponent_piece_mask);
			*current_player->get_enpassant_square() = OUT_OF_BOUNDS;
			uint64_t opponent_state = opponent_player->get_player_state();
			this->board_state &= ~enpassant_opponent_piece_mask;

			found = true;
		}
		if (!found)return false;
	}
	else if (castle_flag) {
		Positions rook_from = OUT_OF_BOUNDS;
		Positions rook_to = OUT_OF_BOUNDS;
		if (this->white_turn) {
			if (target_square == g1) {
				rook_from = h1;
				rook_to = f1;
			}
			else if (target_square == c1) {
				rook_from = a1;
				rook_to = d1;
			}
		}
		else {
			if (target_square == g8) {
				rook_from = h8;
				rook_to = f8;
			}
			else if (target_square == c8) {
				rook_from = a8;
				rook_to = d8;
			}
		}
		this->board_state &= ~bitmask(rook_from);
		this->board_state |= bitmask(rook_to);
	}
	this->board_state &= ~bitmask(source_square);
	this->board_state |= bitmask(target_square);
	current_player->make_move(move);
	this->previous_move = move;
	return true;
}

void Engine::unmake_move()
{
	if (!prev_states.empty()) {
		Game_state game_state = prev_states.top();
		this->previous_move = game_state.previous_move;
		this->board_state = game_state.board_state;
		this->white_player->set_state(game_state.white_state);
		this->black_player->set_state(game_state.black_state);
		delete[] game_state.white_state.player_pieces_state;
		delete[] game_state.black_state.player_pieces_state;
		this->white_turn = game_state.white_turn;
		prev_states.pop();
	}
}

bool Engine::find_move(string move, uint32_t & move_found, Player * current_player)
{
	if (!pgn_utilities::decode_pgn(move)) return false;
	move_found = current_player->get_moves().get_pgn_moves()[move];
	return true;
}

bool Engine::is_game_over(Player* current_player, Player* opponent_player)
{
	if (current_player->is_player_in_check() && current_player->get_moves().get_length() == 0) {
		cout << "CHECK MATE!\n" << (this->white_turn ? "BLACK" : "WHITE") << " WINS\n";
		return true;
	}
	else if (!current_player->is_player_in_check() && current_player->get_moves().get_length() == 0) {
		cout << "DRAW\nBY STALE MATE\n";
		return true;
	}
	else {
		bool no_pawns = false;
		bool no_rooks = false;
		bool no_bishops = false;
		bool no_queens = false;
		bool no_knights = false;
		if (!current_player->get_player_pieces()[PAWN] && !opponent_player->get_player_pieces()[PAWN])
			no_pawns = true;
		if (!current_player->get_player_pieces()[BISHOP] && !opponent_player->get_player_pieces()[BISHOP])
			no_bishops = true;
		if (!current_player->get_player_pieces()[QUEEN] && !opponent_player->get_player_pieces()[QUEEN])
			no_queens = true;
		if (!current_player->get_player_pieces()[ROOK] && !opponent_player->get_player_pieces()[ROOK])
			no_rooks = true;
		if (!current_player->get_player_pieces()[KNIGHT] && !opponent_player->get_player_pieces()[KNIGHT])
			no_knights = true;
		if (no_bishops && no_pawns && no_rooks && no_queens && no_knights) {
			cout << "DRAW\nBY INSUFFICIENT MATERIAL\n";
			return true;
		}
		else if (no_pawns && no_rooks && no_queens && no_knights) {
			cout << "DRAW\nBY INSUFFICIENT MATERIAL\n";
			return true;
		}
		else if (no_bishops && no_pawns && no_rooks && no_queens) {
			cout << "DRAW\nBY INSUFFICIENT MATERIAL\n";
			return true;
		}
		else {
			return false;
		}
	}

	return false;
}
