# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: yelu <yelu@student.42kl.edu.my>            +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/11/09 00:00:25 by yelu              #+#    #+#              #
#    Updated: 2026/03/06 15:22:24 by yelu             ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

# Colors
RED		=	\033[1;31m
GREEN	=	\033[1;32m
YELLOW	=	\033[1;33m
BLUE	=	\033[1;34m
PINK 	=	\033[35m
RESET	=	\033[0m

# Program name
NAME = request

# Directories
SRC_DIR = src

# Compiler & flags
CC = c++
CFLAGS = -Wall -Werror -Wextra -std=c++98
RM = rm -rf

# Source files
SRC = main.cpp \
		request.cpp

# Build rules
all: $(NAME)

$(NAME): $(SRC)
	$(CC) $(CFLAGS) $(SRC) -o $(NAME)

# Clean object files and executable
clean:
	@echo "$(RED)❌ No object files to clean for this project$(RESET)"

fclean:
	@echo "$(RED)❌ Deleting$(RESET)\t$(NAME)"
	@$(RM) $(NAME)
	@echo "$(GREEN)✅ Removed\t$(RESET)$(WHITE)$(NAME)$(RESET)"

re: fclean all

# # debug:
#@echo "SRC: $(SRC)"
#@echo "OBJ: $(OBJ)"
#@echo "OBJ_DIR: $(OBJ_DIR)"

.PHONY: all clean fclean re

# -I(include directory) - Tells the compiler to look for header files 
# -L(directory) - Tells the compiler to look for .a library path in this folder
# -l(%.a) - Tells the compiler to look for that specified .a file
