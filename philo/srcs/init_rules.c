/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_rules.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynakashi <ynakashi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 11:11:25 by ynakashi          #+#    #+#             */
/*   Updated: 2022/02/22 08:52:23 by ynakashi         ###   ########.fr       */
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

t_rules	*init_rules(int argc, char **argv)
{
	t_rules	*rules;

	rules = (t_rules *)malloc(sizeof(t_rules));
	if (!rules)
		return (NULL);
	rules->philo_num = ft_atoi(argv[1]);
	rules->die_time = ft_atoi(argv[2]);
	rules->eat_time = ft_atoi(argv[3]);
	rules->sleep_time = ft_atoi(argv[4]);
	if (argc == 6)
		rules->ate_num = ft_atoi(argv[5]);
	else
		rules->ate_num = -1;

	rules->m_fork = (pthread_mutex_t *)malloc(sizeof(pthread_mutex_t) * rules->philo_num);
	if (rules->m_fork == NULL)
	{
		free(rules);
		return (NULL);
	}
	rules->die_flg = 0;
	rules->ate = 0;
	rules->all_ate = 0;
	rules->first_timestamp = 0;

	return (rules);
}
