#include "MODE.hpp"
#include "ERR_MSG.hpp"
#include "parsingUtils.hpp"

bool Mode::enableMemberLimit(Client &client, Channel &channel, std::string *limit) {
	if (!limit) {
		ERR_NEEDMOREPARAMS(client, "MODE", "Missing <limit> parameter");
		return false;
	}
	uint16_t size_limit;
	if (!parseU16(*limit, size_limit)) {
		ERR_INVALIDMODEPARAM(client, channel.name, "l", *limit, ":Member limit should be an integer below 2^16");
		return false;
	}
	if (channel.mode & USER_LIMIT)
		return false;
	channel.mode |= USER_LIMIT;
	channel.size_limit = size_limit;
	return true;
}

bool Mode::disableMemberLimit(Channel &channel) {
	if (~channel.mode & USER_LIMIT)
		return false;
	channel.mode &= ~USER_LIMIT;
	return true;
}
