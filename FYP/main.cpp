#include "Debug_utilities.h"
using namespace std;
int main() {
	Debug_utilities debug = Debug_utilities(Fen_utility::start_pos);
	//debug.engine_normal_run();
	///*debug.print_board_for_fen();
	//debug.print_bitboard_for_fen();
	//debug.print_bitboard_for_piece(BISHOP);*/
	debug.perform_custom_perft(4);
}