/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   fork.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynakashi <ynakashi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 18:27:25 by ynakashi          #+#    #+#             */
/*   Updated: 2022/02/27 18:29:03 by ynakashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	put_forks(t_philo *philo)
{
	pthread_mutex_unlock(&(philo->share->m_fork[philo->left_fork_id]));
	pthread_mutex_unlock(&(philo->share->m_fork[philo->right_fork_id]));
	return (STOP);
}

int	get_left_fork(t_philo *philo)
{
	int	left_fork;

	left_fork = philo->left_fork_id;
	pthread_mutex_lock(&philo->share->m_fork[left_fork]);
	pthread_mutex_lock(&philo->share->lock_timestamp);
	if (check_flg(philo) == STOP)
	{
		pthread_mutex_unlock(&philo->share->m_fork[left_fork]);
		return (STOP);
	}
	show_log(get_time(), philo->id, SHOW_FORK);
	pthread_mutex_unlock(&philo->share->lock_timestamp);
	return (0);
}

int	get_right_fork(t_philo *philo)
{
	int	right_fork;

	right_fork = philo->right_fork_id;
	pthread_mutex_lock(&philo->share->m_fork[right_fork]);
	pthread_mutex_lock(&philo->share->lock_timestamp);
	if (check_flg(philo) == STOP)
	{
		pthread_mutex_unlock(&philo->share->m_fork[right_fork]);
		return (STOP);
	}
	show_log(get_time(), philo->id, SHOW_FORK);
	pthread_mutex_unlock(&philo->share->lock_timestamp);
	return (0);
}

int	taken_a_fork(t_philo *philo)
{
	if (get_left_fork(philo) == STOP)
		return (STOP);
	if (get_right_fork(philo) == STOP)
	{
		pthread_mutex_unlock(&(philo->share->m_fork[philo->left_fork_id]));
		return (STOP);
	}
	return (0);
}
