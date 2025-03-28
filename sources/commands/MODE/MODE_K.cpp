#include "MODE.hpp"
#include "ERR_MSG.hpp"

// This check is implementation defined, but a key with spaces or comas would be difficult to use in JOIN messages
static bool checkKey(std::string key) {
	return std::
}

bool Mode::enableKey(Client &client, Channel &channel, std::string *key) {
	if (!key) {
		ERR_NEEDMOREPARAMS(client, "MODE", "Missing <key> parameter");
		return false;
	}
	// TODO: check if the key is valid (doesn't contain spaces nor comas)
	channel.mode |= KEY_PROTECTED;
	channel.key = *key;
}

case 'k':
					std::cout << "le mot de passe est bien enleve" << std::endl;
					_channel_list[channel].mode &= ~KEY_PROTECTED;
					break;
