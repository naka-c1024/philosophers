/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynakashi <ynakashi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 16:04:14 by ynakashi          #+#    #+#             */
/*   Updated: 2022/02/22 08:50:51 by ynakashi         ###   ########.fr       */
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

# define RESET "\033[0m"
# define BLACK "\033[30m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN "\033[36m"
# define WHITE "\033[37m"

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
	t_rules				*info;
	pthread_mutex_t		mutex;
	struct s_philo		*left; // 左のphilo, next
	struct s_philo		*right; // 右のphilo, pre
	pthread_t			thread_id;
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
