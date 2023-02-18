#include "Debug_utilities.h"
#include<fstream>
#include<string>
using namespace std;

void debug_pgn_data();
void debug_normal_run();
void debug_perft_tests();
void debug_moves_generation();
int main() {
	enum debug_attr {
		NORMAL_RUN,
		PGN_DATA_DEBUG,
		PERFT_DEBUG,
		DEBUG_MOVES_GEN,
		NONE
	};
	debug_attr run_attribute = PGN_DATA_DEBUG;
	switch (run_attribute)
	{
		case NORMAL_RUN:
			debug_normal_run();
			break;
		case PGN_DATA_DEBUG:
			debug_pgn_data();
			break;
		case PERFT_DEBUG:
			debug_perft_tests();
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
	// Read from the text file
	ifstream MyReadFile("D:\\PROJECTS\\pythonProject\\pgn_output");
	string game;
	// Use a while loop together with the getline() function to read the file line by line
	while (getline(MyReadFile, game)) {
		// Output the text from the file
		vector<string> moves = split(game, ',');
		pgn_data.push_back(moves);
		//break;
	}
	// Close the file
	MyReadFile.close();
	int game_index = 1;
	for (vector<string> game : pgn_data) {
		Debug_utilities debug(Fen_utility::start_pos);
		debug.run_game_pgn(game,game_index++);
	}
}
void debug_normal_run() {
	//4k3/8/8/8/8/3KP2r/8/8 w - - 0 1
	//4k3/8/8/8/6B1/8/4P3/3K4 w - - 0 1
	//3k4/4b3/8/1pP5/1K6/8/8/8 w - b6 0 3
	//4k3/1KP3r1/8/8/8/8/8/8 w - - 0 1
	//4k3/8/8/8/8/8/8/4KB1r w - -0 1
	//1r5r/pp1k1ppp/4n1b1/6P1/1b6/1NN1pPB1/PP2P2P/R3K2R w KQkq - 0 1
	//2r3k1/5pp1/4b2p/1p2Q3/2P1P3/1P5P/1RB3PK/q7 w - - 0 1
	//2k4r/pp6/4Pnpp/2q1pp2/3r4/1B3P2/PP4PP/1QR4K w - - 0 1
	//r7/pb1nkp2/1p1pp3/2pP4/Q1P1P3/2PBRNPq/P4PKp/8 w - - 0 1
	Debug_utilities debug = Debug_utilities("r7/pb1nkp2/1p1pp3/2pP4/Q1P1P3/2PBRNPq/P4PKp/8 w - - 0 1");
	debug.engine_normal_run();
}
void debug_perft_tests() {
	for (int i = 1; i <= 5; i++) {
		Debug_utilities debug = Debug_utilities(Fen_utility::start_pos);
		debug.perform_custom_perft(i);
	}
}
void debug_moves_generation() {
	unordered_map<string, int> move_generation_test_cases = {
		{"4k3/8/8/8/8/4P3/8/4K3 w - - 0 1",6}, // pawn moves
		{"4k3/8/8/8/2p1P3/2P1P3/8/4K3 w - - 0 1", 6}, // pawn blocked by piece
		{"4k3/8/8/8/8/3KP2r/8/8 w - - 0 1",7}, // pawn pinned
		{"4k3/8/8/8/8/8/4P3/3K4 w - - 0 1", 6}, // double push
		{"4k3/8/8/8/2p5/4P3/2P1P3/3K4 w - - 0 1",5}, // pawn blocked by a piece
		{"4k3/8/8/8/6b1/8/4P3/3K4 w - - 0 1",4}, // pawn pinned
		{"4k3/8/8/3P1p2/4P3/8/8/3K4 w - - 0 1",8}, // pawn capture moves
		{"4k3/8/8/8/8/5p2/3KP2r/8 w - - 0 1",7},  // pawn pinned
		{"3rk3/2P5/8/8/8/8/8/3K4 w - - 0 1",8},  // pawn promotion
		{"4k3/8/8/5pP1/8/8/3K4/8 w - f6 0 2",10}, // en passant
		{"3k4/4b3/8/1pP5/1K6/8/8/8 w - b6 0 3",5}, //  en passant when pinned
		{"4k3/1KP3r1/8/8/8/8/8/8 w - - 0 1",7}, // pawn promotion when pinned
		{"4k3/8/8/8/8/3B4/8/4K3 w - - 0 1",16},  //Bishop moves
		{"4k3/8/8/8/8/8/2K5/1B6 w - - 0 1",8}, //Bishop blocked by any piece
		{"4k3/8/8/8/8/3K4/p7/1B6 w - - 0 1",10}, // bishop_capture_moves
		{"4k3/8/8/8/8/8/8/4KB1r w - - 0 1",4}, // bishop_pinned
		{"4k3/8/8/8/8/8/2B5/4K2r w - - 0 1",3}, // check
		{"4k3/8/8/8/4N3/8/8/4K3 w - - 0 1",13}, // knight_moves
		{"4k3/8/8/8/8/3N4/8/4K3 w - - 0 1",12}, // knight blocked by any piece
		{"4k3/8/8/4p3/1n3r2/3N4/8/4K3 w - - 0 1",10}, // knight_capture
		{"4k3/8/8/8/5b2/4N3/3K4/8 w - - 0 1",7},  // knight_pinned
		{"4k3/8/8/8/3N1b2/8/3K4/8 w - - 0 1",6}, // check
		{"4k3/8/8/8/8/3Q4/8/3K4 w - - 0 1",29}, // Queen_moves
		{"4k3/8/8/8/8/3Q4/8/3K4 w - - 0 1",29}, // Queen blocked by any piece
		{"4k3/8/8/5n2/3p4/1b1Q4/8/3K4 w - - 0 1",6}, //Queen_capture
		{"4k3/8/8/8/8/8/8/2QK2r1 w - - 0 1",3},  // check
		{"1k6/8/q7/8/8/8/4Q3/5K2 w - - 0 1",8},  //Queen_pinned
		{"1k6/8/8/8/8/7b/6Q1/5K2 w - - 0 1",5}, // Queen_pinned
		{"4k3/8/8/8/8/8/8/R3K3 w - - 0 1",15},  // rook_moves
		{"4k3/8/8/8/8/8/8/3RK3 w - - 0 1",14},  // rook blocked by any piece
		{"4k3/8/8/8/8/1p6/8/1Rn1K3 w - - 0 1",8}, //rook_capture
		{"4k3/8/8/8/7b/8/5R2/4K3 w - - 0 1",4},  // rook_pinned
		{"4k3/8/8/8/7b/8/2R5/4K3 w - - 0 1",5},  // check
		{"4k3/8/8/8/8/8/8/R3K2R w KQ - 0 1",26}, // castling
		{"8/8/4k3/8/8/8/8/Rn2K2R w KQ - 1 1",22}, // castling
		{"4k3/8/8/8/8/3K4/8/8 w - - 0 1",8}, // king_move
		{"4k3/8/8/8/8/8/4P3/4K3 w - - 0 1",6},  // king blocked by a piece
		{"4k3/8/8/8/8/8/4p3/4K3 w - - 0 1",3},  // king capture
		{"8/8/4k3/8/8/8/8/R3K2R w - - 5 3",24},   // castling when king is moved
		{"r3k3/8/8/8/8/8/8/4K2R w Kq - 0 1",15}, // castling
	};
	for (auto value : move_generation_test_cases) {
		Debug_utilities debug = Debug_utilities(value.first);
		debug.debug_move_generation(value.second);
	}
}