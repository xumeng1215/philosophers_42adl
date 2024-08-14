/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mexu <charlie_xumeng@hotmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/08/09 13:44:27 by mexu              #+#    #+#             */
/*   Updated: 2024/08/13 12:19:04 by mexu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/*
take fisrt fork, write status
take second fork, write status
set last meal time
increase the meal count
write status eating
usleep time_to_eat
set philo to full if the meal count reach the limit
unlock forks (mutex)
*/
void	eating(t_philo *philo)
{
	safe_mutex(&philo->first_fork->fork, LOCK);
	write_status(TAKE_FIRST_FORK, philo, DEBUG_MODE);
	safe_mutex(&philo->second_fork->fork, LOCK);
	write_status(TAKE_SECOND_FORK, philo, DEBUG_MODE);
	set_long(&philo->philo_mutex, &philo->last_meal_time, get_time());
	philo->meal_count++;
	write_status(EATING, philo, DEBUG_MODE);
	precise_usleep(philo->table->time_to_eat, philo->table);
	if (philo->table->meals_limits > 0
		&& philo->meal_count == philo->table->meals_limits)
		set_bool(&philo->philo_mutex, &philo->flag_full, true);
	safe_mutex(&philo->second_fork->fork, UNLOCK);
	safe_mutex(&philo->first_fork->fork, UNLOCK);
}

/*
write status
usleep the time_to_sleep
*/
void	sleeping(t_philo *philo)
{
	write_status(SLEEPING, philo, DEBUG_MODE);
	precise_usleep(philo->table->time_to_sleep, philo->table);
}

/*
write status
if the philo_nbr is even, the system is already fair, just return
otherwise when the philo_nbr is odd, calculate the available thinking time,
force the philo think half of the avaiable time by usleep
*/
void	thinking(t_philo *philo)
{
	long	max_thinking_time;

	write_status(THINKING, philo, DEBUG_MODE);
	if (philo->table->philo_nbr % 2 == 0)
		return ;
	max_thinking_time = philo->table->time_to_die
		- philo->table->time_to_eat
		- philo->table->time_to_sleep;
	precise_usleep(max_thinking_time * 0.5, philo->table);
}
