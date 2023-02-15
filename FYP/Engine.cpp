#include "Engine.h"
#include "pgn_utilities.h"

using namespace std;

Engine::Engine()
{
	Fen_utility fen = Fen_utility("2k5/8/8/8/8/8/8/R3K2R w KQ - 0 11");
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
		current_player->print_moves();
		bool correct_move = false;
		string move;
		do {
			cout << (this->white_turn ? "WHITE's" : "BLACK's") << " TURN!\n";
			cout << "Please enter your move : ";
			cin >> move;
		} while (!(make_move(move, current_player,opponent_player)));
		//cout << "PREVIOUS DOUBLE PUSH (AFTER MAKE MOVE) : " << Move::decode_move(previous_move, MOVE_DECODE_ATTRIBUTES::DOUBLE_PUSH_FLAG) << endl;
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
