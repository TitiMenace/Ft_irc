#include "Server.hpp"
#include "MODE.hpp"
#include "includes.hpp"
#include "Channel.hpp"
#include "RPL.hpp"
#include "parsingUtils.hpp"


// i: Set/remove Invite-only channel
// t: Set/remove the restrictions of the TOPIC command to channel operators
// k: Set/remove the channel key (password)
// o: Give/take channel operator privilege
// l: Set/remove the user limit to channel

bool	Mode::enableMode(Client &client, Channel &channel, char flag, std::string *param, std::map<int, Client> &users) {
	switch (flag) {
		case 'i': return restrictInvites(channel);
		case 't': return protectTopic(channel);
		case 'k': return enableKey(client, channel, param);
		case 'o': return promoteMember(client, channel, param, users);
		case 'l': return enableMemberLimit(client, channel, param);
	}
	ERR_UNKNOWNMODE(client, std::string(&flag, 1));
	return false;
}

bool	Mode::disableMode(Client &client, Channel &channel, char flag, std::string *param, std::map<int, Client> &users) {
	(void) param;
	switch (flag) {
		case 'i': return allowInvites(channel);
		case 't': return unprotectTopic(channel);
		case 'k': return disableKey(channel);
		case 'o': return demoteMember(client, channel, param, users);
		case 'l': return disableMemberLimit(channel);
	}
	ERR_UNKNOWNMODE(client, std::string(&flag, 1));
	return false;
}

bool	Mode::changeMode(Client &client, Channel &channel, std::string flag, std::string *param, std::map<int, Client> &users){
	if (flag.size() != 2) {
		ERR_UNKNOWNMODE(client, flag);
		return false;
	}
	
	if (flag[0] == '+')
		return Mode::enableMode(client, channel, flag[1], param, users);
	if (flag[0] == '-')
		return Mode::disableMode(client, channel, flag[1], param, users);
	
	ERR_UNKNOWNMODE(client, flag);
	return false;
}

void MSG_MODE(Client &client, std::string source, std::string channel, Message message){
	std::stringstream output;

	output << ":" << source;
	output << " MODE " << channel;
	output << " " << message.params[1];
	if (message.params.size() >= 3)
		output << " :" << message.params[2];
	output << "\r\n";
    client.outBuffer += output.str();
}

void Server::mode(Message message, Client &client) {
	std::cout << "mode appele avec " << message.params.size() << " arguments" << std::endl;

	if (client.state != REGISTERED) {
		ERR_NOTREGISTERED(client);
		return;
	}

	if (message.params.empty()){
		std::cout << "pas de arguments de mod" << std::endl;
		ERR_NEEDMOREPARAMS(client, "MODE", "Missing <target> parameter");
		return;
	}

	std::string channel_name = message.params[0];
	Channel *channel = findInMap(_channel_list, channel_name);
	if (!channel) {
		ERR_NOSUCHCHANNEL(client, channel_name);
		return;
	}

	if (message.params.size() < 2) {
		RPL_CHANNELMODEIS(*channel, client);
		return;
	}

	// TODO: NOTONCHANNEL
	if (!channel->operators.count(client.socket_fd)) {
		if (!channel->members.count(client.socket_fd))
			ERR_NOTONCHANNEL(client, channel_name);
		else
			ERR_CHANOPRIVSNEEDED(client, channel_name);
		return;
	}

	if (Mode::changeMode(client, *channel, message.params[1], (message.params.size() >= 3) ? &message.params[2] : NULL, _users)) {
		// Send mode message to channel members
		// TODO: maybe hide sensitive info (key in MODE +k?)
		for (std::set<int>::iterator it = channel->members.begin(); it != channel->members.end(); it++) {
			MSG_MODE(_users[*it], client.nickname, channel_name, message);
		}
	}
}
