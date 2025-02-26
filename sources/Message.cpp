#include "Message.hpp"


bool matchChar(std::string &buffer, std::size_t &buffer_pos, char c) {
	if (buffer_pos == buffer.length() || buffer[buffer_pos] != c)
		return false;
	buffer_pos++;
	return true;
}

bool parseSpaces(std::string &buffer, std::size_t &buffer_pos) {
	if (!matchChar(buffer, buffer_pos, ' '))
		return false;
	while (matchChar(buffer, buffer_pos, ' '))
		;
	return true;
}

// message ::= <command> *( SPACE <param> ) [ SPACE : <trailing> ] <\r\n>
bool parseMessage(std::string &buffer, Message &out_message) {
	std::size_t buffer_pos = 0;
	Message message;

	if (!parseCommand(buffer, buffer_pos, message.command))
		return false;
	while (parseSpaces(buffer, buffer_pos)) {
		std::string param;

		if (!matchChar(buffer, buffer_pos, ':')) {
			if (!parseParam(buffer, buffer_pos, param))
				return false;
			message.params.push_back(param);
			continue;
		}
		parseTrailing(buffer, buffer_pos, param);
		message.params.push_back(param);
		break;
	}
	if (buffer_pos != buffer.length())
		return false;
	out_message = message;
	return true;
}

// command ::= [A-Za-z0-9]*
bool parseCommand(std::string &buffer, std::size_t &buffer_pos, std::string &out_command) {
	std::size_t i = buffer_pos;

	while (i < buffer.length() && isalnum(buffer[i]))
		i++;
	if (i == buffer_pos)
		return false;
	out_command = buffer.substr(buffer_pos, i - buffer_pos);
	buffer_pos = i;
	return true;
}


bool charInSet(char c, std::string set) {
	return set.find(c) != std::string::npos;
}

// param ::= [^\r\n\0: ][^\r\n\0 ]*
bool parseParam(std::string &buffer, std::size_t &buffer_pos, std::string &out_param) {
	std::size_t i = buffer_pos;

	if (charInSet(buffer[i], std::string("\r\n\0: ", 5)))
		return false;
	i++;
	while (i < buffer.length() && !charInSet(buffer[i], std::string("\r\n\0 ", 4)))
		i++;
	out_param = buffer.substr(buffer_pos, i - buffer_pos);
	buffer_pos = i;
	return true;
}

// trailing ::= [^\r\n\0]*
void parseTrailing(std::string &buffer, std::size_t &buffer_pos, std::string &out_param) {
	std::size_t i = buffer_pos;

	while (i < buffer.length() && !charInSet(buffer[i], std::string("\r\n\0", 3)))
		i++;
	out_param = buffer.substr(buffer_pos, i - buffer_pos);
	buffer_pos = i;
}
