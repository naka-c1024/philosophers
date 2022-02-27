/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   create_philo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynakashi <ynakashi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 18:19:12 by ynakashi          #+#    #+#             */
/*   Updated: 2022/02/27 18:21:40 by ynakashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

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

	i = 0;
	while (i < share->philo_num)
		pthread_mutex_init(&share->m_fork[i++], NULL);
	pthread_mutex_init(&share->lock_share_var, NULL);
	pthread_mutex_init(&share->lock_timestamp, NULL);
	philo = first_philo(share);
	if (!philo)
	{
		destroy_share(share);
		return (NULL);
	}
	i = 1;
	while (i < share->philo_num)
	{
		new = (t_philo *)malloc(sizeof(t_philo));
		if (new == NULL)
		{
			destroy_philos(philo, i);
			destroy_share(share);
			return (NULL);
		}
		new->id = i + 1;
		new->right_fork_id = i;
		new->left_fork_id = (i + 1) % share->philo_num;
		new->ate_count = 0;
		new->die_limit_time = 0;
		new->share = share;
		philo->right->left = new;
		new->right = philo->right;
		new->left = philo;
		philo->right = new;
		i++;
	}
	return (philo);
}
