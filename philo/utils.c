/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mengxu <mengxu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 12:49:20 by mexu              #+#    #+#             */
/*   Updated: 2024/08/13 13:23:59 by mengxu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// output error messages then exit
void	error_exit(const char *msg)
{
	printf(R "Error: %s\n" RST, msg);
	exit(EXIT_FAILURE);
}

/*
get time utils, return long value microsecond usec
gettimeofday() will have seconds(tv_sec) and microseconds(tv_usec)
 1 second == 1000 millisecond (msec)
 1 millisecond == 1000 microsecond (usec)
 */
long	get_time(void)
{
	struct timeval	tv;

	if (gettimeofday(&tv, NULL))
		error_exit("gettimeofday failed.");
	return (tv.tv_sec * 1e6 + tv.tv_usec);
}

// precise usleep
// will check if the simulation is finished or not
void	precise_usleep(long usec, t_table *table)
{
	long	start;
	long	elapsed;
	long	remaining;

	start = get_time();
	while (get_time() - start < usec)
	{
		if (simulation_finished(table))
			break ;
		elapsed = get_time() - start;
		remaining = usec - elapsed;
		if (remaining > 1e4)
			usleep(remaining / 2);
		else
			while (get_time() - start < usec)
				;
	}
}

// clean the table
// destroy mutex for the philos and table
void	clean(t_table *table)
{
	t_philo	*philo;
	t_fork	*fork;
	int		i;

	i = -1;
	while (++i < table->philo_nbr)
	{
		philo = table->philos + i;
		fork = table->forks + i;
		safe_mutex(&philo->philo_mutex, DESTROY);
		safe_mutex(&fork->fork, DESTROY);
	}
	safe_mutex(&table->write_mutex, DESTROY);
	safe_mutex(&table->table_mutex, DESTROY);
	free(table->forks);
	free(table->philos);
}
