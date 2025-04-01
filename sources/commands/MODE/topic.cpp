#include "MODE.hpp"

bool Mode::protectTopic(Channel &channel) {
	if (channel.mode & TOPIC_ONLY_OP)
		return false;
	channel.mode |= TOPIC_ONLY_OP;
	return true;
}

bool Mode::unprotectTopic(Channel &channel) {
	if (~channel.mode & TOPIC_ONLY_OP)
		return false;
	channel.mode &= ~TOPIC_ONLY_OP;
	return true;
}
