#pragma once
#include<iostream>
#include<bitset>
#include"Player.h"
#include"attribute_utilities.h"
using namespace std;
void print_bits(uint64_t bitboard, string end = "", string start = "");
string get_bits(uint64_t bitboard, string end = "", string start = "");
void printBitboard(uint64_t bitboard);
void printAsciiBitboard(uint64_t bitboard, Player white_player, Player black_player);
uint64_t getBitboard(int, int);