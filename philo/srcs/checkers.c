/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   checkers.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanusz <djanusz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 16:25:44 by djanusz           #+#    #+#             */
/*   Updated: 2023/07/10 16:32:40 by djanusz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	check_end(t_philo *philo)
{
	int	tmp;

	pthread_mutex_lock(&philo->check_end);
	tmp = philo->end;
	pthread_mutex_unlock(&philo->check_end);
	return (tmp);
}

int	check_must_eat(t_philo *philo)
{
	int	tmp;

	pthread_mutex_lock(&philo->check_must_eat);
	tmp = philo->must_eat;
	pthread_mutex_unlock(&philo->check_must_eat);
	return (tmp);
}

unsigned long	check_last_eat(t_philo *philo)
{
	unsigned long	tmp;

	pthread_mutex_lock(&philo->check_last_eat);
	tmp = philo->last_eat;
	pthread_mutex_unlock(&philo->check_last_eat);
	return (tmp);
}
