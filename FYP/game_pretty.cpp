#include"game_pretty.h"
void print_bits(uint64_t number, string end, string start ) {
	cout << start << bitset<64>(number) << end << endl;
}
string get_bits(uint64_t number, string end , string start ) {
	return start + bitset<64>(number).to_string() + end + "\n";
}
void printBitboard(uint64_t bitboard) {
	for (int i = 0; i <= 7; i++) {
		cout << " " << 8 - i << "  ";
		for (int j = 0; j <= 7; j++) {
			if ((bitboard >> (i * 8 + j)) & 1) {
				printf(" 1 ");
			}
			else {
				printf(" 0 ");
			}
		}
		printf("\n");
	}
	cout << "\n     ";
	for (int i = 'a'; i <= 'h'; i++)
		cout << (char)i << "  ";
	cout << endl;
	cout << "\n\t BITS : ";
	print_bits(bitboard, "\n", "0b");
	cout << "\t Decimal : " << bitboard << endl << endl;
}
void printAsciiBitboard(uint64_t bitboard, Player white_player,Player black_player) {
	for (int i = 0; i <= 7; i++) {
		cout << " " << 8 - i << "  ";
		for (int j = 0; j <= 7; j++) {
			uint64_t position_mask = bitmask(i * 8 + j);
			if ((bitboard >> (i * 8 + j)) & 1) {
				bool printed = false;
				for (int side = BLACK; side <= WHITE && !printed; side++) {
					Player current_player = (side == BLACK ? black_player : white_player);
					if (current_player.get_player_state() & position_mask) {
						uint64_t * player_piece_states = current_player.get_player_pieces();
						for (int piece = PAWN; piece <= KING && !printed; piece++) {
							if (player_piece_states[piece] & position_mask) {
								cout << " " << piece_unicodes[side][piece] << " ";
								printed = true;
							}
						}
					}
				}
			}
			else {
				printf(" . ");
			}
		}
		printf("\n");
	}
	cout << "\n     ";
	for (int i = 'a'; i <= 'h'; i++)
		cout << (char)i << "  ";
	cout << endl;
}
uint64_t getBitboard(int file, int rank) {
	uint64_t board = 0ull;
	for (int i = 0; i < 8; i++) {
		for (int j = 0; j < 8; j++) {
			if (j == (7 - file) || i == (7 - rank))
				board |= (1ull << i * 8 + j);
		}
	}
	return board;
}