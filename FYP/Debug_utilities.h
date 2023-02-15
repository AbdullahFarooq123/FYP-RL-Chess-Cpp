#pragma once
#include <unordered_map>
#include<iostream>
#include"Engine.h"

class Debug_utilities:protected Engine
{
public:
	Debug_utilities();
	~Debug_utilities();
	void run_make_unmake_debug();
	void engine_normal_run();
private:
	std::unordered_map<std::string, int> move_generations;
};

