#pragma once
#include<iostream>
class Iterator
{
private:
	uint32_t* ptr;

public:
	Iterator(uint32_t* ptr);

	int operator*() const;

	bool operator!=(const Iterator& other) const;

	Iterator& operator++();
};

