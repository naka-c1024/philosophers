/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check_arg.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynakashi <ynakashi@student.42tokyo.jp>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/02/14 08:51:15 by ynakashi          #+#    #+#             */
/*   Updated: 2022/02/14 08:51:51 by ynakashi         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/philosophers.h"

void	show_help(void)
{
	printf("Usage: ./philo num die eat sleep [ate]\n\n");
	printf("\tnum:\tnumber_of_philosophers\n");
	printf("\tdie:\ttime_to_die\n");
	printf("\teat:\ttime_to_eat\n");
	printf("\tsleep:\ttime_to_sleep\n");
	printf("\tate(option):\n");
	printf("\tnumber_of_times_each_philosopher_must_eat\n");
	printf("\n");
}

size_t	ft_strlen(char *str)
{
	size_t	i;

	i = 0;
	while (str[i])
		i++;
	return (i);
}

int	ft_isdigit(int c)
{
	return (('0' <= c && c <= '9'));
}

bool	is_uint(char *str)
{
	size_t	i;
	size_t	len;
	long	ans;

	ans = 0;
	i = 0;
	len = ft_strlen(str);
	if (str[i] == '-')
		return (false);
	if (str[i] == '+' && len != 1)
		i++;
	while (ft_isdigit(str[i]))
	{
		if (ans > (INT_MAX - (str[i] - '0')) / 10)
			return (false);
		ans *= 10;
		ans += str[i] - '0';
		i++;
	}
	if (len != i)
		return (false);
	return (true);
}

bool	check_arg(int argc, char **argv)
{
	int	i;

	if (argc != 5 && argc != 6)
	{
		show_help();
		return (false);
	}
	i = 1;
	while (i < argc)
	{
		if (is_uint(argv[i]) == false)
		{
			show_help();
			return (false);
		}
		i++;
	}
	return (true);
}
