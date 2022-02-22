# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ynakashi <ynakashi@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/13 16:07:02 by ynakashi          #+#    #+#              #
#    Updated: 2022/02/22 11:08:39 by ynakashi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

SHELL := /bin/bash

NAME	:=	philo

CC		:=	cc

RM		:=	rm -rf

DEBUG_FLAGS	:=	-g

SRCS	:=	./srcs/*.c

$(NAME)	:
	$(CC) $(SRCS) -o $(NAME)

fclean	:
	$(RM) $(NAME)

re		: fclean $(NAME)

# sh
sort	:
	./philo 200 60 60 60 | awk '{print $1}' | tee output.txt | sort -n > sort.txt && diff -u output.txt sort.txt && rm output.txt sort.txt

# kati	: re run

debug	: fclean
	$(CC) $(DEBUG_FLAGS) $(SRCS) -o $(NAME)
