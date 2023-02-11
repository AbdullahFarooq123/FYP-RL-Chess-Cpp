#pragma once

#include<iostream>
#include"attribute_utilities.h"

#define bitmask(position) ((uint64_t)(1ull<<position))

const uint64_t left_edge = 0b0000000100000001000000010000000100000001000000010000000100000001;
const uint64_t right_edge = 0b1000000010000000100000001000000010000000100000001000000010000000;
const uint64_t before_left_edge = 0b0000001000000010000000100000001000000010000000100000001000000010;
const uint64_t before_right_edge = 0b0100000001000000010000000100000001000000010000000100000001000000;
const uint64_t bottom_edge = 0b1111111100000000000000000000000000000000000000000000000000000000;
const uint64_t before_bottom_edge = 0b0000000011111111000000000000000000000000000000000000000000000000;
const uint64_t top_edge = 0b0000000000000000000000000000000000000000000000000000000011111111;
const uint64_t before_top_edge = 0b0000000000000000000000000000000000000000000000001111111100000000;
const uint64_t white_king_side_castle_occupancy = 0b0110000000000000000000000000000000000000000000000000000000000000;
const uint64_t white_queen_side_castle_occupancy = 0b0000111000000000000000000000000000000000000000000000000000000000;
const uint64_t black_king_side_castle_occupancy = 0b0000000000000000000000000000000000000000000000000000000001100000;
const uint64_t black_queen_side_castle_occupancy = 0b0000000000000000000000000000000000000000000000000000000000001110;

void init_precalculation_utilities();

//*****************General Utilities Templates*******************************
int count_set_bits(uint64_t);
int get_least_bit_index(uint64_t);
uint64_t setOccupancy(int, int, uint64_t);
void init_slider_attacks(bool);

//*****************Random Number Generation Templates************************
extern unsigned int random_state;
uint32_t get_32b_rand_no();
uint64_t get_64b_rand_no();

//*****************Magic Number Generation Templates*************************
void init_magic_numbers();
uint64_t generate_magic_number();
uint64_t find_magic_number(uint64_t, int, bool);

//*****************Pawn Utilities Templates**********************************
//[side][position]
extern uint64_t pawn_attack_maps[2][64];
void init_pawn_attacks();
uint64_t get_pawn_attack(int, int);

//*****************King Utilities Templates**********************************
//[position]
extern uint64_t king_attack_maps[64];
void init_king_attacks();
uint64_t get_king_attack(int);

//*****************Knight Utilities Templates********************************
//[position]
extern uint64_t knight_attack_maps[64];
void init_knight_attacks();
uint64_t get_knight_attack(int);

//*****************Rook Utilities Templates**********************************
//[position]
extern int rook_attack_count[64];
//[position]
extern uint64_t rook_magic_number[64];
//[position]
extern uint64_t rook_attacks[64];
//[position][occupancy]
extern uint64_t rook_attacks_table[64][4096];
void init_rook_attack_mask();
void init_rook_attack_count();
uint64_t get_rook_attak_mask_exc_ends(uint64_t);
uint64_t get_rook_attak_mask_inc_end_blockers(uint64_t, uint64_t);
uint64_t get_rook_attacks(int, uint64_t);

//*****************Bishop Utilities Templates********************************
//[position]
extern int bishop_attack_count[64];
//[position]
extern uint64_t bishop_magic_number[64];
//[position]
extern uint64_t bishop_attacks[64];
//[position][occupancy]
extern uint64_t bishop_attacks_table[64][512];
void init_bishop_attack_mask();
void init_bishop_attack_count();
uint64_t get_bishop_attak_mask_exc_ends(uint64_t);
uint64_t get_bishop_attak_mask_inc_end_blockers(uint64_t, uint64_t);
uint64_t get_bishop_attacks(int, uint64_t);

