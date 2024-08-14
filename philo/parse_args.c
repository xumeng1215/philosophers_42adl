/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_args.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mexu <charlie_xumeng@hotmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 13:09:05 by mexu              #+#    #+#             */
/*   Updated: 2024/08/13 12:07:47 by mexu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* 
validate arg first, then convert to number
only digits allowed. 
not too long. 4 digits means less than 9999. 
*/
static long	ft_atol(const char *str)
{
	long	num;
	int		i;

	i = 0;
	num = 0;
	while (str[i])
	{
		if (!(str[i] >= '0' && str[i] <= '9'))
			error_exit("input digits only.");
		num = num * 10 + (str[i] - '0');
		i++;
	}
	if (i >= INPUT_NUMBER_LENGTH)
		error_exit("input number has too many digits.");
	return (num);
}

/* 
init table with args.
time (input as msec) will be convert to microsecond (usec) by *1000
all timestamps in the program is going to be count as usec
until write the status output the timestamp as msec
*/
void	parse_args(char **av, t_table *table)
{
	table->philo_nbr = ft_atol(av[1]);
	table->time_to_die = ft_atol(av[2]) * 1e3;
	table->time_to_eat = ft_atol(av[3]) * 1e3;
	table->time_to_sleep = ft_atol(av[4]) * 1e3;
	if (table->time_to_die < 6e4
		||table->time_to_eat < 6e4
		||table->time_to_sleep < 6e4)
		error_exit("input number for timestamp is too small.");
	if (av[5])
		table->meals_limits = ft_atol(av[5]);
	else
		table->meals_limits = -1;
}
