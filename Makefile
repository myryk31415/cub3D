# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: padam <padam@student.42heilbronn.com>      +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2023/10/09 15:34:56 by padam             #+#    #+#              #
#    Updated: 2024/05/16 17:15:48by padam            ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME = cub3D

CC = cc
CFLAGS =  $(INCLUDES) -g3
LIBFT_DIR = libft
LIBMLX_DIR = ./MLX42
INCLUDES = -I./includes -I $(LIBMLX_DIR)/include/MLX42/

ifeq ($(shell uname), Linux)
#Linux
CFLAGS += -D LINUX -lm
LIBMLX = $(LIBMLX_DIR)/build/libmlx42.a -lXext -lX11 -lm -lbsd -lglfw -lpthread
else
#MacOS
LIBMLX = $(LIBMLX_DIR)/build/libmlx42.a -ldl -lglfw3 -pthread -framework Cocoa -framework OpenGL -framework IOKit
endif

SRC_PATH = src
OBJ_PATH = obj

OBJ_DIRS =	parsing	raycasting main

SRCS_MAIN =	main.c cleanup.c
SRCS_PARSING = parse_file.c parse_utils.c
SRCS_RAY = raycast.c

SRC_NAME =	$(addprefix main/,						$(SRCS_MAIN))		\
			$(addprefix parsing/,					$(SRCS_PARSING))	\
			$(addprefix raycasting/,				$(SRCS_RAY))

RED = \033[1;31m
GREEN = \033[1;32m
BLUE = \033[1;34m
YELLOW = \033[1;33m
RESET = \033[0m

OBJ_NAME = $(SRC_NAME:.c=.o)
OBJS = $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))

all: $(NAME)

$(NAME): $(LIBFT_DIR)/libft.a $(OBJS)
	@$(CC) -o $(NAME) $(CFLAGS) $(OBJS) $(LIBMLX) $(LIBFT_DIR)/libft.a
	@printf "%-100s\n" "$(NAME) compiled"

$(LIBFT_DIR)/libft.a:
	@make -C $(LIBFT_DIR)

$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)
	@mkdir -p $(addprefix $(OBJ_PATH)/,$(OBJ_DIRS))

$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c | $(OBJ_PATH)
	@printf "%-100s\r" "$(CC) $(CFLAGS) -o $@"
	@$(CC) $(CFLAGS) -o $@ -c $<

mlx:
	cd $(LIBMLX_DIR) && cmake -B build && cmake --build build -j4

init:
	git submodule update --init --recursive

update:
	git submodule update --recursive --remote

clean:
	@rm -rf $(OBJ_PATH)
	@make -C $(LIBFT_DIR) clean

fclean: clean
	@rm -f $(NAME)
	@make -C $(LIBFT_DIR) fclean
	@rm -rf $(LIBMLX_DIR)/build

re: fclean mlx all

ascii_art:
	@clear
	@printf "$(GREEN)	                                            \n\
	                                                                \n\
                                        .--,-\`\`-.                 \n\
  ,----..                              /   /     '.               \n\
 /   /   \                   ,---,    / ../        ;        ,---, \n\
|   :     :          ,--,  ,---.'|    \ \`\`\  .\`-    '     ,---.'| \n\
.   |  ;. /        ,'_ /|  |   | :     \___\/   \   :     |   | : \n\
.   ; /--\`    .--. |  | :  :   : :          \   :   |     |   | | \n\
;   | ;     ,'_ /| :  . |  :     |,-.       /  /   /    ,--.__| | \n\
|   : |     |  ' | |  . .  |   : '  |       \  \   \   /   ,'   | \n\
.   | '___  |  | ' |  | |  |   |  / :   ___ /   :   | .   '  /  | \n\
'   ; : .'| :  | : ;  ; |  '   : |: |  /   /\   /   : '   ; |:  | \n\
'   | '/  : '  :  \`--'   \ |   | '/ : / ,,/  ',-    . |   | '/  ' \n\
|   :    /  :  ,      .-./ |   :    | \ ''\        ;  |   :    :| \n\
 \   \ .'    \`--\`----'     /    \  /   \   \     .'    \   \  /   \n\
  \`---\`                    \`-'----'     \`--\`-,,-'       \`----'    \n\
                                            $(BLUE)by: padam && aweizman$(RESET) \n\
                                                                  \n"

.PHONY: all $(NAME) $(LIBFT_DIR)/libft.a $(LIBMLX) $(OBJ_PATH) $(OBJ_PATH) clean fclean re
