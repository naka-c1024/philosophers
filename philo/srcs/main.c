/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynakashi <ynakashi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 15:58:38 by ynakashi          #+#    #+#             */
/*   Updated: 2022/02/23 15:17:42 by ynakashi         ###   ########.fr       */
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

void	destroy_rules(t_rules *rules)
{
	int	i;

	i = 0;
	while (i < rules->philo_num)
	{
		pthread_mutex_destroy(&rules->m_fork[i]);
		i++;
	}
	free(rules->m_fork);
	pthread_mutex_destroy(&rules->meal_check);
	free(rules);
}

t_philo	*first_philo(t_rules *rules)
{
	t_philo	*philo;

	philo = (t_philo *)malloc(sizeof(t_philo));
	if (!philo)
		return (NULL);

	philo->id = 1;
	philo->right_fork_id = 0;
	philo->left_fork_id = 1 % rules->philo_num;
	philo->ate_count = 0;
	philo->t_last_meal = 0;
	philo->limit = 0;
	philo->rules = rules;

	philo->right = philo;
	philo->left = philo;
	return (philo);
}

t_philo	*create_philo(t_rules *rules)
{
	t_philo	*philo;
	t_philo	*new;
	int		i;

	int	num; // jとかに変える,numをiに変えて上のをjにするのもあり

	num = 0;
	while (num < rules->philo_num)
		pthread_mutex_init(&rules->m_fork[num++], NULL);
	pthread_mutex_init(&rules->meal_check, NULL);

	philo = first_philo(rules); // 最初に一人
	if (!philo)
	{
		destroy_rules(rules);
		return (NULL);
	}
	i = 1;
	while (i < rules->philo_num) // 双方好循環リスト作成
	{
		new = (t_philo *)malloc(sizeof(t_philo));
		if (new == NULL)
		{
			destroy_philos(philo, i);
			destroy_rules(rules);
			return (NULL);
		}

		new->id = i + 1;
		new->right_fork_id = i;
		new->left_fork_id = (i + 1) % rules->philo_num;
		new->ate_count = 0;
		new->t_last_meal = 0;
		new->limit = 0;
		new->rules = rules;

		philo->right->left = new; // 右からphilo作成,最初はphiloのrightはphiloだから左から作成
		new->right = philo->right;
		new->left = philo;
		philo->right = new;
		i++;
	}
	return (philo);
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

	ate_count = philo->rules->ate;
	if (ate_count && ate_count == philo->rules->philo_num)
	{
		philo->rules->all_ate = 1;
		return (1);
	}
	return (0);
}

void	show_log(long time, int philo_id, char *type)
{
	printf("%ld %d %s\n", time, philo_id, type);
}

int	check_limit(t_philo *philo)
{
	if (get_time() >= philo->limit)
	{
		philo->rules->die_flg = 1;
		show_log(get_time(), philo->id, SHOW_DIED);
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
	pthread_mutex_unlock(&(philo->rules->m_fork[lfork]));
	pthread_mutex_unlock(&(philo->rules->m_fork[rfork]));
	return (0);
}


void	*monitor(void *void_philo)
{
	t_philo	*philo;

	philo = (t_philo *)void_philo;
	philo->t_last_meal = get_time();
	philo->limit = philo->t_last_meal + philo->rules->die_time;
	while (1)
	{
		pthread_mutex_lock(&philo->rules->meal_check);
		if (philo->rules->die_flg || philo->rules->all_ate)
			break ;
		if (check_all_ate(philo))
			break ;
		if (check_limit(philo))
			break ;
		pthread_mutex_unlock(&philo->rules->meal_check);
		usleep(1000);
	}
	put_forks(philo);
	pthread_mutex_unlock(&philo->rules->meal_check);
	return (NULL);
}

int	ate_dieflg_check(t_philo *philo, int type)
{
	if (philo->rules->all_ate == 1 || philo->rules->die_flg == 1)
	{
		if (type == EAT)
			put_forks(philo);
		pthread_mutex_unlock(&philo->rules->meal_check);
		return (-1);
	}
	return (0);
}

int	get_left_fork(t_philo *philo)
{
	int	lfork;

	lfork = philo->left_fork_id;
	pthread_mutex_lock(&philo->rules->m_fork[lfork]);
	pthread_mutex_lock(&philo->rules->meal_check);
	if (ate_dieflg_check(philo, FORK) == -1)
	{
		pthread_mutex_unlock(&philo->rules->m_fork[lfork]);
		return (-1);
	}
	show_log(get_time(), philo->id, SHOW_FORK);
	pthread_mutex_unlock(&philo->rules->meal_check);
	return (0);
}

int	get_right_fork(t_philo *philo)
{
	int	rfork;

	rfork = philo->right_fork_id;
	pthread_mutex_lock(&philo->rules->m_fork[rfork]);
	pthread_mutex_lock(&philo->rules->meal_check);
	if (ate_dieflg_check(philo, FORK) == -1)
	{
		pthread_mutex_unlock(&philo->rules->m_fork[rfork]);
		return (-1);
	}
	show_log(get_time(), philo->id, SHOW_FORK);
	pthread_mutex_unlock(&philo->rules->meal_check);
	return (0);
}

int	get_forks(t_philo *philo)
{
	if (get_left_fork(philo) == -1)
		return (-1);
	if (get_right_fork(philo) == -1)
	{
		pthread_mutex_unlock(&(philo->rules->m_fork[philo->left_fork_id]));
		return (-1);
	}
	return (0);
}

void	eattime_set(t_philo *philo)
{
	philo->t_last_meal = get_time();
	philo->limit = philo->t_last_meal + philo->rules->die_time;
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
	if (philo->ate_count == philo->rules->ate_num)
		philo->rules->ate += 1;
}

int	eat(t_philo *philo)
{
	pthread_mutex_lock(&philo->rules->meal_check);
	if (ate_dieflg_check(philo, EAT) == -1)
		return (-1);
	eattime_set(philo);
	show_log(philo->t_last_meal, philo->id, SHOW_EAT);
	pthread_mutex_unlock(&philo->rules->meal_check);
	adjustment_sleep(get_time() + philo->rules->eat_time);
	philo->ate_count += 1;
	check_eat_count(philo);
	return (0);
}

int	philo_sleep(t_philo *philo)
{
	pthread_mutex_lock(&philo->rules->meal_check);
	if (ate_dieflg_check(philo, SLEEP) == -1)
		return (-1);
	show_log(get_time(), philo->id, SHOW_SLEEP);
	pthread_mutex_unlock(&philo->rules->meal_check);
	adjustment_sleep(get_time() + philo->rules->sleep_time);
	return (0);
}

int	think(t_philo *philo)
{
	pthread_mutex_lock(&philo->rules->meal_check);
	if (ate_dieflg_check(philo, THINK) == -1)
		return (-1);
	show_log(get_time(), philo->id, SHOW_THINK);
	pthread_mutex_unlock(&philo->rules->meal_check);
	return (0);
}

void	*philo_action(void *void_philo)
{
	pthread_t	tid;
	t_philo		*philo;

	philo = (t_philo *)void_philo;
	if (philo->id % 2 == 0)
		usleep(1000);
	if (pthread_create(&tid, NULL, monitor, void_philo))
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
	pthread_join(tid, NULL);
	return (NULL);
}

bool	main_threads(t_philo *philo)
{
	int	philo_num;
	int	i;

	philo_num = philo->rules->philo_num;
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
	t_rules	*rules;
	t_philo	*philo;

	if (check_arg(argc, argv) == false)
		return (EXIT_SUCCESS);
	rules = init_rules(argc, argv);
	if (!rules)
		return (EXIT_FAILURE);

	philo = create_philo(rules);
	if (!philo)
		return (EXIT_FAILURE);

	if (main_threads(philo) == false)
	{
		destroy_philos(philo, rules->philo_num);
		destroy_rules(rules);
		return (EXIT_FAILURE);
	}

	destroy_philos(philo, rules->philo_num);
	destroy_rules(rules);
	return (EXIT_SUCCESS);
}
