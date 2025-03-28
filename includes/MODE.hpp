#pragma once

#include "Channel.hpp"
#include <string>


namespace Mode {
	bool	enableMode(Client &client, Channel &channel, char flag, std::string *param);
	bool	disableMode(Client &client, Channel &channel, char flag, std::string *param);

	// Type A: o (or type B)
	// Type B: 
	// Type C: kl
	// Type D: it

	// +/-i
	bool	restrictInvites(Channel &channel);
	bool	allowInvites(Channel &channel);
	// +/-t
	bool	protectTopic(Client &client, Channel &channel);
	bool	unprotectTopic(Client &client, Channel &channel);
	// +/-k
	bool	enableKey(Client &client, Channel &channel, std::string *key);
	bool	disableKey(Client &client, Channel &channel);
	// +/-o
	bool	promoteMember(Client &client, Channel &channel, std::string *member);
	bool	demoteMember(Client &client, Channel &channel, std::string *member);
	// +/-l
	bool	enableMemberLimit(Client &client, Channel &channel, std::string *limit);
	bool	disableMemberLimit(Client &client, Channel &channel);
};