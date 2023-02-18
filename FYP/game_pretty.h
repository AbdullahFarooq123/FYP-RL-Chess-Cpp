#pragma once
#include<bitset>
#include<string>
#include <iomanip>
#include"Player.h"
using namespace std;
void print_bits(uint64_t bitboard, string end = "", string start = "");
string get_bits(uint64_t bitboard, string end = "", string start = "");
void printBitboard(uint64_t bitboard);
void printAsciiBitboard(uint64_t bitboard, Player white_player, Player black_player, bool invert_board = false, bool white_turn = false);
uint64_t getBitboard(int, int);
string add_str_padding(string value, int width);