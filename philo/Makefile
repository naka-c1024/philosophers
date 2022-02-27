# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ynakashi <ynakashi@student.42tokyo.jp>     +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/02/13 16:07:02 by ynakashi          #+#    #+#              #
#    Updated: 2022/02/27 19:38:33 by ynakashi         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME	:=	philo
CC		:=	cc
RM		:=	rm -rf
CFLAGS	:=	-Wall -Wextra -Werror

DEBUG_FLAGS	:=	-g
NO_BUILTIN_FLAGS	:=	-fno-builtin
SRCS	:=	./srcs/action.c\
			./srcs/check_arg.c\
			./srcs/create_philo.c\
			./srcs/destroy.c\
			./srcs/flag.c\
			./srcs/fork.c\
			./srcs/init_share.c\
			./srcs/main.c\
			./srcs/thread.c\
			./srcs/utils.c

OBJS	:=	$(SRCS:.c=.o)

# suffix rule
.c.o	:
	$(CC) $(CFLAGS) -c $< -o $@

$(NAME)	: $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

all		: $(NAME)

clean	:
	$(RM) $(OBJS)

fclean	: clean
	$(RM) $(NAME)

re		: fclean all

sort	:
	./philo 200 60 60 60 | awk '{print $$1}' | tee output.txt | sort -n > sort.txt && diff -u output.txt sort.txt && rm output.txt sort.txt
	./philo 4 310 200 100 | awk '{print $$1}' | tee output.txt | sort -n > sort.txt && diff -u output.txt sort.txt && rm output.txt sort.txt
	./philo 5 800 200 200 7 | awk '{print $$1}' | tee output.txt | sort -n > sort.txt && diff -u output.txt sort.txt && rm output.txt sort.txt

nm		: fclean $(OBJS)
	$(CC) $(CFLAGS) $(NO_BUILTIN_FLAGS) $(OBJS) -o $(NAME)
	nm -u $(NAME)

debug	: fclean $(OBJS)
	$(CC) $(CFLAGS) $(DEBUG_FLAGS) $(OBJS) -o $(NAME)

.PHONY	: all clean fclean re sort nm debug
