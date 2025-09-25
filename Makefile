# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: miltavar <miltavar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2025/06/12 13:17:19 by miltavar          #+#    #+#              #
#    Updated: 2025/07/09 09:56:42 by miltavar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME		= pipex
CC			= cc
CFLAGS		= -Wall -Wextra -Werror -Ilibft/includes -Iincludes

SRCS		= main.c child.c pipes.c pipex_bonus.c check_arg.c check_path.c
BONUS_SRC	= pipex_bonus.c main.c child.c pipes.c check_arg.c check_path

OBJS		= $(SRCS:.c=.o)
BONUS_OBJ	= $(BONUS_SRC:.c=.o)

LIBFT_PATH	= ./libft
LIBFT		= $(LIBFT_PATH)/libft.a

all: $(NAME)

$(NAME): $(OBJS) $(LIBFT)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

bonus: $(BONUS_OBJ) $(LIBFT)
	$(CC) $(CFLAGS) $(BONUS_OBJ) $(LIBFT) -o $(NAME)

$(LIBFT):
	@$(MAKE) -C $(LIBFT_PATH)

%.o: %.c includes/pipex.h
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -f $(OBJS) $(BONUS_OBJ)
	@$(MAKE) -C $(LIBFT_PATH) clean

fclean: clean
	@rm -f $(NAME)
	@$(MAKE) -C $(LIBFT_PATH) fclean

re: fclean all

.PHONY: all clean fclean re bonus
