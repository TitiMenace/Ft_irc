#pragma once

bool matchChar(std::string &buffer, std::size_t &buffer_pos, char c);

bool charInSet(char c, std::string set);

bool parseSpaces(std::string &buffer, std::size_t &buffer_pos);