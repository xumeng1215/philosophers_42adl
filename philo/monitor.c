/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitor.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mengxu <mengxu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 23:55:28 by mexu              #+#    #+#             */
/*   Updated: 2024/08/13 13:23:02 by mengxu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* 
check if the simulation is finished or not
 */
bool	simulation_finished(t_table *table)
{
	return (get_bool(&table->table_mutex, &table->end_simulation));
}

/* 
check philo is dead or not
if the philo is full, just return
if the epalsed time from last meal timestamp is larger than the time_to_die,
return true, the philo is dead.
protect the code using philo mutex from routine functions from philo thread.
otherwise return false.
 */
static bool	philo_died(t_philo *philo)
{
	bool	result;

	result = false;
	if (get_bool(&philo->philo_mutex, &philo->flag_full))
		return (result);
	safe_mutex(&philo->philo_mutex, LOCK);
	if ((get_time() - philo->last_meal_time)
		> (philo->table->time_to_die))
		result = true;
	safe_mutex(&philo->philo_mutex, UNLOCK);
	return (result);
}

/* 
for the thread of monitor
wait for all the philo threads are running.
loop untile the simulation is finished
loop through the philos, check if the philo is dead or not
if dead, write status, then set end_simulation to true
 */
void	*monitor_dinner(void *data)
{
	t_table	*table;
	int		i;

	table = (t_table *)data;
	while (!simulation_finished(table))
	{
		i = -1;
		while (++i < table->philo_nbr && !simulation_finished(table))
		{
			if (philo_died(table->philos + i))
			{
				write_status(DEAD, table->philos + i, DEBUG_MODE);
				set_bool(&table->table_mutex, &table->end_simulation, true);
			}
		}
	}
	return (NULL);
}
