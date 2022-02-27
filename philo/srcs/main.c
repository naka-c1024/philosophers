/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynakashi <ynakashi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/13 15:58:38 by ynakashi          #+#    #+#             */
/*   Updated: 2022/02/27 18:44:33 by ynakashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

int	main(int argc, char **argv)
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
		destroy_share(share);
		return (EXIT_FAILURE);
	}
	destroy_philos(philo, share->philo_num);
	destroy_share(share);
	return (EXIT_SUCCESS);
}
