/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   write_status.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mengxu <mengxu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/01 17:00:54 by mexu              #+#    #+#             */
/*   Updated: 2024/08/13 13:07:19 by mengxu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

// debug function
// will output which fork is taken and the meal count
static void	status_debug(t_philo_status status, t_philo *philo, long time)
{
	if (!simulation_finished(philo->table))
	{
		if (status == TAKE_FIRST_FORK)
			printf(G "%-8ld" RST "%d took 1st fork. fork id 🍴%d.\n", time,
				philo->id, philo->first_fork->fork_id);
		else if (status == TAKE_SECOND_FORK)
			printf(G "%-8ld" RST "%d took 2nd fork. fork id 🍴%d.\n", time,
				philo->id, philo->second_fork->fork_id);
		else if (status == EATING)
			printf(G "%-8ld" RST "%d is eating. meals count 🍲%ld.\n", time,
				philo->id, philo->meal_count);
		else if (status == SLEEPING)
			printf(G "%-8ld" RST "%d is sleeping😴.\n", time, philo->id);
		else if (status == THINKING)
			printf(G "%-8ld" RST "%d is thinking💭.\n", time, philo->id);
		else if (status == DEAD)
			printf(G "%-8ld" RST "%d is " R "dead💀\n" RST, time, philo->id);
	}
}

// calculate the timestamp first
// check if the philo is full or not, if full, just return
// output the status as required (can change to status_debug for output)
// the timestamp is output as msec
void	write_status(t_philo_status status, t_philo *philo, bool debug)
{
	long	time;

	time = (get_time() - philo->table->start_simulation_timestamp) / 1e3;
	if (philo->flag_full)
		return ;
	safe_mutex(&philo->table->write_mutex, LOCK);
	if (debug)
		status_debug(status, philo, time);
	else
	{
		if (!simulation_finished(philo->table))
		{
			if (status == TAKE_FIRST_FORK || status == TAKE_SECOND_FORK)
				printf(G "%-8ld" RST "%d has taken a fork\n", time, philo->id);
			else if (status == EATING)
				printf(G "%-8ld" RST "%d is eating\n", time, philo->id);
			else if (status == SLEEPING)
				printf(G "%-8ld" RST "%d is sleeping\n", time, philo->id);
			else if (status == THINKING)
				printf(G "%-8ld" RST "%d is thinking\n", time, philo->id);
			else if (status == DEAD)
				printf(R "%-8ld" RST "%d died\n", time, philo->id);
		}
	}
	safe_mutex(&philo->table->write_mutex, UNLOCK);
}
