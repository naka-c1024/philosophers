/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   thread.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynakashi <ynakashi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 18:32:26 by ynakashi          #+#    #+#             */
/*   Updated: 2022/02/27 18:33:24 by ynakashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

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
		if (check_full_stomach(philo) || check_starving(philo))
			break ;
		pthread_mutex_unlock(&philo->share->lock_share_var);
		usleep(100);
	}
	put_forks(philo);
	pthread_mutex_unlock(&philo->share->lock_share_var);
	return (NULL);
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
		if (taken_a_fork(philo) == STOP ||
			eating(philo) == STOP ||
			sleeping(philo) == STOP ||
			thinking(philo) == STOP)
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
