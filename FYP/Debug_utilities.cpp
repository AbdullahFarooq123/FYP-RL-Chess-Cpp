#include "Debug_utilities.h"

Debug_utilities::Debug_utilities(string fen_string) :Engine(fen_string){
	this->fen_string = fen_string;
}

Debug_utilities::~Debug_utilities(){}

void Debug_utilities::run_make_unmake_debug()
{
	while (true) {
		pair<Player*, Player*> players = Engine::set_sides(true);
		Player* current_player = players.first;
		Player* opponent_player = players.second;
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
			else
				cout << "Please enter 'make' or 'unmake' : " << endl;
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
	pair<Player*, Player*> players = Engine::set_sides(false);
	Player* current_player = players.first;
	Player* opponent_player = players.second;
	int nodes = 0;
	for (uint32_t move : current_player->get_moves()) {
		make_move(move, current_player, opponent_player);
		this->white_turn = !white_turn;
		nodes += perft_debug(depth - 1);
		unmake_move();
	}
	return nodes;
}

void Debug_utilities::run_pgn_game(vector<string> game, int game_no)
{
	Engine::run_from_pgn(game, game_no);
}

void Debug_utilities::debug_move_generation(int moves_tested)
{
	pair<Player*, Player*> players = Engine::set_sides(false);
	Player* current_player = players.first;
	Player* opponent_player = players.second;
	if (current_player->get_moves().get_length() == moves_tested);
	else cout << fen_string << " : INCORRECT!" << endl;
}
