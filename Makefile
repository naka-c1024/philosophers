# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ynakashi <ynakashi@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/13 16:07:02 by ynakashi          #+#    #+#              #
#    Updated: 2022/02/13 16:18:35 by ynakashi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	=	philo

CC		=	cc

RM		=	rm -rf

DEBUG_FLAGS	=	-g

SRCS	=	./srcs/*.c

$(NAME)	:
	$(CC) $(SRCS) -o $(NAME)

fclean	:
	$(RM) $(NAME)

re		: fclean $(NAME)

# sh
run		:
	echo "hello world"

debug	: fclean
	$(CC) $(DEBUG_FLAGS) $(SRCS) -o $(NAME)
