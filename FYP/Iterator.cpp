#include "Iterator.h"
Iterator::Iterator(uint32_t* ptr):ptr(ptr)
{
}
int Iterator::operator*() const
{
	return *ptr;
}
bool Iterator::operator!=(const Iterator& other) const
{
	return ptr != other.ptr;
}
Iterator& Iterator::operator++()
{
	++ptr;
	return *this;
}
