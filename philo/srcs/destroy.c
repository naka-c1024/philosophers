/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   destroy.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynakashi <ynakashi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/27 18:22:53 by ynakashi          #+#    #+#             */
/*   Updated: 2022/02/27 18:23:21 by ynakashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	destroy_philos(t_philo *philos, int count)
{
	t_philo	*tmp;

	while (count--)
	{
		tmp = philos->left;
		free(philos);
		philos = tmp;
	}
}

void	destroy_share(t_share *share)
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
	pthread_mutex_destroy(&share->lock_timestamp);
	free(share);
}
