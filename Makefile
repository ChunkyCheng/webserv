# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: akok <akok@student.42.fr>                  +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/01 17:46:57 by akok              #+#    #+#              #
#    Updated: 2026/03/20 14:58:44 by akok             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = webserv

COMPILER = c++
FLAGS = -Wall -Wextra -Werror -O2 -std=c++98
INCLUDES = -I. -IhttpProtocol -ICGIHandler -Iserver

HTTP_SRCS = httpProtocol/request.cpp

CGI_SRCS = CGIHandler/CGIHandler.cpp

SRCS =	main.cpp	\
		$(HTTP_SRCS)	\
		$(CGI_SRCS)

OBJS = $(SRCS:%.cpp=$(OBJDIR)/%.o)

OBJDIR = OBJS

GREEN = \033[0;32m
YELLOW = \033[0;33m
RESET = \033[0m

all: $(NAME)

$(NAME): $(OBJS)
	@$(COMPILER) $(FLAGS) $(INCLUDES) $^ -o $@
	@echo "$(GREEN)$(NAME) built.$(RESET)"

$(OBJDIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(COMPILER) $(FLAGS) $(INCLUDES) -c $< -o $@

valgrind: $(NAME)
	valgrind --leak-check=yes --show-leak-kinds=all ./$(NAME)

clean:
	@rm -rf $(OBJDIR)
	@echo "$(YELLOW)object files removed.$(RESET)"

fclean: clean
	@rm -rf $(NAME)
	@echo "$(YELLOW)$(NAME) removed.$(RESET)"

re: fclean all

.PHONY: all clean fclean re
