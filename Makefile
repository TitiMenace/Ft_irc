
NAME 		= ircserv

CXX			= clang++
CXXFLAGS	= -g3 -Wall -Wextra -Werror -std=c++98
CPPFLAGS	= -Iincludes -MP -MMD

FILES		= main Server Message

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
		@mkdir -p $(OBJS_DIR)
		$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

clean:
		rm -rf $(OBJS_DIR)

fclean: clean
		rm -f $(NAME)

re : clean all

.PHONY: all clean fclean re makelib
