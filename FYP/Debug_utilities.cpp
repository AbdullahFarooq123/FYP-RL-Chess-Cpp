#include "Debug_utilities.h"



Debug_utilities::Debug_utilities()
{
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
					this->make_move(moves[0], current_player,opponent_player);
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
	Player* current_player = this->white_turn ? white_player : black_player;
	Player* opponent_player = !this->white_turn ? white_player : black_player;

	current_player->generate_moves();
	int nodes = 0;
	for (uint32_t move : current_player->get_moves()) {
		make_move(move, current_player,opponent_player);
		this->white_turn = !white_turn;
		nodes += perft_debug(depth - 1);
		unmake_move();
	}
	return nodes;
}

void Debug_utilities::custom_perft_debug(int depth)
{
	if (depth == 0) {
		nodes++;
		return;
	}
	Player * current_player = this->white_turn ? white_player : black_player;
	Player* opponent_player = !this->white_turn ? white_player : black_player;

	current_player->generate_moves();
	get_move_attr_count(current_player->get_moves());
	for (uint32_t move : current_player->get_moves()) {
		make_move(move, current_player,opponent_player);
		this->white_turn = !white_turn;
		custom_perft_debug(depth - 1);
		unmake_move();
	}
}

void Debug_utilities::perform_custom_perft(int depth)
{
	custom_perft_debug(depth);
	cout << "DEPTH : " << depth << endl;
	cout << "NODES : " << nodes << endl;
	cout << "CAPTURES : " << captures << endl;
	cout << "ENPASSANTS : " << enpassants << endl;
	cout << "CASTLES : " << castles << endl;
	cout << "PROMOTIONS : " << promotions << endl;
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
