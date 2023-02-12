#include "Engine.h"
#include<fcntl.h>
#include<io.h>
using namespace std;

Engine::Engine()
{
	Fen_utility fen = Fen_utility("5k2/8/8/8/4q3/8/4R1PP/4K3 w - - 0 11");
	this->board_state = fen.getBitboard();
	int enpassant_position = fen.get_enpassant();
	uint64_t white_state = fen.getBitboard(Side::WHITE);
	uint64_t black_state = fen.getBitboard(Side::BLACK);
	int castle_rights = fen.get_castle_rights();
	this->previous_move = 0;
	this->white_turn = fen.get_player_turn();
	uint64_t white_player_bitboards[6];
	uint64_t black_player_bitboards[6];
	Piece white_pieces[6] = { P,N,B,R,Q,K };
	Piece black_pieces[6] = { p,n,b,r,q,k };
	for (int piece = 0; piece < 6; piece++) {
		white_player_bitboards[piece] = fen.getBitboard(Side::WHITE, white_pieces[piece]);
		black_player_bitboards[piece] = fen.getBitboard(Side::BLACK, black_pieces[piece]);
	}
	white_player = new Player(Side::WHITE, &this->board_state, white_state, white_player_bitboards, enpassant_position, &this->previous_move,castle_rights);
	black_player = new Player(Side::BLACK, &this->board_state, black_state, black_player_bitboards, enpassant_position, &this->previous_move,castle_rights);
	white_player->set_opponent_player(black_player);
	black_player->set_opponent_player(white_player);
	init_precalculation_utilities();
}

Engine::~Engine()
{
	delete white_player;
	delete black_player;
}

void Engine::run()
{
	printAsciiBitboard(this->board_state,*white_player,*black_player);
	Player* current_player = this->white_turn ? white_player : black_player;
	current_player->generate_moves();
	current_player->print_moves();
	this->white_turn = !white_turn;
}

void Engine::validate_moves(Player* current_player)
{

}
