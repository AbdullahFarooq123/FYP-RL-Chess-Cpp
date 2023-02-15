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
					this->make_move(moves[0], current_player);
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
