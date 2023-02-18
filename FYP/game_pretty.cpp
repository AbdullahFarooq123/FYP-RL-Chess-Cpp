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
void printAsciiBitboard(uint64_t bitboard, Player white_player,Player black_player, bool invert_board , bool white_turn ) {
	for (int i = (invert_board?(white_turn?0:7):0); (invert_board ? (white_turn ? i<=7 : i>=0) : i <= 7); (invert_board ? (white_turn ? i++ : i--) : i++)) {
		cout << " " << 8 - i << "  ";
		for (int j = (invert_board ? (white_turn ? 0 : 7) : 0); (invert_board ? (white_turn ? j <= 7 : j >= 0) : j <= 7); (invert_board ? (white_turn ? j++ : j--) : j++)) {
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
	for (int i = (invert_board ? (white_turn ? 'a' : 'h') : 'a'); (invert_board ? (white_turn ? i <= 'h' : i >= 'a') : i <= 'h'); (invert_board ? (white_turn ? i++ : i--) : i++))
		cout << (char)i << "  ";
	cout << endl << endl;
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

string add_str_padding(string value, int width)
{
	int back_padding = floor((float)(width - value.length()) / 2);
	int front_padding = ceil((float)(width - value.length()) / 2);
	string new_string = "";
	for (int i = 0; i < back_padding; i++)
		new_string += ' ';
	new_string += value;
	for (int i = 0; i < front_padding; i++)
		new_string += ' ';
	return new_string;
}
