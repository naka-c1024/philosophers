/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   flag.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynakashi <ynakashi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 18:30:23 by ynakashi          #+#    #+#             */
/*   Updated: 2022/02/27 18:31:01 by ynakashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	check_full_stomach(t_philo *philo)
{
	if (philo->share->equal_ate_cnt == philo->share->philo_num)
	{
		philo->share->full_stomach_flg = 1;
		return (STOP);
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
		return (STOP);
	}
	return (0);
}

int	check_flg(t_philo *philo)
{
	pthread_mutex_lock(&philo->share->lock_share_var);
	if (philo->share->full_stomach_flg || philo->share->starving_flg)
	{
		pthread_mutex_unlock(&philo->share->lock_share_var);
		pthread_mutex_unlock(&philo->share->lock_timestamp);
		return (STOP);
	}
	pthread_mutex_unlock(&philo->share->lock_share_var);
	return (0);
}
