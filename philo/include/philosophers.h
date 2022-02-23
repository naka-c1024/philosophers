/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynakashi <ynakashi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 16:04:14 by ynakashi          #+#    #+#             */
/*   Updated: 2022/02/23 12:00:44 by ynakashi         ###   ########.fr       */
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

# define COLOR_R	"\033[41m"
# define COLOR_OFF	"\033[m"

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

	long long			first_timestamp;
	pthread_mutex_t		*m_fork;
	pthread_mutex_t		meal_check;
	pthread_mutex_t		mutex;
	int					die_flg;
	int					ate;
	int					all_ate;
}	t_rules;

typedef struct s_philo
{
	int					id;
	int					left_fork_id;
	int					right_fork_id;
	int					ate_count;
	long long			t_last_meal;
	long long			limit;
	t_rules				*rules;
	pthread_mutex_t		mutex;
	struct s_philo		*left; // 左のphilo, next
	struct s_philo		*right; // 右のphilo, pre
	pthread_t			thread_id; // これをpthread_createの第一引数に&をつけて渡す
}			t_philo;

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

// init_rules.c
int		ft_atoi(const char *str);
t_rules	*init_rules(int argc, char **argv);

#endif
