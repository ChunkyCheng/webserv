CXX			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98 -g3 #-fsanitize=address

PARSE_DIR		=	ConfigParsing
PARSE_FILES		=	ConfigParser.cpp		ADirective.cpp		ADirectiveBlock.cpp		\
					ServerDirective.cpp		\
					Config.cpp	DirectiveCreator.cpp

SERVER_DIR		=	Server
SERVER_FILES	=	WebServer.cpp		Epoll.cpp			\
					Server.cpp			ServerSocket.cpp	\
					Client.cpp			ClientSocket.cpp	\
					RequestHandler.cpp

MODULES		=	PARSE	SERVER

$(foreach M,$(MODULES), $(eval $(M)_SRCS = $(addprefix $($(M)_DIR)/, $($(M)_FILES))))

SRC_DIR		=	SRCS
SRC_FILES	=	main.cpp
SRCS		=	$(addprefix $(SRC_DIR)/, $(SRC_FILES) $(foreach M,$(MODULES), $($(M)_SRCS)))

OBJ_DIR	=	OBJS
OBJS	=	$(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

IFLAGS = $(addprefix -I$(SRC_DIR)/, $(foreach M,$(MODULES), $($(M)_DIR))) -IHEADERS

##########################################################################################

RESET	=	\033[0m
BOLD	=	\033[1m
YELLOW	=	\033[33m
RED		=	\033[31m

##########################################################################################

NAME	=	webserv

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@printf "$(BOLD)$(YELLOW)$(NAME) successfully compiled\n$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(IFLAGS) -c $< -o $@

clean:
	@printf "$(RED)"
	@if [ -d $(OBJ_DIR) ]; then						\
		rm -rf $(OBJ_DIR);							\
		printf "Removed $(NAME) OBJS directory\n";	\
	else											\
		printf "$(NAME) objects not found\n"; fi;
	@printf "$(RESET)"

fclean: clean
	@printf "$(RED)"
	@if [ -f $(NAME) ]; then						\
		rm -f $(NAME);								\
		printf "Removed $(NAME)\n"					\
	else											\
		printf "$(NAME) not found\n"; fi;
	@printf "$(RESET)"

re: fclean
	@make -s all

.PHONY: all clean fclean re
