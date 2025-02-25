
NAME 		= ircserv

CXX		= clang++

CXXFLAGS	= -g3 -Wall -Wextra -Werror -std=c++98

CPPFLAGS	= -MP -MMD

FILES		= main Server

SRCS		= $(addsuffix .cpp, $(FILES))

OBJS_DIRS	= .build

OBJS		= 	$(patsubst %.cpp, $(OBJS_DIRS)/%.o, $(SRCS))

DEPS		= 	$(OBJS:.o=.d)

all:	$(NAME)

$(NAME) : $(OBJS)
		$(CXX) $(OBJS) -o $@

-include $(DEPS)

$(OBJS_DIRS)/%.o: %.cpp
		@mkdir -p $(OBJS_DIRS)
		$(CXX) $(CXXFLAGS) $(CPPFLAGS) -o $@ -c $<

clean:
		rm -rf $(OBJS_DIRS)

fclean: clean
		rm -f $(NAME)

re : clean all

.PHONY: all clean fclean re makelib
