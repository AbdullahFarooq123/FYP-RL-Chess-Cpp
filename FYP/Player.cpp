#include "Player.h"
#include "game_pretty.h"
#include "pgn_utilities.h"
inline uint64_t get_piece_ray_in_one_direction(Positional_Directions direction, Positions piece_position, uint64_t board_state) {
	switch (direction)
	{
	case NORTH:
	case EAST:
	case SOUTH:
	case WEST:
		return get_rook_attacks(piece_position, board_state) & directional_rays[direction][piece_position];
	case NORTH_EAST:
	case SOUTH_EAST:
	case SOUTH_WEST:
	case NORTH_WEST:
		return get_bishop_attacks(piece_position, board_state) & directional_rays[direction][piece_position];
	}
	return ~0ull;
}
inline Positional_Directions opposite_direction(Positional_Directions direction) {
	switch (direction)
	{
	case NORTH:
		return SOUTH;
	case EAST:
		return WEST;
	case SOUTH:
		return NORTH;
	case WEST:
		return EAST;
	case NORTH_EAST:
		return SOUTH_WEST;
	case SOUTH_EAST:
		return NORTH_WEST;
	case SOUTH_WEST:
		return NORTH_EAST;
	case NORTH_WEST:
		return SOUTH_EAST;
	default:
		return NOT_ALLIGNED;
	}
}
Player::Player(Player_Side player_side, uint64_t* board_state, uint64_t player_state, uint64_t player_pieces_state[6], int enpassant_square, uint32_t* previous_move, unsigned int castle_rights)
{
	this->player_side = player_side;
	this->board_state = board_state;
	this->previous_move = previous_move;
	this->player_state = player_state;
	this->enpassant_square = enpassant_square;
	this->castling_rights = castle_rights;
	this->moves = Move();
	for (int piece = PieceName::PAWN; piece <= PieceName::KING; piece++)
		this->player_pieces_state[piece] = player_pieces_state[piece];
	in_check = false;
}

Player::~Player()
{
}

void Player::print_moves()
{
	cout << "*************************************************************************************************************************************\n";
	cout << "*                                                                                                                                   *\n";
	cout << "*" << add_str_padding("MOVES TABLE", string("***********************************************************************************************************************************").length()) << "*" << endl;
	cout << "*                                                                                                                                   *\n";
	cout << "*************************************************************************************************************************************\n";
	cout << "*  PIECE NAME  *  SOURCE SQ.  *  TARGET SQ.  *  DOUBLE PUSH  *  ENPASSANT  *  CAPTURE  *  CASTLE  *  PROMOTION PIECE  *  MOVE NAME  *\n";
	cout << "*************************************************************************************************************************************\n";
	for (uint32_t move : moves) {
		string source_square = add_str_padding(str_positions[Move::decode_move(move, SOURCE_SQUARE)], string("  SOURCE SQ.  ").length());
		string target_square = add_str_padding(str_positions[Move::decode_move(move, TARGET_SQUARE)], string("  TARGET SQ.  ").length());
		string piece_name = add_str_padding(str_piece_names[Move::decode_move(move, PIECE_NAME)], string("  PIECE NAME  ").length());
		string promotion_piece_name = add_str_padding(str_piece_names[Move::decode_move(move, PROMOTION_PIECE_NAME)], string("  PROMOTION PIECE  ").length());
		string capture_flag = add_str_padding(((bool)Move::decode_move(move, CAPTURE_FLAG) ? "CAPTURE" : "-"), string("  CAPTURE  ").length());
		string double_push_flag = add_str_padding(((bool)Move::decode_move(move, DOUBLE_PUSH_FLAG) ? "DOUBLE PUSH" : "-"), string("  DOUBLE PUSH  ").length());
		string en_passant_flag = add_str_padding(((bool)Move::decode_move(move, EN_PASSANT_FLAG) ? "EN-PASSANT" : "-"), string("  ENPASSANT  ").length());
		string castle_flag = add_str_padding(((bool)Move::decode_move(move, CASTLE_FLAG) ? "CASTLE" : "-"), string("  CASTLE  ").length());
		string possible_move_name = add_str_padding(pgn_utilities::encode_pgn(move), string("  MOVE NAME  ").length());
		cout << "*" << piece_name << "*" << source_square << "*" << target_square << "*" << double_push_flag << "*" << en_passant_flag << "*" << capture_flag << "*" << castle_flag << "*" << promotion_piece_name << "*" << possible_move_name << "*" << endl;
	}
	cout << "*************************************************************************************************************************************\n";
}

void Player::set_opponent_player(Player * opponent_player)
{
	this->opponent_player = opponent_player;
}

void Player::generate_moves()
{
	moves.reset_moves();
	uint64_t opponent_state = (*this->board_state) & (~this->player_state);
	uint64_t* opponent_pieces = this->opponent_player->get_player_pieces();
	uint64_t opponent_sliding_pieces = opponent_pieces[BISHOP] | opponent_pieces[ROOK] | opponent_pieces[QUEEN];
	unsigned int check_flags = 0;
	uint64_t opponent_attacks = this->get_opponent_attacks(check_flags, opponent_pieces);
	bool knight_check_flag = Move::decode_check_flag(check_flags, CHECK_DECODE_ATTRIBUTES::KNIGHT_CHECK);
	uint64_t attacker_mask = knight_check_flag ? square_bitmask[Move::decode_check_flag(check_flags, CHECK_DECODE_ATTRIBUTES::ATTACKER_POSITION)] : ~0ull;
	Positions player_kings_position = (Positions)get_least_bit_index(this->player_pieces_state[KING]);
	uint64_t attackers_ray = (Move::decode_check_flag(check_flags, CHECK_DECODE_ATTRIBUTES::CHECK) ? get_attackers_ray(check_flags) : ~0ull);
	uint64_t kings_ray = get_queen_attacks(player_kings_position,*this->board_state);
	int piece_name = (Move::decode_check_flag(check_flags, CHECK_DECODE_ATTRIBUTES::DOUBLE_CHECK) ? PieceName::KING : PieceName::PAWN);
	for (; piece_name <= PieceName::KING; piece_name++) {
		uint64_t piece_bitboard = this->player_pieces_state[piece_name];
		switch (piece_name)
		{
		case PieceName::PAWN:
			generate_pawn_moves(piece_bitboard, opponent_state, opponent_attacks, check_flags, attackers_ray, kings_ray, opponent_sliding_pieces, opponent_pieces, player_kings_position, knight_check_flag,attacker_mask);
			break;
		case PieceName::KNIGHT:
			generate_knight_moves(piece_bitboard, opponent_state, opponent_attacks, check_flags, attackers_ray, kings_ray, opponent_sliding_pieces, opponent_pieces, player_kings_position, knight_check_flag, attacker_mask);
			break;
		case PieceName::BISHOP:
			generate_bishop_moves(piece_bitboard, opponent_state, opponent_attacks, check_flags, attackers_ray, kings_ray, opponent_sliding_pieces, opponent_pieces, player_kings_position, knight_check_flag, attacker_mask);
			break;
		case PieceName::ROOK:
			generate_rook_moves(piece_bitboard, opponent_state, opponent_attacks, check_flags, attackers_ray, kings_ray, opponent_sliding_pieces, opponent_pieces, player_kings_position, knight_check_flag, attacker_mask);
			break;
		case PieceName::QUEEN:
			generate_queen_moves(piece_bitboard, opponent_state, opponent_attacks, check_flags, attackers_ray, kings_ray, opponent_sliding_pieces, opponent_pieces, player_kings_position, knight_check_flag, attacker_mask);
			break;
		case PieceName::KING:
			generate_king_moves(piece_bitboard, opponent_state, opponent_attacks, check_flags, attackers_ray, kings_ray, opponent_sliding_pieces, opponent_pieces, player_kings_position, knight_check_flag, attacker_mask);
			break;
		}
	}
}

void Player::make_move(uint32_t move)
{
	Positions source_square = (Positions)Move::decode_move(move, SOURCE_SQUARE);
	Positions target_square = (Positions)Move::decode_move(move, TARGET_SQUARE);
	PieceName piece_name = (PieceName)Move::decode_move(move, PIECE_NAME);
	PieceName promotion_piece_name = (PieceName)Move::decode_move(move, PROMOTION_PIECE_NAME);
	bool is_promotion = (piece_name == PAWN) && (square_bitmask[source_square] & (this->player_side == WHITE ? top_edge << 8 : bottom_edge >> 8));
	bool castle_flag = (bool)Move::decode_move(move, CASTLE_FLAG);
	if (piece_name == KING || piece_name == ROOK) {
		switch (player_side)
		{
		case WHITE:
			if (piece_name == KING)castling_rights &= 0b1100;
			else if (source_square == g1)castling_rights &= 0b1110;
			else if (source_square == a1)castling_rights &= 0b1101;
			break;
		case BLACK:
			if (piece_name == KING)castling_rights &= 0b0011;
			else if (source_square == g8)castling_rights &= 0b1011;
			else if (source_square == a8)castling_rights &= 0b0111;
			break;
		}
	}
	player_state &= ~square_bitmask[source_square];
	player_pieces_state[piece_name] &= ~square_bitmask[source_square];
	if (is_promotion)
		player_pieces_state[promotion_piece_name] |= square_bitmask[target_square];
	else if (castle_flag) {
		Positions rook_from_square = OUT_OF_BOUNDS;
		Positions rook_to_square = OUT_OF_BOUNDS;
		if (this->player_side == WHITE && target_square == g1) {
			rook_from_square = h1;
			rook_to_square = f1;
		}
		else if (this->player_side == WHITE && target_square == c1) {
			rook_from_square = a1;
			rook_to_square = d1;
		}
		else if (this->player_side == BLACK && target_square == g8) {
			rook_from_square = h8;
			rook_to_square = f8;
		}
		else if (this->player_side == BLACK && target_square == c8) {
			rook_from_square = a8;
			rook_to_square = d8;
		}
		player_pieces_state[ROOK] &= (uint64_t)~square_bitmask[rook_from_square];
		player_state &= ~square_bitmask[rook_from_square];
		player_pieces_state[ROOK] |= square_bitmask[rook_to_square];
		player_state |= square_bitmask[rook_to_square];
		player_pieces_state[KING] |= square_bitmask[target_square];
		player_state |= square_bitmask[target_square];
	}
	else
		player_pieces_state[piece_name] |= square_bitmask[target_square];
	player_state |= square_bitmask[target_square];
	this->in_check = false;
}

void Player::generate_pawn_moves(uint64_t piece_bitboard, uint64_t opponent_state, uint64_t opponent_attacks, unsigned int check_flags, uint64_t attackers_ray, uint64_t kings_ray, uint64_t opponent_sliding_pieces, uint64_t * opponent_pieces, Positions king_position, bool knight_check_flag, uint64_t attacker_mask)
{
	bool prev_move_double_push_flag = Move::decode_move(*previous_move, DOUBLE_PUSH_FLAG);
	while (piece_bitboard) {
		Positions piece_position = (Positions)get_least_bit_index(piece_bitboard);
		uint64_t piece_mask = square_bitmask[piece_position];
		bool is_promotion = piece_mask & (player_side == WHITE ? before_top_edge : before_bottom_edge);
		bool possible_pin = kings_ray & piece_mask;
		uint64_t valid_ray = is_discovered_check(piece_position, opponent_sliding_pieces, opponent_pieces, king_position, kings_ray, possible_pin);
		//Pawn Attacks 
		uint64_t pawn_attack_map = pawn_attack_maps[this->player_side][piece_position];
		pawn_attack_map &= opponent_state;
		pawn_attack_map &= attackers_ray;
		pawn_attack_map &= valid_ray;
		pawn_attack_map &= attacker_mask;
		while (pawn_attack_map) {
			Positions attack_position = (Positions)get_least_bit_index(pawn_attack_map);
			switch (is_promotion) {
			case true:
				for (int promotion_piece = KNIGHT; promotion_piece <= QUEEN; promotion_piece++)
					moves.add_move(Move::encode_move(piece_position, attack_position, PAWN, promotion_piece, 1, 0, 0, 0));
				break;
			case false:
				moves.add_move(Move::encode_move(piece_position, attack_position, PAWN, NONE, 1, 0, 0, 0));
				break;
			}
			pawn_attack_map &= pawn_attack_map - 1;
		}
		//Quite and double push moves
		if (!knight_check_flag) {
			uint64_t quite_move = 0ull;
			uint64_t double_push_move = 0ull;
			switch (player_side) {
			case WHITE:
				quite_move = (piece_mask >> 8);
				if (piece_mask & before_bottom_edge)
					double_push_move = (quite_move >> 8);
				break;
			case BLACK:
				quite_move = (piece_mask << 8);
				if (piece_mask & before_top_edge)
					double_push_move = (quite_move << 8);
				break;
			}
			quite_move &= ~*this->board_state;
			double_push_move &= ~*this->board_state;
			if (quite_move) {
				quite_move &= attackers_ray;
				quite_move &= valid_ray;
				while (quite_move) {
					int move_position = get_least_bit_index(quite_move);
					switch (is_promotion) {
					case true:
						for (int promotion_piece = KNIGHT; promotion_piece <= QUEEN; promotion_piece++)
							moves.add_move(Move::encode_move(piece_position, move_position, PAWN, promotion_piece, 0, 0, 0, 0));
						break;
					case false:
						moves.add_move(Move::encode_move(piece_position, move_position, PAWN, NONE, 0, 0, 0, 0));
						break;
					}
					quite_move &= quite_move - 1;
				}
				if (double_push_move) {
					double_push_move &= attackers_ray;
					double_push_move &= valid_ray;
					while (double_push_move) {
						int move_position = get_least_bit_index(double_push_move);
						moves.add_move(Move::encode_move(piece_position, move_position, PAWN, NONE, 0, 1, 0, 0));
						double_push_move &= double_push_move - 1;
					}
				}

			}

		}
		//enpassant_capture
		if ((piece_position & (player_side == WHITE ? bottom_edge >> 32 : top_edge << 32))&&!~valid_ray) {
			if (this->enpassant_square != OUT_OF_BOUNDS) {
				uint64_t enpasssant_square_mask = square_bitmask[this->enpassant_square];
				uint64_t possible_opponent_piece_for_enpassant = this->player_side == WHITE ? enpasssant_square_mask << 8 : enpasssant_square_mask >> 8;
				bool opponent_piece_exist_for_enpassant = possible_opponent_piece_for_enpassant & *this->board_state;
				if (opponent_piece_exist_for_enpassant) {
					pawn_attack_map &= enpasssant_square_mask;
					if (pawn_attack_map)
						moves.add_move(Move::encode_move(piece_position, get_least_bit_index(pawn_attack_map), PAWN, NONE, 0, 0, 1, 0));
				}
			}
			else if (prev_move_double_push_flag) {
				uint64_t previous_move_target_square = square_bitmask[Move::decode_move(*previous_move, TARGET_SQUARE)];
				uint64_t possible_enpassant_bitboard = this->player_side == WHITE ? previous_move_target_square >> 8 : previous_move_target_square << 8;
				pawn_attack_map &= possible_enpassant_bitboard;
				if (pawn_attack_map)
					moves.add_move(Move::encode_move(piece_position, get_least_bit_index(pawn_attack_map), PAWN, NONE, 0, 0, 1, 0));
			}
		}
		piece_bitboard &= piece_bitboard - 1;
	}
}

void Player::generate_knight_moves(uint64_t piece_bitboard, uint64_t opponent_state, uint64_t opponent_attacks, unsigned int check_flags, uint64_t attackers_ray, uint64_t kings_ray, uint64_t opponent_sliding_pieces, uint64_t * opponent_pieces, Positions king_position, bool knight_check_flag, uint64_t attacker_mask)
{
	while (piece_bitboard) {
		Positions piece_position = (Positions)get_least_bit_index(piece_bitboard);
		uint64_t piece_mask = square_bitmask[piece_position];
		uint64_t knight_attack_map = knight_attack_maps[piece_position];
		knight_attack_map &= ~this->player_state;
		knight_attack_map &= attackers_ray;
		if (knight_attack_map) {
			bool possible_pin = kings_ray & square_bitmask[piece_position];
			uint64_t valid_ray = is_discovered_check(piece_position, opponent_sliding_pieces, opponent_pieces, king_position, kings_ray, possible_pin);
			knight_attack_map &= valid_ray;
			knight_attack_map &= attacker_mask;
			while (knight_attack_map) {
				int attack_position = get_least_bit_index(knight_attack_map);
				bool is_attack_position = opponent_state & square_bitmask[attack_position];
				moves.add_move(Move::encode_move(piece_position, attack_position, KNIGHT, NONE, is_attack_position, 0, 0, 0));
				knight_attack_map &= knight_attack_map - 1;
			}
		}
		piece_bitboard &= piece_bitboard - 1;
	}
}

void Player::generate_bishop_moves(uint64_t piece_bitboard, uint64_t opponent_state, uint64_t opponent_attacks, unsigned int check_flags, uint64_t attackers_ray, uint64_t kings_ray, uint64_t opponent_sliding_pieces, uint64_t * opponent_pieces, Positions king_position, bool knight_check_flag, uint64_t attacker_mask)
{
	while (piece_bitboard) {
		Positions piece_position = (Positions)get_least_bit_index(piece_bitboard);
		uint64_t piece_mask = square_bitmask[piece_position];
		uint64_t bishop_attack_map = get_bishop_attacks(piece_position, *this->board_state);
		uint64_t ray_opposite_to_king_square = 0ull;
		bishop_attack_map &= ~this->player_state;
		bishop_attack_map &= attackers_ray;
		if (bishop_attack_map) {
			bool possible_pin = kings_ray & square_bitmask[piece_position];
			uint64_t valid_ray = is_discovered_check(piece_position, opponent_sliding_pieces, opponent_pieces, king_position, kings_ray, possible_pin);
			bishop_attack_map &= valid_ray;
			bishop_attack_map &= attacker_mask;
			while (bishop_attack_map) {
				int attack_position = get_least_bit_index(bishop_attack_map);
				bool is_attack_position = opponent_state & square_bitmask[attack_position];
				moves.add_move(Move::encode_move(piece_position, attack_position, BISHOP, NONE, is_attack_position, 0, 0, 0));
				bishop_attack_map &= bishop_attack_map - 1;
			}
		}
		piece_bitboard &= piece_bitboard - 1;
	}
}

void Player::generate_rook_moves(uint64_t piece_bitboard, uint64_t opponent_state, uint64_t opponent_attacks, unsigned int check_flags, uint64_t attackers_ray, uint64_t kings_ray, uint64_t opponent_sliding_pieces, uint64_t * opponent_pieces, Positions king_position, bool knight_check_flag, uint64_t attacker_mask)
{
	while (piece_bitboard) {
		Positions piece_position = (Positions)get_least_bit_index(piece_bitboard);
		uint64_t piece_mask = square_bitmask[piece_position];
		uint64_t rook_attack_map = get_rook_attacks(piece_position, *this->board_state);
		uint64_t ray_opposite_to_king_square = 0ull;
		rook_attack_map &= ~this->player_state;
		rook_attack_map &= attackers_ray;
		if (rook_attack_map) {
			bool possible_pin = kings_ray & square_bitmask[piece_position];
			uint64_t valid_ray = is_discovered_check(piece_position, opponent_sliding_pieces, opponent_pieces, king_position, kings_ray, possible_pin);
			rook_attack_map &= valid_ray;
			rook_attack_map &= attacker_mask;
			while (rook_attack_map) {
				int attack_position = get_least_bit_index(rook_attack_map);
				bool is_attack_position = opponent_state & square_bitmask[attack_position];
				moves.add_move(Move::encode_move(piece_position, attack_position, ROOK, NONE, is_attack_position, 0, 0, 0));
				rook_attack_map &= rook_attack_map - 1;
			}
		}
		piece_bitboard &= piece_bitboard - 1;
	}
}

void Player::generate_queen_moves(uint64_t piece_bitboard, uint64_t opponent_state, uint64_t opponent_attacks, unsigned int check_flags, uint64_t attackers_ray, uint64_t kings_ray, uint64_t opponent_sliding_pieces, uint64_t * opponent_pieces, Positions king_position, bool knight_check_flag, uint64_t attacker_mask)
{
	while (piece_bitboard) {
		Positions piece_position = (Positions)get_least_bit_index(piece_bitboard);
		uint64_t piece_mask = square_bitmask[piece_position];
		uint64_t queen_attack_map = get_bishop_attacks(piece_position, *this->board_state) | get_rook_attacks(piece_position, *this->board_state);
		queen_attack_map &= ~this->player_state;
		queen_attack_map &= attackers_ray;
		if (queen_attack_map) {
			bool possible_pin = kings_ray & square_bitmask[piece_position];
			uint64_t valid_ray = is_discovered_check(piece_position, opponent_sliding_pieces, opponent_pieces, king_position, kings_ray, possible_pin);
			queen_attack_map &= valid_ray;
			queen_attack_map &= attacker_mask;
			while (queen_attack_map) {
				int attack_position = get_least_bit_index(queen_attack_map);
				bool is_attack_position = opponent_state & square_bitmask[attack_position];
				moves.add_move(Move::encode_move(piece_position, attack_position, QUEEN, NONE, is_attack_position, 0, 0, 0));
				queen_attack_map &= queen_attack_map - 1;
			}
		}
		piece_bitboard &= piece_bitboard - 1;
	}
}

void Player::generate_king_moves(uint64_t piece_bitboard, uint64_t opponent_state, uint64_t opponent_attacks, unsigned int check_flags, uint64_t attackers_ray, uint64_t kings_ray, uint64_t opponent_sliding_pieces, uint64_t * opponent_pieces, Positions king_position, bool knight_check_flag, uint64_t attacker_mask)
{
	bool in_check = Move::decode_check_flag(check_flags, CHECK_DECODE_ATTRIBUTES::CHECK);
	while (piece_bitboard) {
		int piece_position = get_least_bit_index(piece_bitboard);
		uint64_t piece_position_mask = square_bitmask[piece_position];
		uint64_t king_attack_map = king_attack_maps[piece_position];
		king_attack_map &= ~this->player_state;
		king_attack_map &= ~opponent_attacks;
		if (in_check && !knight_check_flag) {
			PieceName attacker_piece_name = (PieceName)Move::decode_check_flag(check_flags, CHECK_DECODE_ATTRIBUTES::ATTACKER_PIECE_NAME);
			Positions attacker_piece_position = (Positions)get_least_bit_index(attacker_mask);
			uint64_t check_piece_attack = get_piece_attacks(attacker_piece_name, attacker_piece_position);
			king_attack_map &= ~check_piece_attack;
		}
		while (king_attack_map) {
			int attack_position = get_least_bit_index(king_attack_map);
			bool is_attack_position = opponent_state & square_bitmask[attack_position];
			moves.add_move(Move::encode_move(piece_position, attack_position, KING, NONE, is_attack_position, 0, 0, 0));
			king_attack_map &= king_attack_map - 1;
		}
		if (!Move::decode_check_flag(check_flags, CHECK_DECODE_ATTRIBUTES::BOTH_CHECK)) {
			switch (player_side) {
			case WHITE:
				if (Move::decode_castle_rights(castling_rights, CASTLE_DECODE_ATTRIBUTES::WHITE_BOTH)) {
					if (this->can_castle(CASTLE_DECODE_ATTRIBUTES::WHITE_KING_SIDE, white_king_side_castle_occupancy, opponent_attacks, piece_position_mask))
						moves.add_move(Move::encode_move(piece_position, g1, KING, NONE, 0, 0, 0, 1));
					if (this->can_castle(CASTLE_DECODE_ATTRIBUTES::WHITE_QUEEN_SIDE, white_queen_side_castle_occupancy, opponent_attacks, piece_position_mask))
						moves.add_move(Move::encode_move(piece_position, c1, KING, NONE, 0, 0, 0, 1));
				}
				break;
			case BLACK:
				if (Move::decode_castle_rights(castling_rights, CASTLE_DECODE_ATTRIBUTES::BLACK_BOTH)) {
					if (this->can_castle(CASTLE_DECODE_ATTRIBUTES::BLACK_KING_SIDE, black_king_side_castle_occupancy, opponent_attacks, piece_position_mask))
						moves.add_move(Move::encode_move(piece_position, g8, KING, NONE, 0, 0, 0, 1));
					if (this->can_castle(CASTLE_DECODE_ATTRIBUTES::BLACK_QUEEN_SIDE, black_queen_side_castle_occupancy, opponent_attacks, piece_position_mask))
						moves.add_move(Move::encode_move(piece_position, c8, KING, NONE, 0, 0, 0, 1));
				}
				break;
			}
		}
		piece_bitboard &= piece_bitboard - 1;
	}
}

bool Player::can_castle(CASTLE_DECODE_ATTRIBUTES attribute_name, uint64_t castle_occupancy, uint64_t opponent_attacks, uint64_t king_position_mask)
{
	uint64_t piece_occupancy_bit = (attribute_name == WHITE_QUEEN_SIDE ? square_bitmask[b1] : attribute_name == BLACK_QUEEN_SIDE ? square_bitmask[b8] : 0ull);
	if (!Move::decode_castle_rights(castling_rights, attribute_name))
		return false;
	else if ((~*this->board_state & (castle_occupancy | piece_occupancy_bit)) != (castle_occupancy | piece_occupancy_bit))
		return false;
	else if (castle_occupancy != (castle_occupancy & ~opponent_attacks))
		return false;
	else if (king_position_mask & opponent_attacks)
		return false;
	else
		return true;
}

bool Player::is_incheck(uint64_t opponent_attacks)
{
	return this->player_pieces_state[KING] & opponent_attacks;
}

uint64_t Player::get_opponent_attacks(unsigned int& check_flags, uint64_t * opponent_pieces)
{
	uint64_t opponent_attacks = 0ull;
	Player_Side opponent_side = (Player_Side)!this->player_side;
	uint64_t player_king_mask = this->player_pieces_state[KING];
	bool knight_attack = false;
	unsigned int attacker_position = Positions::OUT_OF_BOUNDS;
	unsigned int attackers_name = 7;
	int check_count = 0;
	for (int piece = PieceName::PAWN; piece <= PieceName::KING; piece++) {
		uint64_t opponent_piece = opponent_pieces[piece];
		switch (piece)
		{
		case PieceName::PAWN:
			while (opponent_piece)
			{
				Positions piece_position = (Positions)get_least_bit_index(opponent_piece);
				uint64_t piece_attack = pawn_attack_maps[opponent_side][piece_position];
				if (piece_attack & player_king_mask) {
					check_count++;
					attacker_position = piece_position;
					attackers_name = piece;
				}
				opponent_attacks |= piece_attack;
				opponent_piece &= opponent_piece - 1;
			}
			break;
		case PieceName::KNIGHT:
			while (opponent_piece)
			{
				Positions piece_position = (Positions)get_least_bit_index(opponent_piece);
				uint64_t piece_attack = knight_attack_maps[piece_position];
				if (piece_attack & player_king_mask) {
					check_count++;
					knight_attack = 1;
					attacker_position = piece_position;
					attackers_name = piece;
				}
				opponent_attacks |= piece_attack;;
				opponent_piece &= opponent_piece - 1;
			}
			break;
		case PieceName::BISHOP:
			while (opponent_piece)
			{
				Positions piece_position = (Positions)get_least_bit_index(opponent_piece);
				uint64_t piece_attack = get_bishop_attacks(piece_position, *this->board_state);
				if (piece_attack & player_king_mask) {
					check_count++;
					attacker_position = piece_position;
					attackers_name = piece;
				}
				opponent_attacks |= piece_attack;
				opponent_piece &= opponent_piece - 1;

			}
			break;
		case PieceName::ROOK:
			while (opponent_piece)
			{
				Positions piece_position = (Positions)get_least_bit_index(opponent_piece);
				uint64_t piece_attack = get_rook_attacks(piece_position, *this->board_state);
				if (piece_attack & player_king_mask) {
					check_count++;
					attacker_position = piece_position;
					attackers_name = piece;
				}
				opponent_attacks |= piece_attack;
				opponent_piece &= opponent_piece - 1;
			}
			break;
		case PieceName::QUEEN:
			while (opponent_piece)
			{
				Positions piece_position = (Positions)get_least_bit_index(opponent_piece);
				uint64_t piece_attack = (get_rook_attacks(piece_position, *this->board_state) | get_bishop_attacks(piece_position, *this->board_state));
				if (piece_attack & player_king_mask) {
					check_count++;
					attacker_position = piece_position;
					attackers_name = piece;
				}
				opponent_attacks |= piece_attack;
				opponent_piece &= opponent_piece - 1;
			}
			break;
		case PieceName::KING:
			while (opponent_piece)
			{
				Positions piece_position = (Positions)get_least_bit_index(opponent_piece);
				uint64_t piece_attack = king_attack_maps[piece_position];
				opponent_attacks |= piece_attack;
				opponent_piece &= opponent_piece - 1;
			}
			break;
		}
	}
	check_flags = Move::encode_check_flag((check_count > 2 ? 2 : check_count), (int)knight_attack, attacker_position, attackers_name);
	in_check = check_count > 0;
	return opponent_attacks;
}

uint64_t Player::get_attackers_ray(unsigned int check_flags)
{
	Positions piece_position = (Positions)Move::decode_check_flag(check_flags, CHECK_DECODE_ATTRIBUTES::ATTACKER_POSITION);
	int king_square = get_least_bit_index(this->player_pieces_state[KING]);
	uint64_t piece_ray = 0ull;
	int delta_file = king_square % 8 - piece_position % 8;
	int delta_rank = king_square / 8 - piece_position / 8;
	if (delta_file == 0) {
		int direction = (delta_rank > 0) ? 1 : -1;
		int square = piece_position + 8 * direction;
		while (square != king_square) {
			piece_ray |= (1ull << square);
			square += 8 * direction;
		}
	}
	else if (delta_rank == 0) {
		int direction = (delta_file > 0) ? 1 : -1;
		int square = piece_position + direction;
		while (square != king_square) {
			piece_ray |= (1ull << square);
			square += direction;
		}
	}
	else if (abs(delta_file) == abs(delta_rank)) {
		int file_direction = (delta_file > 0) ? 1 : -1;
		int rank_direction = (delta_rank > 0) ? 1 : -1;
		int square = piece_position + 8 * rank_direction + file_direction;
		while (square != king_square) {
			piece_ray |= (1ull << square);
			square += 8 * rank_direction + file_direction;
		}
	}
	piece_ray |= (1ull << piece_position);
	return piece_ray;
}

uint64_t Player::get_piece_attacks(PieceName piece_name, Positions piece_position)
{
	uint64_t player_pieces_without_king = *this->board_state & ~square_bitmask[get_least_bit_index(this->player_pieces_state[KING])];
	switch (piece_name)
	{
	case PAWN:
		return pawn_attack_maps[!this->player_side][piece_position];
	case KNIGHT:
		return knight_attack_maps[piece_position];
	case KING:
		return king_attack_maps[piece_position];
	case BISHOP:
		return get_bishop_attacks(piece_position, player_pieces_without_king);
	case ROOK:
		return get_rook_attacks(piece_position, player_pieces_without_king);
	case QUEEN:
		return get_rook_attacks(piece_position, player_pieces_without_king) | get_bishop_attacks(piece_position, player_pieces_without_king);
	default:
		return 0ull;
	}
}

uint64_t Player::generate_ray_opposite_to_kings_square(Positions piece_position, uint64_t opponent_sliding_pieces, uint64_t piece_bitmask, Directions & ray_direction)
{
	Positions king_square = (Positions)get_least_bit_index(this->player_pieces_state[KING]);

	uint64_t piece_ray = 0ull;
	int delta_file = king_square % 8 - piece_position % 8;
	int delta_rank = king_square / 8 - piece_position / 8;
	if (delta_file == 0) {
		int direction = (delta_rank > 0) ? -1 : 1;
		if (((piece_bitmask & top_edge) && (direction == -1)) || ((piece_bitmask & bottom_edge) && (direction == 1)))
			return 0ull;
		int square = piece_position + 8 * direction;
		uint64_t ray_mask = square_bitmask[square];
		while (true) {
			piece_ray |= ray_mask;
			if ((ray_mask & *this->board_state) || ((ray_mask & top_edge) && (direction == -1)) || ((ray_mask & bottom_edge) && (direction == 1)))
				break;
			square += 8 * direction;
			ray_mask = square_bitmask[square];
		}
		ray_direction = STRAIGHT;
	}
	else if (delta_rank == 0) {
		int direction = (delta_file > 0) ? -1 : 1;
		if (((piece_bitmask & left_edge) && (direction == -1)) || ((piece_bitmask & right_edge) && (direction == 1)))
			return 0ull;
		int square = piece_position + direction;
		uint64_t ray_mask = square_bitmask[square];
		while (true) {
			piece_ray |= ray_mask;
			if ((ray_mask & *this->board_state) || ((ray_mask & left_edge) && (direction == -1)) || ((ray_mask & right_edge) && (direction == 1)))
				break;
			square += direction;
			ray_mask = square_bitmask[square];
		}
		ray_direction = STRAIGHT;
	}
	else if (abs(delta_file) == abs(delta_rank)) {
		int file_direction = (delta_file > 0) ? -1 : 1;
		int rank_direction = (delta_rank > 0) ? -1 : 1;
		if (((piece_bitmask & (left_edge | bottom_edge)) && (file_direction == -1 && rank_direction == 1))
			||
			((piece_bitmask & (right_edge | top_edge)) && (file_direction == 1 && rank_direction == -1))
			||
			((piece_bitmask & (left_edge | top_edge)) && (file_direction == -1 && rank_direction == -1))
			||
			((piece_bitmask & (right_edge | bottom_edge)) && (file_direction == 1 && rank_direction == 1)))
			return 0ull;
		int square = piece_position + 8 * rank_direction + file_direction;
		uint64_t ray_mask = square_bitmask[square];
		while (true) {
			piece_ray |= ray_mask;
			if ((ray_mask & *this->board_state) ||
				(((ray_mask & (left_edge | bottom_edge)) && (file_direction == -1 && rank_direction == 1))
					||
					((ray_mask & (right_edge | top_edge)) && (file_direction == 1 && rank_direction == -1))
					||
					((ray_mask & (left_edge | top_edge)) && (file_direction == -1 && rank_direction == -1))
					||
					((ray_mask & (right_edge | bottom_edge)) && (file_direction == 1 && rank_direction == 1))))
				break;
			square += 8 * rank_direction + file_direction;
			ray_mask = square_bitmask[square];
		}
		ray_direction = SLAINT;
	}
	return piece_ray;
}

uint64_t Player::get_ray_in_the_direction_of_king_square(Positions piece_position, PieceName piece_name)
{
	int king_square = get_least_bit_index(this->player_pieces_state[KING]);
	uint64_t piece_ray = 0ull;
	int delta_file = king_square % 8 - piece_position % 8;
	int delta_rank = king_square / 8 - piece_position / 8;
	if ((delta_file == 0) && (piece_name == QUEEN || piece_name == ROOK)) {
		int direction = (delta_rank > 0) ? 1 : -1;
		int square = piece_position + 8 * direction;
		while (square != king_square) {
			piece_ray |= (1ull << square);
			square += 8 * direction;
		}
	}
	else if ((delta_rank == 0) && (piece_name == QUEEN || piece_name == ROOK)) {
		int direction = (delta_file > 0) ? 1 : -1;
		int square = piece_position + direction;
		while (square != king_square) {
			piece_ray |= (1ull << square);
			square += direction;
		}
	}
	else if ((abs(delta_file) == abs(delta_rank)) && (piece_name == QUEEN || piece_name == BISHOP)) {
		int file_direction = (delta_file > 0) ? 1 : -1;
		int rank_direction = (delta_rank > 0) ? 1 : -1;
		int square = piece_position + 8 * rank_direction + file_direction;
		while (square != king_square) {
			piece_ray |= (1ull << square);
			square += 8 * rank_direction + file_direction;
		}
	}
	piece_ray &= ~this->player_pieces_state[KING];
	return piece_ray;
}

bool Player::is_discovered_check(uint64_t * opponent_pieces, Directions direction, uint64_t ray_opposite_to_king_square)
{
	switch (direction)
	{
	case SLAINT:
	{
		uint64_t opponent_sliding_pieces = opponent_pieces[BISHOP] | opponent_pieces[QUEEN];
		return ray_opposite_to_king_square & opponent_sliding_pieces;
	}
	case STRAIGHT:
	{
		uint64_t opponent_sliding_pieces = opponent_pieces[ROOK] | opponent_pieces[QUEEN];
		return ray_opposite_to_king_square & opponent_sliding_pieces;
	}
	}
	return false;
}

Positional_Directions Player::get_direction_relative_to_piece(Positions piece_1_position, Positions piece_2_position)
{
	int rank_difference = piece_1_position / 8 - piece_2_position / 8;
	int file_difference = piece_1_position % 8 - piece_2_position % 8;
	if (rank_difference == 0 || file_difference == 0) {
		if (rank_difference < 0)
			return Positional_Directions::SOUTH;
		else if (rank_difference > 0)
			return Positional_Directions::NORTH;
		else if (file_difference > 0)
			return Positional_Directions::WEST;
		else
			return Positional_Directions::EAST;
	}
	else if (abs(rank_difference) == abs(file_difference)) {
		if (rank_difference == file_difference) {
			if (rank_difference < 0)
				return Positional_Directions::SOUTH_EAST;
			else
				return Positional_Directions::NORTH_WEST;
		}
		else {
			if (rank_difference < 0)
				return Positional_Directions::SOUTH_WEST;
			else
				return Positional_Directions::NORTH_EAST;
		}
	}
	else return Positional_Directions::NOT_ALLIGNED;
}

uint64_t Player::is_discovered_check(Positions piece_position, uint64_t opponent_sliding_pieces, uint64_t * opponent_pieces, Positions king_position, uint64_t kings_ray, bool possible_pin)
{
	if (possible_pin) {
		Positional_Directions direction_with_king = get_direction_relative_to_piece(king_position, piece_position);
		uint64_t piece_rays = get_queen_attacks(piece_position, *this->board_state);
		uint64_t piece_ray_in_opposite_direction = directional_rays[direction_with_king][piece_position] & piece_rays;
		if (piece_ray_in_opposite_direction & opponent_sliding_pieces) {
			Directions general_direction = get_general_direction(direction_with_king);
			uint64_t relative_sliding_pieces = get_sliding_mask_wrt_attack_direction(general_direction, opponent_pieces);
			if (piece_ray_in_opposite_direction & relative_sliding_pieces) {
				uint64_t piece_ray_in_king_direction = directional_rays[direction_with_king][king_position] & kings_ray;
				return (piece_ray_in_king_direction | piece_ray_in_opposite_direction);
			}
		}
	}
	return ~0ull;
}

Directions Player::get_general_direction(Positional_Directions positional_direction)
{
	switch (positional_direction)
	{
	case EAST:
	case WEST:
	case NORTH:
	case SOUTH:
		return STRAIGHT;
	case NORTH_EAST:
	case NORTH_WEST:
	case SOUTH_EAST:
	case SOUTH_WEST:
		return SLAINT;
	default:
		return AMBIGIOUS;
	}
}

uint64_t Player::get_sliding_mask_wrt_attack_direction(Directions direction, uint64_t * opponent_pieces)
{
	if (direction == AMBIGIOUS)
		return 0ull;
	return opponent_pieces[QUEEN] | opponent_pieces[(direction == SLAINT ? BISHOP : ROOK)];
}

uint64_t Player::get_player_state()
{
	return this->player_state;
}

uint64_t* Player::get_ptr_player_state()
{
	return &this->player_state;
}

uint64_t* Player::get_player_pieces()
{
	return this->player_pieces_state;
}

Move Player::get_moves()
{
	return this->moves;
}

Player* Player::get_opponent_player()
{
	return this->opponent_player;
}

uint32_t Player::get_castling_rights()
{
	return this->castling_rights;
}

int* Player::get_enpassant_square()
{
	return &this->enpassant_square;
}

void Player::set_state(Player_state state)
{
	this->player_state = state.player_state;
	for (int i = PAWN; i <= KING; i++) {
		this->player_pieces_state[i] = state.player_pieces_state[i];
	}
	this->castling_rights = state.castling_rights;
	this->enpassant_square = state.enpassant_square;
}

uint64_t* Player::get_deep_copy_pieces()
{
	uint64_t* temp_player_pieces = new uint64_t[6];
	for (int piece = PAWN; piece <= KING; piece++)
		temp_player_pieces[piece] = this->player_pieces_state[piece];
	return temp_player_pieces;
}

void Player::remove_piece_from_player_state(uint64_t mask)
{
	this->player_state &= ~mask;
}

bool Player::is_player_in_check()
{
	return this->in_check;
}

inline void check_and_add_move_for_promotion() {

}
