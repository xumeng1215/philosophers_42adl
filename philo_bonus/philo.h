/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mexu <charlie_xumeng@hotmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/16 22:39:49 by mexu / char       #+#    #+#             */
/*   Updated: 2024/08/13 12:16:28 by mexu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <pthread.h>
# include <stdbool.h>
# include <errno.h>
# include <sys/time.h>

# define INPUT_NUMBER_LENGTH 5
# define DEBUG_MODE 1

// colors in the output
// ASCII excape character
# define RST "\033[0m"
# define R "\033[1;31m"
# define G "\033[1;32m"
# define B "\033[1;34m"

// pthread_mutex_t is too long
typedef pthread_mutex_t	t_mtx;

// opcode for mutex and thread
typedef enum e_opcode
{
	LOCK = 1,
	UNLOCK = 2,
	INIT = 3,
	DESTROY = 4,
	CREATE = 5,
	JOIN = 6,
	DETACH = 7
}	t_opcode;

// status of philosophers
typedef enum e_philo_status
{
	EATING,
	SLEEPING,
	THINKING,
	TAKE_FIRST_FORK,
	TAKE_SECOND_FORK,
	DEAD
	// FULL,
}	t_philo_status;

typedef struct s_philo	t_philo;

typedef struct s_fork
{
	int		fork_id;
	t_mtx	fork;
}	t_fork;

typedef struct s_table
{
	long		philo_nbr;
	long		time_to_die;
	long		time_to_eat;
	long		time_to_sleep;
	long		meals_limits;
	bool		end_simulation;
	t_fork		*forks;
	t_philo		*philos;
	bool		all_thread_ready;
	t_mtx		table_mutex;
	t_mtx		write_mutex;
	pthread_t	monitor;
	long		start_simulation_timestamp;
}	t_table;

typedef struct s_philo
{
	int			id;
	long		meal_count;
	bool		flag_full;
	t_fork		*first_fork;
	t_fork		*second_fork;
	long		last_meal_time;
	pthread_t	thread_id;
	t_table		*table;
	t_mtx		philo_mutex;
}	t_philo;

// main
void	parse_args(char **av, t_table *table);
void	data_init(t_table *table);
void	dinner_simulator(t_table *table);
void	*philo_simulator(void *data);
void	*one_philo(void *data);

// monitor
void	*monitor_dinner(void *data);
bool	simulation_finished(t_table *table);

// utils
void	error_exit(const char *msg);
long	get_time(void);
void	precise_usleep(long usec, t_table *table);
void	clean(t_table *table);

// sync utils
// void wait_all_threads(t_table *table);
// bool all_threads_running(t_mtx *mutex, long *threads_count, long philo_nbr);
// void de_sync_philos(t_philo *philo);

// get and set
void	set_bool(t_mtx *mutex, bool *dest, bool value);
void	set_long(t_mtx *mutex, long *dest, long value);
bool	get_bool(t_mtx *mutex, bool *value);
long	get_long(t_mtx *mutex, long *value);

// safe functions
void	*safe_malloc(size_t bytes);
void	safe_mutex(t_mtx *mtx, t_opcode opcode);
void	safe_thread(pthread_t *thrd,
			void *(*start_routine)(void *), void *data, t_opcode opcode);

// philo routine
void	eating(t_philo *philo);
void	sleeping(t_philo *philo);
void	thinking(t_philo *philo);

// writer
void	write_status(t_philo_status status, t_philo *philo, bool debug);
#endif
