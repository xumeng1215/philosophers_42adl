/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_safety.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mexu <charlie_xumeng@hotmail.com>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/07/31 14:35:07 by mexu              #+#    #+#             */
/*   Updated: 2024/08/13 12:21:30 by mexu             ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	*safe_malloc(size_t bytes)
{
	void	*result;

	result = malloc(bytes);
	if (result == NULL)
		error_exit("error when malloc.");
	return (result);
}

//output the error code and opcode when status is not 0 (something went wrong.)
static void	handle_status(int status, t_opcode opcode)
{
	if (status == 0)
		return ;
	else
	{
		printf(R "opcode is %d, returned status is %d.\n" RST, opcode, status);
		error_exit("something went wrong. check status value.");
	}
}

//use opcode to handle mutex
void	safe_mutex(t_mtx *mtx, t_opcode opcode)
{
	if (opcode == INIT)
		handle_status(pthread_mutex_init(mtx, NULL), opcode);
	else if (opcode == LOCK)
		handle_status(pthread_mutex_lock(mtx), opcode);
	else if (opcode == UNLOCK)
		handle_status(pthread_mutex_unlock(mtx), opcode);
	else if (opcode == DESTROY)
		handle_status(pthread_mutex_destroy(mtx), opcode);
	else
		error_exit("wrong opcode for mutex handle.");
}

//use opcode to handle thread
void	safe_thread(pthread_t *thrd, void *(*start_routine)(void *),
		void *data, t_opcode opcode)
{
	if (opcode == CREATE)
		handle_status(pthread_create(thrd, NULL, start_routine, data), opcode);
	else if (opcode == JOIN)
		handle_status(pthread_join(*thrd, NULL), opcode);
	else if (opcode == DETACH)
		handle_status(pthread_detach(*thrd), opcode);
	else
		error_exit("wrong opcode for thread handle.");
}
