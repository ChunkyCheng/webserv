CXX			=	c++
CXXFLAGS	=	-Wall -Wextra -Werror -std=c++98 -g3 #-fsanitize=address

PARSE_DIR		=	ConfigParsing
PARSE_FILES		=	ConfigParser.cpp		ADirective.cpp			ADirectiveBlock.cpp	\
					Config.cpp				DirectiveCreator.cpp	ConfigExcept.cpp

DIRECTIVE_DIR	=	$(PARSE_DIR)/directives
DIRECTIVE_FILES	=	ServerDirective.cpp				LocationDirective.cpp		\
					ErrorPageDirective.cpp			ListenDirective.cpp			\
					MethodDirective.cpp				RootDirective.cpp			\
					IndexDirective.cpp				AutoindexDirective.cpp		\
					ClientMaxDirective.cpp			ReturnDirective.cpp

SERVER_DIR		=	Server
SERVER_FILES	=	WebServer.cpp		Epoll.cpp			\
					Server.cpp			ServerSocket.cpp	\
					Client.cpp			ClientSocket.cpp	\
					Location.cpp							\
					RequestHandler.cpp

MODULES		=	PARSE	DIRECTIVE	SERVER

$(foreach M,$(MODULES), $(eval $(M)_SRCS = $(addprefix $($(M)_DIR)/, $($(M)_FILES))))

SRC_DIR		=	SRCS
SRC_FILES	=	main.cpp
SRCS		=	$(addprefix $(SRC_DIR)/, $(SRC_FILES) $(foreach M,$(MODULES), $($(M)_SRCS)))

OBJ_DIR	=	OBJS
OBJS	=	$(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

IFLAGS = $(addprefix -I$(SRC_DIR)/, $(foreach M,$(MODULES), $($(M)_DIR))) -IHEADERS

##########################################################################################


NEEDS_REBUILD_CMD   :=                                                              \
    for src in $(SRCS); do                                                          \
        obj="$$(echo "$$src" | sed 's|^$(SRC_DIR)|$(OBJ_DIR)|')";                   \
        obj="$${obj%.cpp}.o";                                                         \
        if [ ! -f "$$obj" ] || [ "$$src" -nt "$$obj" ]; then echo "$$src"; fi;      \
    done
NEEDS_REBUILD   :=  $(shell $(NEEDS_REBUILD_CMD))
SRCTOTAL        :=  $(words $(NEEDS_REBUILD))
SRCNUM          :=  0

RESET       =   \033[0m
BOLD        =   \033[1m
RED         =   \033[31m
GREEN       =   \033[32m
YELLOW      =   \033[33m
ORANGE      =   \033[38;5;208m
YLLWGRN     =   \033[38;2;150;255;25m
PREVLINE    =   \033[A
RMLINE      =   \r\033[K


##########################################################################################

NAME	=	webserv

all: $(NAME)

$(NAME): $(OBJS)
	@$(CXX) $(CXXFLAGS) $(OBJS) -o $(NAME)
	@printf "\n$(BOLD)$(YELLOW)$(NAME) successfully compiled\n$(RESET)"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(eval SRCNUM := $(shell expr $(SRCNUM) + 1))
	@$(eval CALCPERCENT := $(shell echo "scale=4; $(SRCNUM) / $(SRCTOTAL) * 100" | bc))
	@$(eval PERCENT := $(shell printf "%.0f" $(CALCPERCENT)))
	@if [ $(SRCNUM) -gt 1 ]; then printf "$(PREVLINE)\r"; fi;
	@printf "$(BOLD)Compiling $(NAME): ["
	@$(eval PROGRESS := $(shell expr $(PERCENT) / 5))
	@if [ $(PERCENT) -ge 5 ]; then printf "$(GREEN)%0.s#" $(shell seq 1 $(PROGRESS)); fi;
	@$(eval CALCPADS := $(shell expr 20 - $(PROGRESS)))
	@if [ $(PERCENT) -lt 100 ]; then printf "$(RESET)$(BOLD)%0.s-" $(shell seq 1 $(CALCPADS)); fi;
	@printf "$(RESET)$(BOLD)] "
	@if [ $(PERCENT) -lt 25  ]; then    \
		printf "$(RED)";                \
	elif [ $(PERCENT) -lt 50 ]; then    \
		printf "$(ORANGE)";             \
	elif [ $(PERCENT) -lt 75 ]; then    \
		printf "$(BOLD)$(YELLOW)";      \
	elif [ $(PERCENT) -lt 100 ]; then   \
		printf "$(BOLD)$(YLLWGRN)";     \
	else                                \
		printf "$(BOLD)$(GREEN)"; fi;
	@printf "%d%% $(RESET)\n" $(PERCENT)
	@printf "Compiling %-34s" $(notdir $@)
	@$(CXX) $(CXXFLAGS) $(IFLAGS) $(DFLAGS) -c $< -o $@

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
		printf "Removed $(NAME)\n";					\
	else											\
		printf "$(NAME) not found\n"; fi;
	@printf "$(RESET)"

re: fclean
	@make -s all

.PHONY: all clean fclean re
