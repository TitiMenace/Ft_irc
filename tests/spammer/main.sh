#!/bin/bash

(
	echo "PASS blabla"
	echo "NICK velimir"
	echo "USER velimir * 0 velimir"
	while : ; do
		printf "PRIVMSG velomir "
		printf "%450s\n" | tr " " "a"
	done
) | telnet localhost 3000