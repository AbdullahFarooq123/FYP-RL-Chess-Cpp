#include "Debug_utilities.h"
#include<fstream>
#include<string>
#include <chrono>
#include <ctime>
using namespace std;

void debug_pgn_data();
void debug_normal_run();
void debug_perft_tests(int depth_from = 1, int depth_to_go = 4);
void debug_moves_generation();
void main_run();
int main() {
	//main_run();
	/*printBitboard(bottom_edge >> 32);
	uint64_t val = -1;
	cout << (int)~right_edge;*/
	//init_magic_numbers();
	init_precalculation_utilities();
	printBitboard(get_bishop_attacks(a1, bitmask(b2)));
}
void main_run() {
	enum debug_attr {
		NORMAL_RUN,
		PGN_DATA_DEBUG,
		PERFT_DEBUG,
		DEBUG_MOVES_GEN,
		NONE
	};

	debug_attr run_attribute = PERFT_DEBUG;
	switch (run_attribute)
	{
	case NORMAL_RUN:
		debug_normal_run();
		break;
	case PGN_DATA_DEBUG:
		debug_pgn_data();
		break;
	case PERFT_DEBUG:
		debug_perft_tests(4,4);
		break;
	case DEBUG_MOVES_GEN:
		debug_moves_generation();
		break;
	default:
		break;
	}
}
void debug_pgn_data() {
	vector<vector<string>> pgn_data;
	ifstream MyReadFile("D:\\PROJECTS\\pythonProject\\pgn_output");
	string game;
	while (getline(MyReadFile, game)) {
		vector<string> moves = split(game, ',');
		pgn_data.push_back(moves);
	}
	MyReadFile.close();
	int game_index = 1;
	for (vector<string> game : pgn_data) {
		Debug_utilities debug(Fen_utility::start_pos);
		debug.run_pgn_game(game,game_index++);
	}
}
void debug_normal_run() {
	Debug_utilities debug = Debug_utilities("3r3k/8/2q5/3P4/8/3K4/8/1Q6 w - - 0 1");
	debug.engine_normal_run();
}
void debug_perft_tests(int depth_from, int depth_to_go) {
	std::chrono::time_point<std::chrono::system_clock> start, end;
	cout << "DEPTH\tNODES\n";
	for (int depth = depth_from; depth <= depth_to_go; depth++) {
		start = std::chrono::system_clock::now();
		Debug_utilities debug = Debug_utilities(Fen_utility::start_pos);
		cout << depth << "    \t" << debug.perft_debug(depth) << endl;
		end = std::chrono::system_clock::now();
		std::chrono::duration<double> elapsed_seconds = end - start;
		std::cout << "TIME : " << elapsed_seconds.count() << "s\n";
	}
}
void debug_moves_generation() {
	for (auto value : Fen_utility::move_generation_test_cases) {
		Debug_utilities debug = Debug_utilities(value.first);
		debug.debug_move_generation(value.second);
	}
}