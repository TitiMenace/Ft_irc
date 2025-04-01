#include "MODE.hpp"

bool Mode::restrictInvites(Channel &channel) {
	if (channel.mode & INVITE_ONLY)
		return false;
	channel.mode |= INVITE_ONLY;
	return true;
}

bool Mode::allowInvites(Channel &channel) {
	if (~channel.mode & INVITE_ONLY)
		return false;
	channel.mode &= ~INVITE_ONLY;
	return true;
}
