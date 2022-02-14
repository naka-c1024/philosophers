/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynakashi <ynakashi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 16:04:14 by ynakashi          #+#    #+#             */
/*   Updated: 2022/02/14 08:53:03 by ynakashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h> // write, usleep
# include <stdlib.h> // malloc, free
# include <string.h> // memset
# include <stdio.h> // printf 使わない方がいい,error,debug用
# include <sys/time.h> // gettimeofday
# include <pthread.h> // pthread
# include <stdbool.h> // bool
# include <limits.h> // macro
# include <stdint.h> // macro(linux)

# define SHOW_FORK "has taken a fork"
# define SHOW_EAT "is eating"
# define SHOW_SLEEP "is sleeping"
# define SHOW_THINK "is thinking"
# define SHOW_DIED "died"

typedef struct s_rules
{
	int	philo_num;
	int	die_time;
	int	eat_time;
	int	sleep_time;
	int	ate_num;
}	t_rules;

typedef enum e_flag
{
	EAT,
	FORK,
	THINK,
	SLEEP,
	ELSE
}	t_flag;

// check_arg.c
void	show_help(void);
size_t	ft_strlen(char *str);
int		ft_isdigit(int c);
bool	is_uint(char *str);
bool	check_arg(int argc, char **argv);

#endif
