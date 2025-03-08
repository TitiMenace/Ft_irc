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

bool parseChanString(std::string &buffer, std::size_t &buffer_pos) {
    std::size_t i = buffer_pos;
    while (i < buffer.length()) {
        unsigned char ch = buffer[i];
        // Invalid characters: NUL (0x00), BELL (0x07), CR (0x0D), LF (0x0A), " " (0x20), "," (0x2C), ":" (0x3A)
        if (ch == 0x00 || ch == 0x07 || ch == 0x0A || ch == 0x0D || ch == 0x20 || ch == 0x2C || ch == 0x3A) {
            break;
        }
        i++;
    }
    if (i == buffer_pos) {
        return false;  // No valid characters found
    }
    buffer_pos = i;
    return true;
}

bool parseChannelID(std::string &buffer, std::size_t &buffer_pos) {
    std::size_t i = buffer_pos;

    for (int count = 0; count < 5; count++) {
        if (i >= buffer.length() || !(std::isdigit(buffer[i]) || (buffer[i] >= 'A' && buffer[i] <= 'Z'))) {
            return false; 
        }
        i++;
    }
    buffer_pos = i;
    return true;
}

// channel    =  ( "#" / "+" / ( "!" channelid ) / "&" ) chanstring
//                 [ ":" chanstring ]

bool parseChannel(std::string &buffer, std::size_t &buffer_pos) {
    if (buffer.length() <= 2 || buffer_pos >= buffer.length()) {
        return false;
    }
    char firstChar = buffer[buffer_pos];
    std::cout << firstChar << std::endl;
    if (firstChar != '#' && firstChar != '+' && firstChar != '&' && firstChar != '!') {
        return false;
    }
    buffer_pos++;
    if (firstChar == '!') {
        if (!parseChannelID(buffer, buffer_pos)) {
            return false;
        }
    }
    if (!parseChanString(buffer, buffer_pos)) {
        return false;
    }
    if (buffer_pos < buffer.length() && buffer[buffer_pos] == ':') {
        buffer_pos++;
        if (!parseChanString(buffer, buffer_pos)) {
            return false;
        }
    }
    if (buffer_pos != buffer.length() && buffer[buffer_pos] != ',')
        return false;
    return true;
}

bool parseChannelList(std::string buffer, std::vector<std::string> &out_list) {
    std::vector<std::string> params_channels_list;
    std::size_t buffer_pos = 0;
    
    while (buffer_pos < buffer.length()) {
        std::size_t start_pos = buffer_pos;
        if (!parseChannel(buffer, buffer_pos)) {
            return false;
        }
        params_channels_list.push_back(buffer.substr(start_pos, buffer_pos - start_pos));
        
        if (buffer_pos < buffer.length() && !matchChar(buffer, buffer_pos, ',')) {
            return false;
        }
    }
    
    out_list = params_channels_list;
    return true;
}


bool parseKeyList(std::string buffer, std::vector<std::string> &out_list) {
    std::vector<std::string> keys_list;
    std::size_t buffer_pos = 0;
    
    while (buffer_pos < buffer.length()) {
        std::size_t start_pos = buffer_pos;
        if (!parseChanString(buffer, buffer_pos)) {
            return false;
        }
        keys_list.push_back(buffer.substr(start_pos, buffer_pos - start_pos));
        
        if (buffer_pos < buffer.length() && !matchChar(buffer, buffer_pos, ',')) {
            return false;
        }
    }
    
    out_list = keys_list;
    return true;
}