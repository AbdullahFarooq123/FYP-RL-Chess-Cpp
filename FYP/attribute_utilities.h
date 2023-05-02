#pragma once
#include<iostream>
enum PieceName {
	PAWN,
	KNIGHT,
	BISHOP,
	ROOK,
	QUEEN,
	KING,
	NONE
};
enum Player_Side {
	WHITE = 1,
	BLACK = 0,
	BOTH = 2
};
enum Board_Edge {
	LEFT,
	RIGHT,
	B_RIGHT,
	B_LEFT
};
enum Positions {
	a8, b8, c8, d8, e8, f8, g8, h8,
	a7, b7, c7, d7, e7, f7, g7, h7,
	a6, b6, c6, d6, e6, f6, g6, h6,
	a5, b5, c5, d5, e5, f5, g5, h5,
	a4, b4, c4, d4, e4, f4, g4, h4,
	a3, b3, c3, d3, e3, f3, g3, h3,
	a2, b2, c2, d2, e2, f2, g2, h2,
	a1, b1, c1, d1, e1, f1, g1, h1, 
	OUT_OF_BOUNDS=-1
};
enum Piece {
	K = 75, Q = 81, R = 82, B = 66, N = 78, P = 80,
	k = 107, q = 113, r = 114, b = 98, n = 110, p = 112,
	ALL = 0
};
enum Directions {
	STRAIGHT=5,
	SLAINT=9,
	AMBIGIOUS
};
enum Positional_Directions {
	NORTH=1,
	EAST,
	SOUTH,
	WEST,
	NORTH_EAST,
	SOUTH_EAST,
	SOUTH_WEST,
	NORTH_WEST,
	NOT_ALLIGNED
};
enum MOVE_DECODE_ATTRIBUTES {
	SOURCE_SQUARE,
	TARGET_SQUARE,
	PIECE_NAME,
	PROMOTION_PIECE_NAME,
	CAPTURE_FLAG,
	DOUBLE_PUSH_FLAG,
	EN_PASSANT_FLAG,
	CASTLE_FLAG
};
enum CHECK_DECODE_ATTRIBUTES {
	CHECK,
	DOUBLE_CHECK,
	BOTH_CHECK,
	KNIGHT_CHECK,
	ATTACKER_POSITION,
	ATTACKER_PIECE_NAME
};
enum CASTLE_DECODE_ATTRIBUTES {
	WHITE_KING_SIDE,
	WHITE_QUEEN_SIDE,
	BLACK_KING_SIDE,
	BLACK_QUEEN_SIDE,
	WHITE_BOTH,
	BLACK_BOTH
};
const std::string str_positions[] = {
	"a8", "b8", "c8", "d8", "e8", "f8", "g8", "h8",
	"a7", "b7", "c7", "d7", "e7", "f7", "g7", "h7",
	"a6", "b6", "c6", "d6", "e6", "f6", "g6", "h6",
	"a5", "b5", "c5", "d5", "e5", "f5", "g5", "h5",
	"a4", "b4", "c4", "d4", "e4", "f4", "g4", "h4",
	"a3", "b3", "c3", "d3", "e3", "f3", "g3", "h3",
	"a2", "b2", "c2", "d2", "e2", "f2", "g2", "h2",
	"a1", "b1", "c1", "d1", "e1", "f1", "g1", "h1"
};
const std::string str_piece_names[] = {
	"PAWN", "KNIGHT", "BISHOP", "ROOK", "QUEEN", "KING", "NONE"
};
const char piece_unicodes[2][6] = {
	{'p','n','b','r','q','k'} ,
	{'P','N','B','R','Q','K'}
};