#pragma once

#include "includes.hpp"

struct Message {
	std::string command;
	std::vector<std::string> params;
};

bool matchChar(std::string &buffer, std::size_t &buffer_pos, char c);
bool parseSpaces(std::string &buffer, std::size_t &buffer_pos);
bool parseMessage(std::string &buffer, Message &out_message);
bool parseCommand(std::string &buffer, std::size_t &buffer_pos, std::string &out_command);
bool parseParam(std::string &buffer, std::size_t &buffer_pos, std::string &out_param);
void parseTrailing(std::string &buffer, std::size_t &buffer_pos, std::string &out_param);

