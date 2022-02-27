/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynakashi <ynakashi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 15:58:38 by ynakashi          #+#    #+#             */
/*   Updated: 2022/02/27 09:43:20 by ynakashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	destroy_philos(t_philo *philos, int count)
{
	t_philo	*tmp;

	while (count--)
	{
		tmp = philos->left; // 左回り
		free(philos);
		philos = tmp;
	}
}

void	destroy_rules(t_share *share)
{
	int	i;

	i = 0;
	while (i < share->philo_num)
	{
		pthread_mutex_destroy(&share->m_fork[i]);
		i++;
	}
	free(share->m_fork);
	pthread_mutex_destroy(&share->lock_share_var);
	free(share);
}

t_philo	*first_philo(t_share *share)
{
	t_philo	*philo;

	philo = (t_philo *)malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);

	philo->id = 1;
	philo->right_fork_id = 0;
	philo->left_fork_id = 1 % share->philo_num;
	philo->ate_count = 0;
	philo->die_limit_time = 0;
	philo->share = share;

	philo->right = philo;
	philo->left = philo;
	return (philo);
}

t_philo	*create_philo(t_share *share)
{
	t_philo	*philo;
	t_philo	*new;
	int		i;

	int	num; // jとかに変える,numをiに変えて上のをjにするのもあり

	num = 0;
	while (num < share->philo_num)
		pthread_mutex_init(&share->m_fork[num++], NULL);
	pthread_mutex_init(&share->lock_share_var, NULL);

	philo = first_philo(share); // 最初に一人
	if (!philo)
	{
		destroy_rules(share);
		return (NULL);
	}
	i = 1;
	while (i < share->philo_num) // 双方好循環リスト作成
	{
		new = (t_philo *)malloc(sizeof(t_philo));
		if (new == NULL)
		{
			destroy_philos(philo, i);
			destroy_rules(share);
			return (NULL);
		}

		new->id = i + 1;
		new->right_fork_id = i;
		new->left_fork_id = (i + 1) % share->philo_num;
		new->ate_count = 0;
		new->die_limit_time = 0;
		new->share = share;

		philo->right->left = new; // 右からphilo作成,最初はphiloのrightはphiloだから左から作成
		new->right = philo->right;
		new->left = philo;
		philo->right = new;
		i++;
	}
	return (philo);
}

// 1s = 1000ms, 1micros = 1/1000ms
long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

void	show_log(long time, int philo_id, char *log_msg)
{
	printf("%ld %d %s\n", time, philo_id, log_msg);
}

int	check_full_stomach(t_philo *philo)
{
	if (philo->share->equal_ate_cnt == philo->share->philo_num) // 全員が回数分食べ終わったかどうか
	{
		philo->share->full_stomach_flg = 1;
		return (1);
	}
	return (0);
}

int	check_starving(t_philo *philo)
{
	long	exact_time;


	exact_time = get_time();
	if (exact_time >= philo->die_limit_time)
	{
		philo->share->starving_flg = 1;
		show_log(exact_time, philo->id, SHOW_DIED);
		return (1);
	}
	return (0);
}

int	put_forks(t_philo *philo)
{
	int	left_fork;
	int	right_fork;

	left_fork = philo->left_fork_id;
	right_fork = philo->right_fork_id;
	pthread_mutex_unlock(&(philo->share->m_fork[left_fork]));
	pthread_mutex_unlock(&(philo->share->m_fork[right_fork]));
	return (0);
}

void	*monitor(void *param)
{
	t_philo	*philo;
	long	exact_time;

	philo = (t_philo *)param;
	exact_time = get_time();
	philo->die_limit_time = exact_time + philo->share->die_time;
	while (1)
	{
		pthread_mutex_lock(&philo->share->lock_share_var);
		if (philo->share->starving_flg || philo->share->full_stomach_flg)
			break ;
		if (check_full_stomach(philo))
			break ;
		if (check_starving(philo))
			break ;
		pthread_mutex_unlock(&philo->share->lock_share_var);
		// usleep(1000);
		usleep(100); // guacamoleだと100で"200 410 200 200"が生き延びた
	}
	put_forks(philo); // これ必要?
	pthread_mutex_unlock(&philo->share->lock_share_var);
	return (NULL);
}

int	check_flg(t_philo *philo, int type)
{
	if (philo->share->full_stomach_flg || philo->share->starving_flg)
	{
		if (type == EAT)
			put_forks(philo);
		pthread_mutex_unlock(&philo->share->lock_share_var);
		return (-1);
	}
	return (0);
}

int	get_left_fork(t_philo *philo)
{
	int	left_fork;

	left_fork = philo->left_fork_id;
	pthread_mutex_lock(&philo->share->m_fork[left_fork]); // ここの順番入れ替えるだけでなぜか死ぬようになる
	pthread_mutex_lock(&philo->share->lock_share_var);
	if (check_flg(philo, FORK) == -1)
	{
		pthread_mutex_unlock(&philo->share->m_fork[left_fork]);
		return (-1);
	}
	show_log(get_time(), philo->id, SHOW_FORK);
	pthread_mutex_unlock(&philo->share->lock_share_var);
	return (0);
}

int	get_right_fork(t_philo *philo)
{
	int	right_fork;

	right_fork = philo->right_fork_id;
	pthread_mutex_lock(&philo->share->m_fork[right_fork]);
	pthread_mutex_lock(&philo->share->lock_share_var);
	if (check_flg(philo, FORK) == -1)
	{
		pthread_mutex_unlock(&philo->share->m_fork[right_fork]);
		return (-1);
	}
	show_log(get_time(), philo->id, SHOW_FORK);
	pthread_mutex_unlock(&philo->share->lock_share_var);
	return (0);
}

int	get_forks(t_philo *philo)
{
	if (get_left_fork(philo) == -1)
		return (-1);
	if (get_right_fork(philo) == -1)
	{
		pthread_mutex_unlock(&(philo->share->m_fork[philo->left_fork_id]));
		return (-1);
	}
	return (0);
}

int	eat(t_philo *philo)
{
	long	exact_time;
	long	end_time;

	pthread_mutex_lock(&philo->share->lock_share_var);
	if (check_flg(philo, EAT) == -1)
		return (-1);

	exact_time = get_time();
	philo->die_limit_time = exact_time + philo->share->die_time;

	show_log(exact_time, philo->id, SHOW_EAT);
	philo->ate_count += 1; // 何回食べたか
	if (philo->ate_count == philo->share->ate_num) // 回数分食べ切ったらateに加算する
		philo->share->equal_ate_cnt += 1;
	pthread_mutex_unlock(&philo->share->lock_share_var);

	end_time = exact_time + philo->share->eat_time;
	while (1)
	{
		pthread_mutex_lock(&philo->share->lock_share_var);
		if (check_flg(philo, EAT) == -1)
			return (-1);
		pthread_mutex_unlock(&philo->share->lock_share_var);

		exact_time = get_time();
		if (exact_time >= end_time)
			break ;
		usleep(1000);
	}
	return (0);
}

int	philo_sleep(t_philo *philo)
{
	long	exact_time;
	long	end_time;

	pthread_mutex_lock(&philo->share->lock_share_var);
	if (check_flg(philo, SLEEP) == -1)
		return (-1);
	exact_time = get_time();
	show_log(exact_time, philo->id, SHOW_SLEEP);
	pthread_mutex_unlock(&philo->share->lock_share_var);

	end_time = exact_time + philo->share->eat_time;
	while (1)
	{
		pthread_mutex_lock(&philo->share->lock_share_var);
		if (check_flg(philo, SLEEP) == -1)
			return (-1);
		pthread_mutex_unlock(&philo->share->lock_share_var);

		exact_time = get_time();
		if (exact_time >= end_time)
			break ;
		usleep(1000);
	}

	return (0);
}

int	think(t_philo *philo)
{
	pthread_mutex_lock(&philo->share->lock_share_var);
	if (check_flg(philo, THINK) == -1)
		return (-1);
	show_log(get_time(), philo->id, SHOW_THINK);
	pthread_mutex_unlock(&philo->share->lock_share_var);
	return (0);
}

void	*philo_action(void *param)
{
	pthread_t	monitor_id;
	t_philo		*philo;

	philo = (t_philo *)param;
	if (philo->id % 2 == 0)
		usleep(1000);
	if (pthread_create(&monitor_id, NULL, monitor, param))
		return (NULL);
	while (1)
	{
		if (get_forks(philo) == -1 \
		|| eat(philo) == -1 \
		|| put_forks(philo) == -1 \
		|| philo_sleep(philo) == -1 \
		|| think(philo) == -1)
			break ;
	}
	pthread_join(monitor_id, NULL);
	return (NULL);
}

bool	main_threads(t_philo *philo)
{
	int	philo_num;
	int	i;

	philo_num = philo->share->philo_num;
	i = 0;
	while (i < philo_num)
	{
		if (pthread_create(&(philo->thread_id), NULL, philo_action, (void *)philo))
			return (false);
		philo = philo->left;
		i++;
	}

	while (philo_num--)
	{
		if (pthread_join(philo->thread_id, NULL))
			return (false);
		philo = philo->left;
	}

	return (true);
}

int main(int argc, char **argv)
{
	t_share	*share;
	t_philo	*philo;

	if (check_arg(argc, argv) == false)
		return (EXIT_SUCCESS);
	share = init_share(argc, argv);
	if (!share)
		return (EXIT_FAILURE);

	philo = create_philo(share);
	if (!philo)
		return (EXIT_FAILURE);

	if (main_threads(philo) == false)
	{
		destroy_philos(philo, share->philo_num);
		destroy_rules(share);
		return (EXIT_FAILURE);
	}

	destroy_philos(philo, share->philo_num);
	destroy_rules(share);
	return (EXIT_SUCCESS);
}
