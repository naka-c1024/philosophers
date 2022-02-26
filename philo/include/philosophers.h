/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.h                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynakashi <ynakashi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 16:04:14 by ynakashi          #+#    #+#             */
/*   Updated: 2022/02/26 11:44:54 by ynakashi         ###   ########.fr       */
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

typedef struct s_share
{
	int	philo_num;
	int	die_time;
	int	eat_time;
	int	sleep_time;
	int	ate_num;

	pthread_mutex_t	*m_fork;
	pthread_mutex_t	meal_check; // philoのactの前、もしくは間にずっとflgを見ている, change name flg_check
	int				equal_ate_cnt; // ate_numと同じ回数になった場合加算される
	int				full_stomach_flg; // 回数分食べたかどうか
	int				starving_flg; // 死んだかどうか
}	t_share;

typedef struct s_philo
{
	int				id;
	pthread_t		thread_id; // これをpthread_createの第一引数に&をつけて渡す
	int				left_fork_id;
	int				right_fork_id;
	int				ate_count; // 何回食べたか
	long			exact_time; // これ必要なくね？ get_time()を2回使う関数はこれに入れて時刻の不整合を無くす
	long			die_limit_time; // exact_timeとdie_timeを足し合わせた時刻
	t_share			*share;
	struct s_philo	*left; // 左のphilo, next
	struct s_philo	*right; // 右のphilo, pre
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

// init_share.c
int		ft_atoi(const char *str);
t_share	*init_share(int argc, char **argv);

#endif
