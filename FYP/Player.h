#pragma once
#include "attribute_utilities.h"
#include "pre_calculation_utilities.h"
#include "Move.h"
#include<iostream>
class Player
{
public:
	Player(Side player_side, uint64_t *board_state, uint64_t player_state, uint64_t player_pieces_state[6], int enpassant_square, uint32_t* previous_move, unsigned int castle_rights);
	~Player();
	void generate_moves();
	void make_move(uint32_t move);
	uint64_t get_bitboard();
	void print_moves();
	void set_opponent_player(Player* opponent_player);
	uint64_t get_player_state();
	uint64_t* get_player_pieces();
private:
	Side player_side;
	unsigned int castling_rights;
	uint64_t* board_state;
	uint32_t* previous_move;
	uint64_t player_state;
	uint64_t player_pieces_state[6];
	int enpassant_square;
	Move moves;
	Player* opponent_player;
	void generate_pawn_moves(uint64_t piece_bitboard, uint64_t opponent_state, uint64_t opponent_attacks, unsigned int check_flags, uint64_t attackers_ray);
	void generate_knight_moves(uint64_t piece_bitboard, uint64_t opponent_state, uint64_t opponent_attacks, unsigned int check_flags, uint64_t attackers_ray);
	void generate_bishop_moves(uint64_t piece_bitboard, uint64_t opponent_state, uint64_t opponent_attacks, unsigned int check_flags, uint64_t attackers_ray);
	void generate_rook_moves(uint64_t piece_bitboard, uint64_t opponent_state, uint64_t opponent_attacks, unsigned int check_flags, uint64_t attackers_ray);
	void generate_queen_moves(uint64_t piece_bitboard, uint64_t opponent_state, uint64_t opponent_attacks, unsigned int check_flags, uint64_t attackers_ray);
	void generate_king_moves(uint64_t piece_bitboard, uint64_t opponent_state, uint64_t opponent_attacks, unsigned int check_flags, uint64_t attackers_ray);
	bool can_castle(CASTLE_DECODE_ATTRIBUTES attribute_name, uint64_t castle_occupancy, uint64_t opponent_attacks, uint64_t king_position_mask);
	bool is_incheck(uint64_t opponent_attacks);
	uint64_t get_opponent_attacks(unsigned int &check_flags);
	uint64_t get_attackers_ray(unsigned int check_flags);
	uint64_t get_piece_attacks(PieceName piece_name, Positions piece_position);
};

