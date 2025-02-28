
NAME 		= ircserv

CXX			= clang++
CXXFLAGS	= -g3 -Wall -Wextra -Werror -std=c++98
CPPFLAGS	= -Iincludes -MP -MMD

FILES		= main \
			parsingUtils \
			Server \
			Message \
			commands/PASS \
			commands/NICK \
			commands/USER \
			commands/PING_PONG \
			commands/PRIVMSG \
			commands/JOIN \

SRCS_DIR	= sources
SRCS		= $(addprefix $(SRCS_DIR)/, $(addsuffix .cpp, $(FILES)))

OBJS_DIR	= .build
OBJS		= $(patsubst $(SRCS_DIR)/%.cpp, $(OBJS_DIR)/%.o, $(SRCS))
DEPS		= $(OBJS:.o=.d)

all:	$(NAME)

$(NAME) : $(OBJS)
		$(CXX) $(OBJS) -o $@

-include $(DEPS)

$(OBJS_DIR)/%.o: $(SRCS_DIR)/%.cpp
		@mkdir -p $(@D)
		$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

clean:
		rm -rf $(OBJS_DIR)

fclean: clean
		rm -f $(NAME)

re : clean all

.PHONY: all clean fclean re makelib

echo:
		echo $(OBJS)
