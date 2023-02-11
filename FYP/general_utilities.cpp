#include"general_utilities.h"
std::vector<std::string> split(const std::string& str, char delimiter) {
	std::vector<std::string> parts;
	std::stringstream ss(str);
	std::string item;
	while (std::getline(ss, item, delimiter))
		parts.push_back(item);
	return parts;
}