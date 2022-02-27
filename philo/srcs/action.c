/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   action.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynakashi <ynakashi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 18:34:54 by ynakashi          #+#    #+#             */
/*   Updated: 2022/02/27 18:41:04 by ynakashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	eating(t_philo *philo)
{
	long	exact_time;
	long	end_time;

	pthread_mutex_lock(&philo->share->lock_timestamp);
	if (check_flg(philo) == STOP)
		return (put_forks(philo));
	exact_time = get_time();
	philo->die_limit_time = exact_time + philo->share->die_time;
	show_log(exact_time, philo->id, SHOW_EAT);
	philo->ate_count += 1;
	if (philo->ate_count == philo->share->ate_num)
		philo->share->equal_ate_cnt += 1;
	pthread_mutex_unlock(&philo->share->lock_timestamp);
	end_time = exact_time + philo->share->eat_time;
	while (1)
	{
		if (check_flg(philo) == STOP)
			return (put_forks(philo));
		if (get_time() >= end_time)
			break ;
		usleep(1000);
	}
	put_forks(philo);
	return (0);
}

int	sleeping(t_philo *philo)
{
	long	exact_time;
	long	end_time;

	pthread_mutex_lock(&philo->share->lock_timestamp);
	if (check_flg(philo) == STOP)
		return (STOP);
	exact_time = get_time();
	show_log(exact_time, philo->id, SHOW_SLEEP);
	pthread_mutex_unlock(&philo->share->lock_timestamp);
	end_time = exact_time + philo->share->eat_time;
	while (1)
	{
		if (check_flg(philo) == STOP)
			return (STOP);
		if (get_time() >= end_time)
			break ;
		usleep(1000);
	}
	return (0);
}

int	thinking(t_philo *philo)
{
	pthread_mutex_lock(&philo->share->lock_timestamp);
	if (check_flg(philo) == STOP)
		return (STOP);
	show_log(get_time(), philo->id, SHOW_THINK);
	pthread_mutex_unlock(&philo->share->lock_timestamp);
	return (0);
}
