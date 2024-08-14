/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_data.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mengxu <mengxu@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 14:33:25 by mexu              #+#    #+#             */
/*   Updated: 2024/08/13 13:27:35 by mengxu           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* 
philo with even number as id will take the fork with the same id as first fork,
and take the fork of the philo next to him (id+1) as second fork.
the philo with odd number id will take different forks as the first and second.
*/
static void	assign_fork(t_philo *philo, t_fork *forks)
{
	if (philo->id % 2 == 0)
	{
		philo->first_fork = &forks[philo->id - 1];
		philo->second_fork = &forks[philo->id % philo->table->philo_nbr];
	}
	else
	{
		philo->first_fork = &forks[philo->id % philo->table->philo_nbr];
		philo->second_fork = &forks[philo->id - 1];
	}
}

/* 
init philos
philo id will start from 1, which is the index i(starting from 0) plus 1
*/
static void	philo_init(t_table *table)
{
	int		i;
	t_philo	*philo;

	i = -1;
	while (++i < table->philo_nbr)
	{
		philo = table->philos + i;
		philo->id = i + 1;
		philo->flag_full = false;
		philo->meal_count = 0;
		philo->table = table;
		safe_mutex(&philo->philo_mutex, INIT);
		assign_fork(philo, table->forks);
	}
}

/* 
init forks
the fork id will be start at 1 as well, like the philos id.
*/
static void	fork_init(t_table *table)
{
	int	i;

	i = -1;
	while (++i < table->philo_nbr)
	{
		safe_mutex(&table->forks[i].fork, INIT);
		table->forks[i].fork_id = i + 1;
	}
}

/* 
init data in table
init forks
init philos
 */
void	data_init(t_table *table)
{
	table->end_simulation = false;
	table->philos = (t_philo *)safe_malloc(table->philo_nbr * sizeof(t_philo));
	table->forks = (t_fork *)safe_malloc(table->philo_nbr * sizeof(t_fork));
	safe_mutex(&table->table_mutex, INIT);
	safe_mutex(&table->write_mutex, INIT);
	fork_init(table);
	philo_init(table);
}
