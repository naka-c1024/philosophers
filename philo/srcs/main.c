/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynakashi <ynakashi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 15:58:38 by ynakashi          #+#    #+#             */
/*   Updated: 2022/02/22 10:28:50 by ynakashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

t_philo	*free_philo(t_philo *philos, int count)
{
	t_philo	*tmp;

	while (count--)
	{
		tmp = philos->left; // 左回り
		free(philos);
		philos = tmp;
	}
	return (NULL);
}

t_philo	*init_philo(void)
{
	t_philo	*philo;

	philo = (t_philo *)malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);
	philo->right = philo;
	philo->left = philo;
	return (philo);
}

t_philo	*create_struct_philo(int philo_num)
{
	t_philo	*philo;
	t_philo	*new;
	t_philo	*first;
	int		i;

	philo = init_philo(); // 最初に一人
	if (!philo)
		return (NULL);
	first = philo;
	philo_num--;
	i = 0;
	while (i < philo_num) // 双方好循環リスト作成
	{
		new = (t_philo *)malloc(sizeof(t_philo));
		if (new == NULL)
			return (free_philo(first, i + 1));
		philo->right->left = new; // 右からphilo作成,最初はphiloのrightはphiloだから左から作成
		new->right = philo->right;
		new->left = philo;
		philo->right = new;
		i++;
	}
	return (philo);
}

void	clear_rules(t_rules *rules)
{
	int	i;

	if (rules->m_fork)
	{
		i = 0;
		while (i < rules->philo_num)
		{
			pthread_mutex_destroy(&rules->m_fork[i++]);
		}
		free(rules->m_fork);
	}
	pthread_mutex_destroy(&rules->meal_check);
	pthread_mutex_destroy(&rules->mutex);
	free(rules);
}

void	clear_philos(t_philo *philos)
{
	t_philo		*tmp;
	int			i;
	int			philo_num;

	if (philos == NULL)
		return ;
	i = 0;
	philo_num = philos->info->philo_num;
	while (i++ < philo_num)
	{
		tmp = philos->left;
		pthread_mutex_destroy(&(philos->mutex));
		free(philos);
		philos = tmp;
	}
}

int	clear_philos_rules(t_philo *philo, t_rules *rules, int status)
{
	if (philo == NULL)
	{
		free(rules->m_fork);
		free(rules);
	}
	else
	{
		clear_philos(philo);
		clear_rules(rules);
	}
	return (status);
}

int	init_mutex(t_rules *rules)
{
	int	num;

	num = 0;
	while (num < rules->philo_num)
		pthread_mutex_init(&rules->m_fork[num++], NULL);
	pthread_mutex_init(&rules->meal_check, NULL);
	pthread_mutex_init(&rules->mutex, NULL);
	return (0);
}

void	create_philo(t_philo *philo, t_rules *rules)
{
	int	i;

	i = 0;
	init_mutex(rules);
	while (i < rules->philo_num)
	{
		philo->id = i + 1;
		philo->right_fork_id = i;
		philo->left_fork_id = (i + 1) % rules->philo_num;
		philo->ate_count = 0;
		philo->t_last_meal = 0;
		philo->limit = 0;
		philo->info = rules;
		pthread_mutex_init(&(philo->mutex), NULL);
		i++;
		philo = philo->left;
	}
}

long long	get_time(void)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}

int	check_all_ate(t_philo *philo)
{
	int		ate_count;

	ate_count = philo->info->ate;
	if (ate_count && ate_count == philo->info->philo_num)
	{
		philo->info->all_ate = 1;
		return (1);
	}
	return (0);
}

void	put_message(long time, int philo_id, char *type)
{
	printf("%ld %d %s\n", time, philo_id, type);
}

int	check_limit(t_philo *philo)
{
	if (get_time() >= philo->limit)
	{
		philo->info->die_flg = 1;
		put_message(get_time(), philo->id, SHOW_DIED);
		return (1);
	}
	return (0);
}

int	put_forks(t_philo *philo)
{
	int	lfork;
	int	rfork;

	lfork = philo->left_fork_id;
	rfork = philo->right_fork_id;
	pthread_mutex_unlock(&(philo->info->m_fork[lfork]));
	pthread_mutex_unlock(&(philo->info->m_fork[rfork]));
	return (0);
}


void	*monitor(void *void_philo)
{
	t_philo	*philo;

	philo = (t_philo *)void_philo;
	philo->t_last_meal = get_time();
	philo->limit = philo->t_last_meal + philo->info->die_time;
	while (1)
	{
		pthread_mutex_lock(&philo->info->meal_check);
		if (philo->info->die_flg || philo->info->all_ate)
			break ;
		if (check_all_ate(philo))
			break ;
		if (check_limit(philo))
			break ;
		pthread_mutex_unlock(&philo->info->meal_check);
		usleep(1000);
	}
	put_forks(philo);
	pthread_mutex_unlock(&philo->info->meal_check);
	return (NULL);
}

int	ate_dieflg_check(t_philo *philo, int type)
{
	if (philo->info->all_ate == 1 || philo->info->die_flg == 1)
	{
		if (type == EAT)
			put_forks(philo);
		pthread_mutex_unlock(&philo->info->meal_check);
		return (-1);
	}
	return (0);
}

int	get_left_fork(t_philo *philo)
{
	int	lfork;

	lfork = philo->left_fork_id;
	pthread_mutex_lock(&philo->info->m_fork[lfork]);
	pthread_mutex_lock(&philo->info->meal_check);
	if (ate_dieflg_check(philo, FORK) == -1)
	{
		pthread_mutex_unlock(&philo->info->m_fork[lfork]);
		return (-1);
	}
	put_message(get_time(), philo->id, SHOW_FORK);
	pthread_mutex_unlock(&philo->info->meal_check);
	return (0);
}

int	get_right_fork(t_philo *philo)
{
	int	rfork;

	rfork = philo->right_fork_id;
	pthread_mutex_lock(&philo->info->m_fork[rfork]);
	pthread_mutex_lock(&philo->info->meal_check);
	if (ate_dieflg_check(philo, FORK) == -1)
	{
		pthread_mutex_unlock(&philo->info->m_fork[rfork]);
		return (-1);
	}
	put_message(get_time(), philo->id, SHOW_FORK);
	pthread_mutex_unlock(&philo->info->meal_check);
	return (0);
}

int	get_forks(t_philo *philo)
{
	if (get_left_fork(philo) == -1)
		return (-1);
	if (get_right_fork(philo) == -1)
	{
		pthread_mutex_unlock(&(philo->info->m_fork[philo->left_fork_id]));
		return (-1);
	}
	return (0);
}

void	eattime_set(t_philo *philo)
{
	philo->t_last_meal = get_time();
	philo->limit = philo->t_last_meal + philo->info->die_time;
}

void	adjustment_sleep(long long end)
{
	long long	now;

	while (1)
	{
		now = get_time();
		if (now >= end)
			break ;
		usleep(1000);
	}
}

void	check_eat_count(t_philo *philo)
{
	if (philo->ate_count == philo->info->ate_num)
		philo->info->ate += 1;
}

int	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->meal_check);
	if (ate_dieflg_check(philo, EAT) == -1)
		return (-1);
	eattime_set(philo);
	put_message(philo->t_last_meal, philo->id, SHOW_EAT);
	pthread_mutex_unlock(&philo->info->meal_check);
	adjustment_sleep(get_time() + philo->info->eat_time);
	philo->ate_count += 1;
	check_eat_count(philo);
	return (0);
}

int	philo_sleep(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->meal_check);
	if (ate_dieflg_check(philo, SLEEP) == -1)
		return (-1);
	put_message(get_time(), philo->id, SHOW_SLEEP);
	pthread_mutex_unlock(&philo->info->meal_check);
	adjustment_sleep(get_time() + philo->info->sleep_time);
	return (0);
}

int	think(t_philo *philo)
{
	pthread_mutex_lock(&philo->info->meal_check);
	if (ate_dieflg_check(philo, THINK) == -1)
		return (-1);
	put_message(get_time(), philo->id, SHOW_THINK);
	pthread_mutex_unlock(&philo->info->meal_check);
	return (0);
}

void	*philosopher(void *void_philo) // change name -> philo_act
{
	pthread_t	tid;
	t_philo		*philo;

	philo = (t_philo *)void_philo;
	if (philo->id % 2 == 0)
		usleep(1000);
	if (pthread_create(&tid, NULL, monitor, void_philo) != 0)
		return (NULL);
	while (1) // this
	{
		if (get_forks(philo) == -1 \
		|| eat(philo) == -1 \
		|| put_forks(philo) == -1 \
		|| philo_sleep(philo) == -1 \
		|| think(philo) == -1)
			break ;
	}
	pthread_join(tid, NULL);
	return (NULL);
}

int	create_threads(t_philo *philo)
{
	int	i;
	int	philo_num;

	philo_num = philo->info->philo_num;
	i = 0;
	while (i < philo_num)
	{
		if (pthread_create(&(philo->thread_id), NULL, philosopher, (void *)philo) != 0)
			return (-1);
		philo = philo->left;
		i++;
	}
	return (0);
}

int	wait_end_threads(t_philo *philo)
{
	int	i;
	int	philo_num;

	philo_num = philo->info->philo_num;
	i = 0;
	while (i < philo_num)
	{
		if (pthread_join(philo->thread_id, NULL) != 0)
			return (-1);
		philo = philo->left;
		i++;
	}
	return (0);
}

int main(int argc, char **argv)
{
	t_rules	*rules;
	t_philo	*philo;

	if (check_arg(argc, argv) == false)
		return (EXIT_SUCCESS);
	rules = init_rules(argc, argv);
	if (!rules)
		return (EXIT_FAILURE);

	philo = create_struct_philo(rules->philo_num);
	if (philo == NULL)
		return (clear_philos_rules(philo, rules, -1));
	create_philo(philo, rules);
	if (create_threads(philo) == -1)
		return (clear_philos_rules(philo, rules, -1));
	if (wait_end_threads(philo) == -1)
		return (clear_philos_rules(philo, rules, -1));
	clear_philos_rules(philo, rules, 0);

	// write(1, "all done\n", 9); // debug
	return (EXIT_SUCCESS);
}
