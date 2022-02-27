/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynakashi <ynakashi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 16:04:14 by ynakashi          #+#    #+#             */
/*   Updated: 2022/02/27 18:36:04 by ynakashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILOSOPHERS_H
# define PHILOSOPHERS_H

# include <unistd.h> // write, usleep
# include <stdlib.h> // malloc, free
# include <string.h> // memset
# include <stdio.h> // printf
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

typedef enum s_num
{
	STOP = -1
}	t_num;

typedef struct s_share
{
	int	philo_num;
	int	die_time;
	int	eat_time;
	int	sleep_time;
	int	ate_num;

	pthread_mutex_t	*m_fork;
	pthread_mutex_t	lock_share_var;
	pthread_mutex_t	lock_timestamp;
	int				equal_ate_cnt;
	int				full_stomach_flg;
	int				starving_flg;
}	t_share;

typedef struct s_philo
{
	int				id;
	pthread_t		thread_id;
	int				left_fork_id;
	int				right_fork_id;
	int				ate_count;
	long			die_limit_time;
	t_share			*share;
	struct s_philo	*left;
	struct s_philo	*right;
}	t_philo;

// check_arg.c
void	show_help(void);
size_t	ft_strlen(char *str);
int		ft_isdigit(int c);
bool	is_uint(char *str);
bool	check_arg(int argc, char **argv);

// init_share.c
int		ft_atoi(const char *str);
t_share	*init_share(int argc, char **argv);

// create_philo.c
t_philo	*first_philo(t_share *share);
t_philo	*create_philo(t_share *share);

// destroy.c
void	destroy_philos(t_philo *philos, int count);
void	destroy_share(t_share *share);

// utils.c
long	get_time(void);
void	show_log(long time, int philo_id, char *log_msg);

// fork.c
int	put_forks(t_philo *philo);
int	get_left_fork(t_philo *philo);
int	get_right_fork(t_philo *philo);
int	taken_a_fork(t_philo *philo);

// flag.c
int	check_full_stomach(t_philo *philo);
int	check_starving(t_philo *philo);
int	check_flg(t_philo *philo);

// thread.c
void	*monitor(void *param);
void	*philo_action(void *param);
bool	main_threads(t_philo *philo);

// action.c
int	eating(t_philo *philo);
int	sleeping(t_philo *philo);
int	thinking(t_philo *philo);

#endif
