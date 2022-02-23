/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_share.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynakashi <ynakashi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/23 21:30:22 by ynakashi          #+#    #+#             */
/*   Updated: 2022/02/23 21:30:24 by ynakashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

static int	read_sign(char c)
{
	if (c == '-')
		return (-1);
	else
		return (1);
}

static int	is_space(char c)
{
	if ((c == ' ') || (c == '\t') || (c == '\n')
		|| (c == '\v') || (c == '\r') || (c == '\f'))
		return (1);
	else
		return (0);
}

int	ft_atoi(const char *str)
{
	size_t	i;
	int		sign;
	long	ans;

	ans = 0;
	i = 0;
	sign = 1;
	while (is_space(str[i]))
		i++;
	if (str[i] == '+' || str[i] == '-')
		sign = read_sign(str[i++]);
	while (ft_isdigit(str[i]))
	{
		if ((ans * 10 + (str[i] - '0')) / 10 != ans)
		{
			if (sign == -1)
				return ((int)LONG_MIN);
			else
				return ((int)LONG_MAX);
		}
		ans *= 10;
		ans += str[i] - '0';
		i++;
	}
	return (ans * sign);
}

t_share	*init_share(int argc, char **argv)
{
	t_share	*share;

	share = (t_share *)malloc(sizeof(t_share));
	if (!share)
		return (NULL);
	share->philo_num = ft_atoi(argv[1]);
	share->die_time = ft_atoi(argv[2]);
	share->eat_time = ft_atoi(argv[3]);
	share->sleep_time = ft_atoi(argv[4]);
	if (argc == 6)
		share->ate_num = ft_atoi(argv[5]);
	else
		share->ate_num = -1;

	share->m_fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * share->philo_num);
	if (share->m_fork == NULL)
	{
		free(share);
		return (NULL);
	}
	share->die_flg = 0;
	share->equal_ate_cnt = 0;
	share->all_ate_flg = 0;

	return (share);
}
