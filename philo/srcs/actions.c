/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   actions.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanusz <djanusz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 16:26:33 by djanusz           #+#    #+#             */
/*   Updated: 2023/07/10 16:49:35 by djanusz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

void	unlock_fork(pthread_mutex_t *l_fork, pthread_mutex_t *r_fork)
{
	if (l_fork)
		pthread_mutex_unlock(l_fork);
	if (r_fork)
		pthread_mutex_unlock(r_fork);
}

int	lock_fork(t_philo *philo)
{
	if (philo->id % 2)
	{
		pthread_mutex_lock(philo->l_fork);
		if (print_action(philo, "has taken a fork"))
			return (unlock_fork(philo->l_fork, NULL), 1);
		if (philo->l_fork == philo->r_fork)
			return (unlock_fork(philo->l_fork, NULL), 1);
		pthread_mutex_lock(philo->r_fork);
		if (print_action(philo, "has taken a fork"))
			return (unlock_fork(philo->l_fork, philo->r_fork), 1);
	}
	else
	{
		pthread_mutex_lock(philo->r_fork);
		if (print_action(philo, "has taken a fork"))
			return (unlock_fork(NULL, philo->r_fork), 1);
		if (philo->l_fork == philo->r_fork)
			return (unlock_fork(NULL, philo->r_fork), 1);
		pthread_mutex_lock(philo->l_fork);
		if (print_action(philo, "has taken a fork"))
			return (unlock_fork(philo->l_fork, philo->r_fork), 1);
	}
	return (0);
}

int	eating(t_philo *philo)
{
	if (lock_fork(philo))
		return (1);
	if (print_action(philo, "is eating"))
		return (unlock_fork(philo->l_fork, philo->r_fork), 1);
	pthread_mutex_lock(&philo->check_last_eat);
	philo->last_eat = get_time();
	pthread_mutex_unlock(&philo->check_last_eat);
	usleep(philo->eat * 1000);
	unlock_fork(philo->l_fork, philo->r_fork);
	pthread_mutex_lock(&philo->check_must_eat);
	if (philo->must_eat)
		philo->must_eat--;
	pthread_mutex_unlock(&philo->check_must_eat);
	return (check_must_eat(philo) == 0);
}

int	thinking(t_philo *philo)
{
	unsigned long	time;

	time = get_time() - check_last_eat(philo);
	if (print_action(philo, "is thinking"))
		return (1);
	if (time < philo->die - 5)
		usleep((philo->die - time - 5) * 1000);
	return (0);
}

void	*routine(void *arg)
{
	t_philo	*philo;

	philo = arg;
	if (philo->id % 2 == 0)
		usleep((philo->eat * 1000) * 0.5);
	while (42)
	{
		if (eating(philo))
			return (NULL);
		if (print_action(philo, "is sleeping"))
			return (NULL);
		usleep(philo->sleep * 1000);
		if (thinking(philo))
			return (NULL);
	}
	return (NULL);
}
