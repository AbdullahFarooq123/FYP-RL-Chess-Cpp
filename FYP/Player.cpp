#include "Player.h"
#include"game_pretty.h"


Player::Player(Side player_side, uint64_t* board_state, uint64_t player_state, uint64_t player_pieces_state[6], int enpassant_square, uint32_t* previous_move, unsigned int castle_rights)
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
}

Player::~Player()
{
}

void Player::generate_moves()
{
	moves.reset_moves();
	uint64_t opponent_state = (*this->board_state) & (~this->player_state);
	unsigned int check_flags = 0;
	uint64_t opponent_attacks = this->get_opponent_attacks(check_flags);
	uint64_t attackers_ray = (Move::decode_check_flag(check_flags, CHECK_DECODE_ATTRIBUTES::CHECK) ? get_attackers_ray(check_flags) : ~0ull);
	int piece_name = (Move::decode_check_flag(check_flags, CHECK_DECODE_ATTRIBUTES::DOUBLE_CHECK) ? PieceName::KING : PieceName::PAWN);
	for (; piece_name <= PieceName::KING; piece_name++) {
		uint64_t piece_bitboard = this->player_pieces_state[piece_name];
		switch (piece_name)
		{
		case PieceName::PAWN:
			generate_pawn_moves(piece_bitboard, opponent_state, opponent_attacks, check_flags, attackers_ray);
			break;
		case PieceName::KNIGHT:
			generate_knight_moves(piece_bitboard, opponent_state, opponent_attacks, check_flags, attackers_ray);
			break;
		case PieceName::BISHOP:
			generate_bishop_moves(piece_bitboard, opponent_state, opponent_attacks, check_flags, attackers_ray);
			break;
		case PieceName::ROOK:
			generate_rook_moves(piece_bitboard, opponent_state, opponent_attacks, check_flags, attackers_ray);
			break;
		case PieceName::QUEEN:
			generate_queen_moves(piece_bitboard, opponent_state, opponent_attacks, check_flags, attackers_ray);
			break;
		case PieceName::KING:
			generate_king_moves(piece_bitboard, opponent_state, opponent_attacks, check_flags, attackers_ray);
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
	bool capture_flag = (bool)Move::decode_move(move, CAPTURE_FLAG);
	bool double_push_flag = (bool)Move::decode_move(move, DOUBLE_PUSH_FLAG);
	bool en_passant_flag = (bool)Move::decode_move(move, EN_PASSANT_FLAG);
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
	player_pieces_state[piece_name] &= ~bitmask(source_square);
	player_pieces_state[piece_name] |= bitmask(target_square);
	*previous_move = move;
}

uint64_t Player::get_bitboard()
{
	return *this->board_state;
}

void Player::print_moves()
{
	for (uint32_t move : moves) {
		string source_square = str_positions[Move::decode_move(move, SOURCE_SQUARE)];
		string target_square = str_positions[Move::decode_move(move, TARGET_SQUARE)];
		string piece_name = str_piece_names[Move::decode_move(move, PIECE_NAME)];
		string promotion_piece_name = str_piece_names[Move::decode_move(move, PROMOTION_PIECE_NAME)];
		string capture_flag = ((bool)Move::decode_move(move, CAPTURE_FLAG) ? "CAPTURE" : "-");
		string double_push_flag = ((bool)Move::decode_move(move, DOUBLE_PUSH_FLAG) ? "DOUBLE PUSH" : "-");
		string en_passant_flag = ((bool)Move::decode_move(move, EN_PASSANT_FLAG) ? "EN-PASSANT" : "-");
		string castle_flag = ((bool)Move::decode_move(move, CASTLE_FLAG) ? "CASTLE" : "-");
		cout << piece_name << "(" << source_square << "->" << target_square << ") " << (promotion_piece_name == "NONE" ? "-" : promotion_piece_name) << ", " << capture_flag << ", " << double_push_flag << ", " << en_passant_flag << ", " << castle_flag << endl;
	}
}

void Player::set_opponent_player(Player * opponent_player)
{
	this->opponent_player = opponent_player;
}

void Player::generate_pawn_moves(uint64_t piece_bitboard, uint64_t opponent_state, uint64_t opponent_attacks, unsigned int check_flags, uint64_t attackers_ray)
{
	while (piece_bitboard) {
		int piece_position = get_least_bit_index(piece_bitboard);
		uint64_t piece_bitmap = bitmask(piece_position);
		bool is_promotion = (player_side == WHITE) ? piece_bitmap & before_top_edge : piece_bitmap & before_bottom_edge;
		//Pawn Attacks
		uint64_t pawn_attack_map = pawn_attack_maps[this->player_side][piece_position];
		pawn_attack_map &= opponent_state;
		pawn_attack_map &= attackers_ray;
		if (Move::decode_check_flag(check_flags, CHECK_DECODE_ATTRIBUTES::KNIGHT_CHECK) & pawn_attack_map)
			pawn_attack_map &= bitmask((Positions)Move::decode_check_flag(check_flags, CHECK_DECODE_ATTRIBUTES::ATTACKER_POSITION));
		while (pawn_attack_map) {
			int attack_position = get_least_bit_index(pawn_attack_map);
			if (is_promotion)
				for (int promotion_piece = KNIGHT; promotion_piece <= QUEEN; promotion_piece++)
					moves.add_move(Move::encode_move(piece_position, attack_position, PAWN, promotion_piece, 1, 0, 0, 0));
			else
				moves.add_move(Move::encode_move(piece_position, attack_position, PAWN, NONE, 1, 0, 0, 0));
			pawn_attack_map &= pawn_attack_map - 1;
		}
		if (!Move::decode_check_flag(check_flags, CHECK_DECODE_ATTRIBUTES::DOUBLE_CHECK) & !Move::decode_check_flag(check_flags, CHECK_DECODE_ATTRIBUTES::KNIGHT_CHECK)) {
			//Quite and double push moves
			uint64_t quite_move = 0ull;
			uint64_t double_push_move = 0ull;
			switch (player_side) {
			case WHITE:
				quite_move = (piece_bitmap >> 8);
				if (piece_bitmap & before_bottom_edge)
					double_push_move = (quite_move >> 8);
				break;
			case BLACK:
				quite_move = (piece_bitmap << 8);
				if (piece_bitmap & before_top_edge)
					double_push_move = (quite_move << 8);
				break;
			}
			quite_move &= ~*this->board_state;
			double_push_move &= ~*this->board_state;
			if (quite_move) {
				quite_move &= attackers_ray;
				int move_position = get_least_bit_index(quite_move);
				if (move_position != -1) {
					if (is_promotion)
						for (int promotion_piece = KNIGHT; promotion_piece <= QUEEN; promotion_piece++)
							moves.add_move(Move::encode_move(piece_position, move_position, PAWN, promotion_piece, 1, 0, 0, 0));
					else
						moves.add_move(Move::encode_move(piece_position, move_position, PAWN, NONE, 0, 0, 0, 0));
				}
				if (double_push_move) {
					double_push_move &= attackers_ray;
					int move_position = get_least_bit_index(double_push_move);
					if (move_position != -1)
						moves.add_move(Move::encode_move(piece_position, move_position, PAWN, NONE, 0, 1, 0, 0));
				}
			}
			//enpassant_capture
			if ((piece_position >= a5 && piece_position <= h4 && player_side == WHITE) || (piece_position >= a5 && piece_position <= h5 && player_side == BLACK)) {
				uint64_t enpassant_capture = pawn_attack_maps[this->player_side][piece_position];
				bool prev_double_push = Move::decode_move(*previous_move, DOUBLE_PUSH_FLAG);
				uint64_t enpassant_bitboard = bitmask(Move::decode_move(*previous_move, TARGET_SQUARE) + (player_side == WHITE ? -8 : +8));
				bool is_enpassant = enpassant_bitboard & enpassant_capture;
				if ((enpassant_square != OUT_OF_BOUNDS) && (bool)(bitmask(enpassant_square) & enpassant_capture)) {
					bool is_enpassant = bitmask(enpassant_square + (player_side == WHITE ? -8 : +8)) & *this->board_state;
					if (is_enpassant)
						moves.add_move(Move::encode_move(piece_position, enpassant_square, PAWN, NONE, 1, 0, 1, 0));
				}
				else if (prev_double_push && is_enpassant) {
					moves.add_move(Move::encode_move(piece_position, log2(enpassant_capture), PAWN, NONE, 1, 0, 1, 0));
				}
			}
		}
		piece_bitboard &= piece_bitboard - 1;
	}
}

void Player::generate_knight_moves(uint64_t piece_bitboard, uint64_t opponent_state, uint64_t opponent_attacks, unsigned int check_flags, uint64_t attackers_ray)
{
	while (piece_bitboard) {
		int piece_position = get_least_bit_index(piece_bitboard);
		uint64_t knight_attack_map = knight_attack_maps[piece_position];
		knight_attack_map &= ~this->player_state;
		knight_attack_map &= attackers_ray;
		if (Move::decode_check_flag(check_flags, CHECK_DECODE_ATTRIBUTES::KNIGHT_CHECK) & knight_attack_map)
			knight_attack_map &= bitmask((Positions)Move::decode_check_flag(check_flags, CHECK_DECODE_ATTRIBUTES::ATTACKER_POSITION));
		while (knight_attack_map) {
			int attack_position = get_least_bit_index(knight_attack_map);
			bool is_attack_position = opponent_state & bitmask(attack_position);
			moves.add_move(Move::encode_move(piece_position, attack_position, KNIGHT, NONE, is_attack_position, 0, 0, 0));
			knight_attack_map &= knight_attack_map - 1;
		}
		piece_bitboard &= piece_bitboard - 1;
	}
}

void Player::generate_bishop_moves(uint64_t piece_bitboard, uint64_t opponent_state, uint64_t opponent_attacks, unsigned int check_flags, uint64_t attackers_ray)
{
	while (piece_bitboard) {
		int piece_position = get_least_bit_index(piece_bitboard);
		uint64_t bishop_attack_map = get_bishop_attacks(piece_position, *this->board_state);
		bishop_attack_map &= ~this->player_state;
		bishop_attack_map &= attackers_ray;
		if (Move::decode_check_flag(check_flags, CHECK_DECODE_ATTRIBUTES::KNIGHT_CHECK) & bishop_attack_map)
			bishop_attack_map &= bitmask((Positions)Move::decode_check_flag(check_flags, CHECK_DECODE_ATTRIBUTES::ATTACKER_POSITION));
		while (bishop_attack_map) {
			int attack_position = get_least_bit_index(bishop_attack_map);
			bool is_attack_position = opponent_state & bitmask(attack_position);
			moves.add_move(Move::encode_move(piece_position, attack_position, BISHOP, NONE, is_attack_position, 0, 0, 0));
			bishop_attack_map &= bishop_attack_map - 1;
		}
		piece_bitboard &= piece_bitboard - 1;
	}
}

void Player::generate_rook_moves(uint64_t piece_bitboard, uint64_t opponent_state, uint64_t opponent_attacks, unsigned int check_flags, uint64_t attackers_ray)
{
	while (piece_bitboard) {
		int piece_position = get_least_bit_index(piece_bitboard);
		uint64_t rook_attack_map = get_rook_attacks(piece_position, *this->board_state);
		rook_attack_map &= ~this->player_state;
		rook_attack_map &= attackers_ray;
		if (Move::decode_check_flag(check_flags, CHECK_DECODE_ATTRIBUTES::KNIGHT_CHECK) & rook_attack_map)
			rook_attack_map &= bitmask((Positions)Move::decode_check_flag(check_flags, CHECK_DECODE_ATTRIBUTES::ATTACKER_POSITION));
		while (rook_attack_map) {
			int attack_position = get_least_bit_index(rook_attack_map);
			bool is_attack_position = opponent_state & bitmask(attack_position);
			moves.add_move(Move::encode_move(piece_position, attack_position, ROOK, NONE, is_attack_position, 0, 0, 0));
			rook_attack_map &= rook_attack_map - 1;
		}
		piece_bitboard &= piece_bitboard - 1;
	}
}

void Player::generate_queen_moves(uint64_t piece_bitboard, uint64_t opponent_state, uint64_t opponent_attacks, unsigned int check_flags, uint64_t attackers_ray)
{
	while (piece_bitboard) {
		int piece_position = get_least_bit_index(piece_bitboard);
		uint64_t queen_attack_map = get_bishop_attacks(piece_position, *this->board_state) | get_rook_attacks(piece_position, *this->board_state);
		queen_attack_map &= ~this->player_state;
		queen_attack_map &= attackers_ray;
		if (Move::decode_check_flag(check_flags, CHECK_DECODE_ATTRIBUTES::KNIGHT_CHECK) & queen_attack_map)
			queen_attack_map &= bitmask((Positions)Move::decode_check_flag(check_flags, CHECK_DECODE_ATTRIBUTES::ATTACKER_POSITION));
		while (queen_attack_map) {
			int attack_position = get_least_bit_index(queen_attack_map);
			bool is_attack_position = opponent_state & bitmask(attack_position);
			moves.add_move(Move::encode_move(piece_position, attack_position, QUEEN, NONE, is_attack_position, 0, 0, 0));
			queen_attack_map &= queen_attack_map - 1;
		}
		piece_bitboard &= piece_bitboard - 1;
	}
}

void Player::generate_king_moves(uint64_t piece_bitboard, uint64_t opponent_state, uint64_t opponent_attacks, unsigned int check_flags, uint64_t attackers_ray)
{

	while (piece_bitboard) {
		int piece_position = get_least_bit_index(piece_bitboard);
		uint64_t piece_position_mask = bitmask(piece_position);
		uint64_t king_attack_map = king_attack_maps[piece_position];
		king_attack_map &= ~this->player_state;
		king_attack_map &= ~opponent_attacks;
		piece_bitboard &= piece_bitboard - 1;
		while (king_attack_map) {
			int attack_position = get_least_bit_index(king_attack_map);
			bool is_attack_position = opponent_state & bitmask(attack_position);
			moves.add_move(Move::encode_move(piece_position, attack_position, KING, NONE, is_attack_position, 0, 0, 0));
			king_attack_map &= king_attack_map - 1;
		}
		if (!Move::decode_check_flag(check_flags, (CHECK_DECODE_ATTRIBUTES)(CHECK_DECODE_ATTRIBUTES::CHECK | CHECK_DECODE_ATTRIBUTES::DOUBLE_CHECK))) {
			switch (player_side) {
			case WHITE:
				if (Move::decode_castle_rights(castling_rights,(CASTLE_DECODE_ATTRIBUTES)(CASTLE_DECODE_ATTRIBUTES::WHITE_KING_SIDE|CASTLE_DECODE_ATTRIBUTES::WHITE_QUEEN_SIDE))) {
					if (this->can_castle(CASTLE_DECODE_ATTRIBUTES::WHITE_KING_SIDE, white_king_side_castle_occupancy, opponent_attacks, piece_position_mask))
						moves.add_move(Move::encode_move(piece_position, g1, KING, NONE, 0, 0, 0, 1));
					if (this->can_castle(CASTLE_DECODE_ATTRIBUTES::WHITE_QUEEN_SIDE, white_queen_side_castle_occupancy, opponent_attacks, piece_position_mask))
						moves.add_move(Move::encode_move(piece_position, c1, KING, NONE, 0, 0, 0, 1));
				}
				break;
			case BLACK:
				if (Move::decode_castle_rights(castling_rights, (CASTLE_DECODE_ATTRIBUTES)(CASTLE_DECODE_ATTRIBUTES::BLACK_KING_SIDE | CASTLE_DECODE_ATTRIBUTES::BLACK_QUEEN_SIDE))) {
					if (this->can_castle(CASTLE_DECODE_ATTRIBUTES::BLACK_KING_SIDE, black_king_side_castle_occupancy, opponent_attacks, piece_position_mask))
						moves.add_move(Move::encode_move(piece_position, g8, KING, NONE, 0, 0, 0, 1));
					if (this->can_castle(CASTLE_DECODE_ATTRIBUTES::BLACK_QUEEN_SIDE, black_queen_side_castle_occupancy, opponent_attacks, piece_position_mask))
						moves.add_move(Move::encode_move(piece_position, c8, KING, NONE, 0, 0, 0, 1));
				}
				break;
			}
		}
	}
}

bool Player::can_castle(CASTLE_DECODE_ATTRIBUTES attribute_name, uint64_t castle_occupancy, uint64_t opponent_attacks, uint64_t king_position_mask)
{
	return ((Move::decode_castle_rights(castling_rights,attribute_name)) && ((~*this->board_state & castle_occupancy) == castle_occupancy && castle_occupancy == (castle_occupancy & ~opponent_attacks)) && !(king_position_mask & opponent_attacks));
}

bool Player::is_incheck(uint64_t opponent_attacks)
{
	return this->player_pieces_state[KING] & opponent_attacks;
}

uint64_t Player::get_opponent_attacks(unsigned int& check_flags)
{
	uint64_t opponent_attacks = 0ull;
	uint64_t* opponent_pieces_state = this->opponent_player->get_player_pieces();
	Side opponent_side = (Side)~this->player_side;
	uint64_t player_king_mask = this->player_pieces_state[KING];
	bool knight_attack = false;
	unsigned int attacker_position = Positions::OUT_OF_BOUNDS;
	unsigned int attackers_name = 7;
	int check_count = 0;
	for (int piece = PieceName::PAWN; piece <= PieceName::KING; piece++) {
		uint64_t opponent_piece = opponent_pieces_state[piece];
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
	return opponent_attacks;
}

uint64_t Player::get_attackers_ray(unsigned int check_flags)
{
	int attacker_square = (Positions)(check_flags >> 3);
	int king_square = get_least_bit_index(this->player_pieces_state[KING]);
	uint64_t attack_ray = 0ull;
	int delta_file = king_square % 8 - attacker_square % 8;
	int delta_rank = king_square / 8 - attacker_square / 8;
	if (delta_file == 0) {
		int direction = (delta_rank > 0) ? 1 : -1;
		int square = attacker_square + 8 * direction;
		while (square != king_square) {
			attack_ray |= (1ull << square);
			square += 8 * direction;
		}
	}
	else if (delta_rank == 0) {
		int direction = (delta_file > 0) ? 1 : -1;
		int square = attacker_square + direction;
		while (square != king_square) {
			attack_ray |= (1ull << square);
			square += direction;
		}
	}
	else if (abs(delta_file) == abs(delta_rank)) {
		int file_direction = (delta_file > 0) ? 1 : -1;
		int rank_direction = (delta_rank > 0) ? 1 : -1;
		int square = attacker_square + 8 * rank_direction + file_direction;
		while (square != king_square) {
			attack_ray |= (1ull << square);
			square += 8 * rank_direction + file_direction;
		}
	}
	attack_ray |= (1ull << attacker_square);
	return attack_ray;
}

uint64_t Player::get_piece_attacks(PieceName piece_name, Positions piece_position)
{
	switch (piece_name)
	{
	case PAWN:
		return pawn_attack_maps[!this->player_side][piece_position];
	case KNIGHT:
		return knight_attack_maps[piece_position];
	case KING:
		return king_attack_maps[piece_position];
	case BISHOP:
		return get_bishop_attacks(piece_position, 0ull);
	case ROOK:
		return get_rook_attacks(piece_position, 0ull);
	case QUEEN:
		return get_rook_attacks(piece_position, 0ull) | get_bishop_attacks(piece_position, 0ull);
	default:
		return 0ull;
	}
}

uint64_t Player::get_player_state()
{
	return this->player_state;
}

uint64_t* Player::get_player_pieces()
{
	return this->player_pieces_state;
}
