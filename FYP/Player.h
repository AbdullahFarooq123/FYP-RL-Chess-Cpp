#pragma once
#include "pre_calculation_utilities.h"
#include "Move.h"
#include"Player_state.h"
using namespace std;
class Player
{
public:
	Player(Player_Side player_side, uint64_t *board_state, uint64_t player_state, uint64_t player_pieces_state[6], int enpassant_square, uint32_t* previous_move, unsigned int castle_rights);
	~Player();
	void generate_moves();
	void make_move(uint32_t move);
	uint64_t get_bitboard();
	void print_moves();
	void set_opponent_player(Player* opponent_player);
	uint64_t get_player_state();
	uint64_t* get_ptr_player_state();
	uint64_t* get_player_pieces();
	Move get_moves();
	Player* get_opponent_player();
	uint32_t get_castling_rights();
	int* get_enpassant_square();
	void set_state(Player_state state);
	uint64_t* get_deep_copy_pieces();
	void remove_piece_from_player_state(uint64_t mask);
	bool is_player_in_check();
private:
	Player_Side player_side;
	unsigned int castling_rights;
	uint64_t* board_state;
	uint32_t* previous_move;
	uint64_t player_state;
	uint64_t player_pieces_state[6];
	int enpassant_square;
	Move moves;
	Player* opponent_player;
	bool in_check;
	void generate_pawn_moves(uint64_t piece_bitboard, uint64_t opponent_state, uint64_t opponent_attacks, unsigned int check_flags, uint64_t attackers_ray, uint64_t kings_ray, uint64_t opponent_sliding_pieces, uint64_t* opponent_pieces);
	void generate_knight_moves(uint64_t piece_bitboard, uint64_t opponent_state, uint64_t opponent_attacks, unsigned int check_flags, uint64_t attackers_ray, uint64_t kings_ray, uint64_t opponent_sliding_pieces, uint64_t* opponent_pieces);
	void generate_bishop_moves(uint64_t piece_bitboard, uint64_t opponent_state, uint64_t opponent_attacks, unsigned int check_flags, uint64_t attackers_ray, uint64_t kings_ray, uint64_t opponent_sliding_pieces, uint64_t* opponent_pieces);
	void generate_rook_moves(uint64_t piece_bitboard, uint64_t opponent_state, uint64_t opponent_attacks, unsigned int check_flags, uint64_t attackers_ray, uint64_t kings_ray, uint64_t opponent_sliding_pieces, uint64_t* opponent_pieces);
	void generate_queen_moves(uint64_t piece_bitboard, uint64_t opponent_state, uint64_t opponent_attacks, unsigned int check_flags, uint64_t attackers_ray, uint64_t kings_ray, uint64_t opponent_sliding_pieces, uint64_t* opponent_pieces);
	void generate_king_moves(uint64_t piece_bitboard, uint64_t opponent_state, uint64_t opponent_attacks, unsigned int check_flags, uint64_t attackers_ray, uint64_t kings_ray, uint64_t opponent_sliding_pieces, uint64_t* opponent_pieces);
	bool can_castle(CASTLE_DECODE_ATTRIBUTES attribute_name, uint64_t castle_occupancy, uint64_t opponent_attacks, uint64_t king_position_mask);
	bool is_incheck(uint64_t opponent_attacks);
	uint64_t get_opponent_attacks(unsigned int &check_flags, uint64_t* opponent_pieces);
	uint64_t get_attackers_ray(unsigned int check_flags);
	uint64_t get_piece_attacks(PieceName piece_name, Positions piece_position);
	uint64_t generate_ray_opposite_to_kings_square(Positions piece_position, uint64_t opponent_pieces, uint64_t piece_bitmask, Directions &direction);
	uint64_t get_ray_in_the_direction_of_king_square(Positions piece_position,PieceName piece_name);
	bool is_discovered_check(uint64_t* opponent_pieces, Directions direction, uint64_t ray_opposite_to_king_square);
};

