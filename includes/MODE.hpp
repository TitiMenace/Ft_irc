#pragma once

#include "Channel.hpp"
#include <string>
#include <map>

namespace Mode {
	bool	changeMode(Client &client, Channel &channel, std::string flag, std::string *param, std::map<int, Client> &users);
	bool	enableMode(Client &client, Channel &channel, char flag, std::string *param, std::map<int, Client> &users);
	bool	disableMode(Client &client, Channel &channel, char flag, std::string *param, std::map<int, Client> &users);

	// Type A: o (or type B)
	// Type B: 
	// Type C: kl
	// Type D: it

	// +/-i
	bool	restrictInvites(Channel &channel);
	bool	allowInvites(Channel &channel);
	// +/-t
	bool	protectTopic(Channel &channel);
	bool	unprotectTopic(Channel &channel);
	// +/-k
	bool	enableKey(Client &client, Channel &channel, std::string *key);
	bool	disableKey(Channel &channel);
	// +/-o
	bool	promoteMember(Client &client, Channel &channel, std::string *member, std::map<int, Client> &users);
	bool	demoteMember(Client &client, Channel &channel, std::string *member, std::map<int, Client> &users);
	// +/-l
	bool	enableMemberLimit(Client &client, Channel &channel, std::string *limit);
	bool	disableMemberLimit(Client &client, Channel &channel);
};