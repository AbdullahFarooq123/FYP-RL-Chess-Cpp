#pragma once
#include<iostream>
#include"Move.h"
class pgn_utilities
{
public:
	pgn_utilities();
	~pgn_utilities();
	static std::string encode_pgn(uint32_t move);
	static bool decode_pgn(std::string move);

private:

};


