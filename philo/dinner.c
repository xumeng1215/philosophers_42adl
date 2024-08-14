/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   dinner.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mengxu <mengxu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 10:49:30 by mexu              #+#    #+#             */
/*   Updated: 2024/08/13 13:27:45 by mengxu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
simulate one philo.
one philo will never eat (only 1 fork) and will die after time_to_die
*/
void	*one_philo(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	set_long(&philo->philo_mutex, &philo->last_meal_time, get_time());
	write_status(TAKE_FIRST_FORK, philo, DEBUG_MODE);
	while (!simulation_finished(philo->table))
		sleeping(philo);
	return (NULL);
}

/*
simulate a philo having dinner
wait until everyone have a thread ready

increase the table's thread count by 1
loop until simulation is finished
if this philo is full, break the loop
eat->sleep->think */
void	*philo_simulator(void *data)
{
	t_philo	*philo;

	philo = (t_philo *)data;
	set_long(&philo->philo_mutex, &philo->last_meal_time, get_time());
	while (!simulation_finished(philo->table))
	{
		if (philo->flag_full)
			break ;
		eating(philo);
		sleeping(philo);
		thinking(philo);
	}
	return (NULL);
}

// if the meal limits is 0, just return.
// set table start simulation time
// if there's only 1 philosopher, create one thread using different function
// otherwise create thread for each philo with dinner simulation
// create a monitor thread to watch the dinner
// set table all_thread_ready to true, then the philo thread will continue
// join the threads at the end
void	dinner_simulator(t_table *table)
{
	int	i;

	if (table->meals_limits == 0)
		return ;
	table->start_simulation_timestamp = get_time();
	if (table->philo_nbr == 1)
		safe_thread(&table->philos[0].thread_id, one_philo,
			&table->philos[0], CREATE);
	else
	{
		i = -1;
		while (++i < table->philo_nbr)
			safe_thread(&table->philos[i].thread_id, philo_simulator,
				&table->philos[i], CREATE);
	}
	safe_thread(&table->monitor, monitor_dinner, table, CREATE);
	i = -1;
	while (++i < table->philo_nbr)
		safe_thread(&table->philos[i].thread_id, NULL, NULL, JOIN);
	set_bool(&table->table_mutex, &table->end_simulation, true);
	safe_thread(&table->monitor, NULL, NULL, JOIN);
	printf(B "dinner finished.\n" RST);
}
