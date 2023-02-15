#include "Engine.h"
using namespace std;

Engine::Engine()
{
	Fen_utility fen = Fen_utility("2b2k2/1P6/8/6b1/8/8/8/R3K2R w KQ - 0 11");
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
	white_player = new Player(Side::WHITE, &this->board_state, white_state, white_player_bitboards, white_turn?enpassant_position:OUT_OF_BOUNDS, &this->previous_move, castle_rights);
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
		current_player->generate_moves();
		current_player->print_moves();
		bool correct_move = false;
		string move;
		do {
			cout << (this->white_turn ? "WHITE's" : "BLACK's") << " TURN!\n";
			cout << "Please enter your move : ";
			cin >> move;
		} while (!(make_move(move, current_player)));
		white_turn = !white_turn;
		system("cls");
	}
}

bool Engine::make_move(string move, Player * current_player)
{
	Player_state white_state = Player_state(this->white_player->get_player_state(),this->white_player->get_deep_copy_pieces(),this->white_player->get_castling_rights(),*this->white_player->get_enpassant_square());
	Player_state black_state = Player_state(this->black_player->get_player_state(), this->black_player->get_deep_copy_pieces(), this->black_player->get_castling_rights(), *this->white_player->get_enpassant_square());
	this->prev_states.push(Game_state(white_state,black_state,this->board_state,this->previous_move, this->white_turn));
	PieceName piece_to_move = NONE;
	PieceName promotion_piece = NONE;
	Positions move_from = OUT_OF_BOUNDS;
	Positions move_to = OUT_OF_BOUNDS;
	uint32_t player_move = 0ul;
	if (!decode_player_move(move, piece_to_move, move_from, move_to,promotion_piece))return false;
	if (!find_move(piece_to_move, move_from, move_to, player_move,promotion_piece, current_player))return false;
	bool capture_flag = (bool)Move::decode_move(player_move, CAPTURE_FLAG);
	bool en_passant_flag = (bool)Move::decode_move(player_move, EN_PASSANT_FLAG);
	uint64_t* opponent_pieces = current_player->get_opponent_player()->get_player_pieces();
	if (capture_flag) {
		uint64_t capture_mask = bitmask(move_to);
		bool found = false;
		for (int piece_name = PAWN; piece_name <= QUEEN; piece_name++) {
			uint64_t* opponent_piece_mask = &opponent_pieces[piece_name];
			if (*opponent_piece_mask & capture_mask) {
				*opponent_piece_mask &= ~capture_mask;
				*current_player->get_opponent_player()->get_ptr_player_state() &= ~capture_mask;
				found = true;
				break;
			}
		}
		if (!found)return false;
	}
	else if (en_passant_flag) {
		Positions enpassant_capture_square = (Positions)(move_to + (this->white_turn ? +8 : -8));
		uint64_t enpassant_mask = bitmask(enpassant_capture_square);
		bool found = false;
		for (int piece_name = PAWN; piece_name <= QUEEN; piece_name++) {
			uint64_t* opponent_piece_mask = &opponent_pieces[piece_name];
			if (*opponent_piece_mask & enpassant_mask) {
				*opponent_piece_mask &= ~enpassant_mask;
				*current_player->get_opponent_player()->get_ptr_player_state() &= ~enpassant_mask;
				*current_player->get_enpassant_square() = OUT_OF_BOUNDS;
				found = true;
				break;
			}
		}
		if (!found)return false;
	}
	this->board_state &= ~bitmask(move_from);
	this->board_state |= bitmask(move_to);
	current_player->make_move(player_move);
	this->previous_move = player_move;
	return true;
}

bool Engine::make_move(uint32_t move, Player * current_player)
{
	Player_state white_state = Player_state(this->white_player->get_player_state(), this->white_player->get_deep_copy_pieces(), this->white_player->get_castling_rights(), *this->white_player->get_enpassant_square());
	Player_state black_state = Player_state(this->black_player->get_player_state(), this->black_player->get_deep_copy_pieces(), this->black_player->get_castling_rights(), *this->white_player->get_enpassant_square());
	this->prev_states.push(Game_state(white_state, black_state, this->board_state, this->previous_move, this->white_turn));
	Positions target_square = (Positions)Move::decode_move(move, MOVE_DECODE_ATTRIBUTES::TARGET_SQUARE);
	Positions source_square = (Positions)Move::decode_move(move, MOVE_DECODE_ATTRIBUTES::SOURCE_SQUARE);
	bool capture_flag = (bool)Move::decode_move(move, CAPTURE_FLAG);
	bool en_passant_flag = (bool)Move::decode_move(move, EN_PASSANT_FLAG);
	uint64_t* opponent_pieces = current_player->get_opponent_player()->get_player_pieces();
	if (capture_flag) {
		uint64_t capture_mask = bitmask(target_square);
		bool found = false;
		for (int piece_name = PAWN; piece_name <= QUEEN; piece_name++) {
			uint64_t* opponent_piece_mask = &opponent_pieces[piece_name];
			if (*opponent_piece_mask & capture_mask) {
				*opponent_piece_mask &= ~capture_mask;
				*current_player->get_opponent_player()->get_ptr_player_state() &= ~capture_mask;
				found = true;
				break;
			}
		}
		if (!found)return false;
	}
	else if (en_passant_flag) {
		Positions enpassant_capture_square = (Positions)(target_square + (this->white_turn ? +8 : -8));
		uint64_t enpassant_mask = bitmask(enpassant_capture_square);
		bool found = false;
		for (int piece_name = PAWN; piece_name <= QUEEN; piece_name++) {
			uint64_t* opponent_piece_mask = &opponent_pieces[piece_name];
			if (*opponent_piece_mask & enpassant_mask) {
				*opponent_piece_mask &= ~enpassant_mask;
				*current_player->get_opponent_player()->get_ptr_player_state() &= ~enpassant_mask;
				*current_player->get_enpassant_square() = OUT_OF_BOUNDS;
				found = true;
				break;
			}
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

bool Engine::decode_player_move(string move, PieceName & piece_to_move, Positions & move_from, Positions & move_to, PieceName& promotion_piece)
{
	string pieces = "PNBRQK";
	switch (move.length()) {
	case 2:
	{
		piece_to_move = PAWN;
		int index_of_move = std::distance(str_positions, std::find(str_positions, str_positions + 64, move.substr(0, 2)));
		if (index_of_move != 64)
			move_to = (Positions)index_of_move;
		else {
			cout << "Move not found!\nPlease be specific for the move" << endl;
			return false;
		}
	}
	break;
	case 4:
	{
		piece_to_move = PAWN;
		int index_of_move_from = std::distance(str_positions, std::find(str_positions, str_positions + 64, move.substr(0, 2)));
		int index_of_move_to = std::distance(str_positions, std::find(str_positions, str_positions + 64, move.substr(2, 2)));
		if (index_of_move_from != 64)
			move_from = (Positions)index_of_move_from;
		else {
			cout << "Move not found!\nPlease be specific for the move" << endl;
			return false;
		}
		if (index_of_move_to != 64)
			move_to = (Positions)index_of_move_to;
		else {
			cout << "Move not found!\nPlease be specific for the move" << endl;
			return false;
		}
	}
	break;
	case 3:
	{
		int index_of_piece = pieces.find(move[0]);
		if (index_of_piece != string::npos)
			piece_to_move = (PieceName)index_of_piece;
		else {
			cout << "Move not found!\nPlease be specific for the move" << endl;
			return false;
		}
	}
	break;
	case 5:
	{
		int index_of_piece = pieces.find(move[0]);
		if (index_of_piece != string::npos)
			piece_to_move = (PieceName)index_of_piece;
		else {
			cout << "Move not found!\nPlease be specific for the move" << endl;
			return false;
		}
		int index_of_move_from = std::distance(str_positions, std::find(str_positions, str_positions + 64, move.substr(0, 2)));
		int index_of_move_to = std::distance(str_positions, std::find(str_positions, str_positions + 64, move.substr(2, 2)));
		if (index_of_move_from != 64)
			move_from = (Positions)index_of_move_from;
		else {
			cout << "Move not found!\nPlease be specific for the move" << endl;
			return false;
		}
		if (index_of_move_to != 64)
			move_to = (Positions)index_of_move_to;
		else {
			cout << "Move not found!\nPlease be specific for the move" << endl;
			return false;
		}
	}
	break;
	case 6:
	{
		int index_of_piece = pieces.find(move[0]);
		if (index_of_piece != string::npos)
			piece_to_move = (PieceName)index_of_piece;
		else {
			cout << "Move not found!\nPlease be specific for the move" << endl;
			return false;
		}
		int index_of_promotion_piece = pieces.find(move[5]);
		if (index_of_promotion_piece != string::npos)
			promotion_piece = (PieceName)index_of_promotion_piece;
		int index_of_move_from = std::distance(str_positions, std::find(str_positions, str_positions + 64, move.substr(1, 2)));
		int index_of_move_to = std::distance(str_positions, std::find(str_positions, str_positions + 64, move.substr(3, 2)));
		if (index_of_move_from != 64)
			move_from = (Positions)index_of_move_from;
		else {
			cout << "Move not found!\nPlease be specific for the move" << endl;
			return false;
		}
		if (index_of_move_to != 64)
			move_to = (Positions)index_of_move_to;
		else {
			cout << "Move not found!\nPlease be specific for the move" << endl;
			return false;
		}
	}
	break;
	default:
		cout << "Move Error!\nPlease be specific for the move" << endl;
		return false;
	}
	return true;
}

bool Engine::find_move(PieceName piece_to_move, Positions move_from, Positions move_to, uint32_t & move_found, PieceName& promotion_piece, Player * current_player)
{
	bool move_already_found = false;

	for (uint32_t move : current_player->get_moves()) {
		PieceName move_piece_name = (PieceName)Move::decode_move(move, MOVE_DECODE_ATTRIBUTES::PIECE_NAME);
		if (move_piece_name == piece_to_move) {
			Positions move_piece_from = (Positions)Move::decode_move(move, MOVE_DECODE_ATTRIBUTES::SOURCE_SQUARE);
			Positions move_piece_to = (Positions)Move::decode_move(move, MOVE_DECODE_ATTRIBUTES::TARGET_SQUARE);
			PieceName promotion_piece_name = (PieceName)Move::decode_move(move, MOVE_DECODE_ATTRIBUTES::PROMOTION_PIECE_NAME);
			if (move_piece_to == move_to) {
				if ((move_from == OUT_OF_BOUNDS || move_from == move_piece_from)&&promotion_piece==NONE) {
					if (move_already_found) {
						cout << "Move ambiguity!\nTwo or more moves with the same name.\nPlease be specific for the move" << endl;
						return false;
					}
						move_already_found = true;
						move_found = move;
				}
				else if (promotion_piece != NONE && promotion_piece == promotion_piece_name) {
					if (move_already_found) {
						cout << "Move ambiguity!\nTwo or more moves with the same name.\nPlease be specific for the move" << endl;
						return false;
					}
					move_already_found = true;
					move_found = move;
				}
			}
		}
	}
	if (!move_already_found)
		cout << "No move found.\nPlease be specific for the move" << endl;
	return move_already_found;
}
