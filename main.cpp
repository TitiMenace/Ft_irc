#include <iostream>
		
int main(int __attribute__((unused))argc, char **__attribute__((unused))argv)
{
    if (argc <= 2)
        std::cout << "Not enough arguments" << std::endl << "./ircserv <port> <password>" << std::endl;
    if (argc > 3)
        std::cout << "Too many arguments" << std::endl << "./ircserv <port> <password>" << std::endl;
}