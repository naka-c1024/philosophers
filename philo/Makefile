# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ynakashi <ynakashi@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/13 16:07:02 by ynakashi          #+#    #+#              #
#    Updated: 2022/02/23 15:09:35 by ynakashi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

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

# 後で消す
test	:
	@echo "1 800 200 200 -> should die"
	@echo "5 800 200 200 -> no one  should die"
	@echo "5 800 200 200 7 -> no one should die and stop when eaten 7 times"
	@echo "4 410 200 200 -> no one should die"
	@echo "4 310 200 100 -> should die"

sort	:
	./philo 200 60 60 60 | awk '{print $$1}' | tee output.txt | sort -n > sort.txt && diff -u output.txt sort.txt && rm output.txt sort.txt
	./philo 4 310 200 100 | awk '{print $$1}' | tee output.txt | sort -n > sort.txt && diff -u output.txt sort.txt && rm output.txt sort.txt
	./philo 5 800 200 200 7 | awk '{print $$1}' | tee output.txt | sort -n > sort.txt && diff -u output.txt sort.txt && rm output.txt sort.txt

# kati	: re run

debug	: fclean
	$(CC) $(DEBUG_FLAGS) $(SRCS) -o $(NAME)
