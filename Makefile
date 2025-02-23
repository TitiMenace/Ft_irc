
NAME 		= ircserv

CXX		= clang++

CFLAGS		= -g3 -Wall -Wextra -Werror -std=c++98

INCLUDES	= includes.hpp Server.hpp

FILES		= main Server

SRCS		= $(addsuffix .cpp, $(FILES))

OBJS_DIRS	= .build

OBJS		= 	$(patsubst %.cpp, $(OBJS_DIRS)/%.o, $(SRCS))

all:	$(NAME)

$(NAME) : $(OBJS)
		$(CXX) $(OBJS) -o $@

$(OBJS_DIRS)/%.o: %.cpp $(INCLUDES)
		@mkdir -p $(OBJS_DIRS)
		$(CXX) $(CFLAGS) -o $@ -c $<

clean:
		rm -rf $(OBJS_DIRS)

fclean: clean
		rm -f $(NAME)

re : clean all

.PHONY: all clean fclean re makelib
