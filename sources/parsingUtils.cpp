#include "includes.hpp"

bool matchChar(std::string &buffer, std::size_t &buffer_pos, char c) {
	if (buffer_pos == buffer.length() || buffer[buffer_pos] != c)
		return false;
	buffer_pos++;
	return true;
}

bool charInSet(char c, std::string set) {
	return set.find(c) != std::string::npos;
}

bool parseSpaces(std::string &buffer, std::size_t &buffer_pos) {
	if (!matchChar(buffer, buffer_pos, ' '))
		return false;
	while (matchChar(buffer, buffer_pos, ' '))
		;
	return true;
}
