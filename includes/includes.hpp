#pragma once

#include <sys/socket.h>
#include <poll.h>
#include <iostream>
#include <cstring>
#include <exception>
#include <stdio.h> 
#include <map>
#include <vector> 
#include <netdb.h> 
#include <netinet/in.h> 
#include <stdlib.h> 
#include <string.h> 
#include <sys/types.h> 
#include <unistd.h> // read(), write(), close()
#include <sys/epoll.h>
#include <ctype.h>
#include "Message.hpp"
#include "Client.hpp"
#include "Server.hpp"