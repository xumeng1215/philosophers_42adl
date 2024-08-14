/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mexu <charlie_xumeng@hotmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 22:39:56 by mexu / char       #+#    #+#             */
/*   Updated: 2024/08/13 12:02:52 by mexu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	main(int ac, char **av)
{
	t_table	table;

	if (ac == 5 || ac == 6)
	{
		parse_args(av, &table);
		data_init(&table);
		dinner_simulator(&table);
		clean(&table);
	}
	else
		error_exit("Wrong input. Example: ./philo 5 800 200 200 [7]");
}
