#pragma once
#include <unordered_map>
#include<iostream>

class Debug_utilities
{
public:
	Debug_utilities();
	~Debug_utilities();
private:
	std::unordered_map<std::string, int> move_generations;
};

