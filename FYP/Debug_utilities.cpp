#include "Debug_utilities.h"

string fen;
Debug_utilities::Debug_utilities(string fen_string) :Engine(fen_string)
{
	fen = fen_string;
}


Debug_utilities::~Debug_utilities()
{
}

void Debug_utilities::run_make_unmake_debug()
{
	while (true) {
		printAsciiBitboard(board_state, *white_player, *black_player);
		Player* current_player = this->white_turn ? white_player : black_player;
		Player* opponent_player = !this->white_turn ? white_player : black_player;

		current_player->generate_moves();
		current_player->print_moves();
		bool correct_input = false;
		do {
			string input;
			cout << "MAKE OR UNMAKE MOVE (make/unmake): ";
			cin >> input;
			if (input == "make") {
				Move moves = current_player->get_moves();
				if (moves.get_length()) {
					this->make_move(moves[0], current_player, opponent_player);
					this->white_turn = !white_turn;
					correct_input = true;
				}
				else
					cout << "No moves left" << endl;
			}
			else if (input == "unmake") {
				unmake_move();
				correct_input = true;
			}
			else {
				cout << "Please enter 'make' or 'unmake' : " << endl;
			}
		} while (!correct_input);
		system("cls");
	}
}

void Debug_utilities::engine_normal_run()
{
	run();
}

int Debug_utilities::perft_debug(int depth)
{
	if (depth == 0)
		return 1;
	Player * current_player = this->white_turn ? white_player : black_player;
	Player * opponent_player = !this->white_turn ? white_player : black_player;

	current_player->generate_moves();
	int nodes = 0;
	for (uint32_t move : current_player->get_moves()) {
		make_move(move, current_player, opponent_player);
		this->white_turn = !white_turn;
		nodes += perft_debug(depth - 1);
		unmake_move();
	}
	return nodes;
}
int check = 0;
int check_mate = 0;
void Debug_utilities::custom_perft_debug(int depth)
{
	if (depth == 0) {
		nodes++;
		return;
	}
	Player* current_player = this->white_turn ? white_player : black_player;
	Player* opponent_player = !this->white_turn ? white_player : black_player;
	current_player->generate_moves();
	if (current_player->is_player_in_check()) {
		if (current_player->get_moves().get_length() == 0) {
			check_mate++;
		}
		check++;
	}
	get_move_attr_count(current_player->get_moves());
	for (uint32_t move : current_player->get_moves()) {
		make_move(move, current_player, opponent_player);
		this->white_turn = !white_turn;
		long cummulative_nodes = nodes;
		custom_perft_debug(depth - 1);
		unmake_move();
	}
}

void Debug_utilities::perform_custom_perft(int depth)
{
	Player* current_player = this->white_turn ? white_player : black_player;
	Player* opponent_player = !this->white_turn ? white_player : black_player;
	current_player->generate_moves();
	for (uint32_t move : current_player->get_moves()) {
		make_move(move, current_player, opponent_player);
		this->white_turn = !white_turn;
		long cummulative_nodes = nodes;
		custom_perft_debug(depth - 1);
		long old_nodes = nodes - cummulative_nodes;
		unmake_move();
		//cout << "Move : " << str_positions[Move::decode_move(move, MOVE_DECODE_ATTRIBUTES::SOURCE_SQUARE)] << str_positions[Move::decode_move(move, MOVE_DECODE_ATTRIBUTES::TARGET_SQUARE)] << "\tNodes : " << old_nodes << endl;
	}
	//custom_perft_debug(depth);
	cout << depth;
	cout << ".NODES : " << nodes << endl;
	//cout << "CAPTURES : " << captures << endl;
	//cout << "ENPASSANTS : " << enpassants << endl;
	//cout << "CASTLES : " << castles << endl;
	//cout << "PROMOTIONS : " << promotions << endl;
	//cout << "Checks : " << check << endl;
	//cout << "Check Mates : " << check_mate << endl;


}

void Debug_utilities::perform_move_generation_debug(int times)
{
	for (int i = 0; i < times; i++)
		white_player->generate_moves();
}

void Debug_utilities::print_board_for_fen()
{
	printAsciiBitboard(this->board_state, *this->white_player, *this->black_player);
}

void Debug_utilities::print_bitboard_for_fen()
{
	printBitboard(this->board_state);
}

void Debug_utilities::print_bitboard_for_piece(PieceName name)
{
	printBitboard(this->white_player->get_player_pieces()[name]);
}

void Debug_utilities::get_move_attr_count(Move moves)
{
	for (uint32_t move : moves) {
		if (Move::decode_move(move, CAPTURE_FLAG))
			captures++;
		else if (Move::decode_move(move, PROMOTION_PIECE_NAME) != NONE)
			promotions++;
		else if (Move::decode_move(move, EN_PASSANT_FLAG))
			enpassants++;
		else if (Move::decode_move(move, CASTLE_FLAG))
			castles++;

	}
}

void Debug_utilities::pgn_run_on() {
	run_pgn();
}

void Debug_utilities::run_game_pgn(vector<string> game, int game_no)
{
	run_from_pgn(game, game_no);
}

void Debug_utilities::debug_move_generation(int moves_tested)
{
	Player* current_player = this->white_turn ? white_player : black_player;
	Player* opponent_player = !this->white_turn ? white_player : black_player;
	current_player->generate_moves();
	if (current_player->get_moves().get_length() == moves_tested);
	else {
		cout << fen << endl;
		//printAsciiBitboard(this->board_state, *white_player, *black_player);
		//current_player->print_moves();
		//cout << "INCORRECT NO OF MOVES GENERATED!" << endl;
	}
}
