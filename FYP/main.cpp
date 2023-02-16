#include "Debug_utilities.h"
using namespace std;
int main() {
	Debug_utilities debug = Debug_utilities(Fen_utility::start_pos);
	debug.engine_normal_run();
}