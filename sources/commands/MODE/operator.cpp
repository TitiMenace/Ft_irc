#include "MODE.hpp"
#include "ERR_MSG.hpp"

bool	Mode::promoteMember(Client &client, Channel &channel, std::string *member, std::map<int, Client> &users) {
	if (!member) {
		ERR_NEEDMOREPARAMS(client, "MODE", "Missing <member> parameter");
		return false;
	}
	// Can't use findUser because it's declared in Server...
	std::map<int, Client>::iterator it;
	for (it = users.begin(); it != users.end(); it++)
		if (it->second.nickname == *member)
			break;
	if (it == users.end()) {
		ERR_NOSUCHNICK(client, *member);
		return false;
	}
	if (!channel.members.count(it->first)) {
		ERR_USERNOTINCHANNEL(client, *member, channel.name);
		return false;
	}
	if (channel.operators.count(it->first))
		return false;
	channel.operators.insert(it->first);
	return true;
}

bool	Mode::demoteMember(Client &client, Channel &channel, std::string *member, std::map<int, Client> &users) {
	if (!member) {
		ERR_NEEDMOREPARAMS(client, "MODE", "Missing <member> parameter");
		return false;
	}
	// As mentionned in promoteMember: can't use findUser because it's declared in Server...
	std::map<int, Client>::iterator it;
	for (it = users.begin(); it != users.end(); it++)
		if (it->second.nickname == *member)
			break;
	if (it == users.end()) {
		ERR_NOSUCHNICK(client, *member);
		return false;
	}
	if (!channel.members.count(it->first)) {
		ERR_USERNOTINCHANNEL(client, *member, channel.name);
		return false;
	}
	if (!channel.operators.count(it->first))
		return false;
	channel.operators.erase(it->first);
	return true;
}
