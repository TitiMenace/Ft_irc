
NAME 		= test

CXX			= clang++
CXXFLAGS	= -g3 -Wall -Wextra -Werror -Wno-unused-parameter #-std=c++98
CPPFLAGS	= -I../includes -Iincludes -Ilibraries/criterion-2.4.2/include -MP -MMD
LDFLAGS		= -Wl,-rpath=libraries/criterion-2.4.2/lib -Llibraries/criterion-2.4.2/lib -lcriterion

FILES		= \
				test \
				Client \
				ServerProcess \
				commands/join \
				commands/mode/mode \
				commands/mode/operator \
				commands/nick \
				commands/pass \
				commands/user \
				commands/ping \
				commands/privmsg \
				commands/invite \
				commands/topic \
				commands/kick

SRCS_DIR	= sources
SRCS		= $(addprefix $(SRCS_DIR)/, $(addsuffix .cpp, $(FILES)))

OBJS_DIR	= .build
OBJS		= $(patsubst $(SRCS_DIR)/%.cpp, $(OBJS_DIR)/%.o, $(SRCS))
DEPS		= $(OBJS:.o=.d)

all:	$(NAME)

$(NAME) : $(OBJS)
		$(CXX) $(LDFLAGS) $(OBJS) -o $@

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
