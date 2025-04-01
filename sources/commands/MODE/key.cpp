#include "MODE.hpp"
#include "ERR_MSG.hpp"

// This check is implementation defined, but a key with spaces or comas would be difficult to use in JOIN messages
static bool checkKey(std::string key) {
	return key.find(' ') == std::string::npos && key.find(',') == std::string::npos;
}

bool Mode::enableKey(Client &client, Channel &channel, std::string *key) {
	if (!key) {
		ERR_NEEDMOREPARAMS(client, "MODE", "Missing <key> parameter");
		return false;
	}
	if (!checkKey(*key)) {
		ERR_INVALIDKEY(client, channel.name);
		return false;
	}
	channel.mode |= KEY_PROTECTED;
	channel.key = *key;
	return true;
}

bool Mode::disableKey(Channel &channel) {
	if (~channel.mode & KEY_PROTECTED)
		return false;
	channel.mode &= ~KEY_PROTECTED;
	return true;
}
