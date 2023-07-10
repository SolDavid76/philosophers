/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   monitoring.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanusz <djanusz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 16:27:26 by djanusz           #+#    #+#             */
/*   Updated: 2023/07/10 16:32:32 by djanusz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	print_action(t_philo *philo, char *msg)
{
	pthread_mutex_lock(philo->print);
	if (check_end(philo))
		return (pthread_mutex_unlock(philo->print), 1);
	printf("%lu %d %s\n", get_time() - philo->start, philo->id, msg);
	pthread_mutex_unlock(philo->print);
	return (0);
}

void	start_threads(t_rules *rules)
{
	int	i;

	i = -1;
	rules->start = get_time();
	while (++i < rules->nb_philo)
	{
		rules->philo[i].start = rules->start;
		rules->philo[i].last_eat = rules->start;
		pthread_create(&rules->philo[i].th, NULL, routine, &rules->philo[i]);
	}
}

int	someone_died(t_philo *philo, int nb_philo, unsigned long die)
{
	int	i;

	i = -1;
	while (++i < nb_philo)
	{
		if ((check_last_eat(&philo[i]) + die) < get_time())
		{
			print_action(&philo[i], "died");
			return (1);
		}
	}
	return (0);
}

int	evryone_eat(t_philo *philo, int nb_philo)
{
	int	i;

	i = -1;
	while (++i < nb_philo)
	{
		if (check_must_eat(&philo[i]))
			return (0);
	}
	return (1);
}

int	monitoring(t_philo *philo, int nb_philo, unsigned long die)
{
	int	i;

	while (42)
	{
		if (someone_died(philo, nb_philo, die) || evryone_eat(philo, nb_philo))
		{
			i = -1;
			while (++i < nb_philo)
			{
				pthread_mutex_lock(&philo[i].check_end);
				philo[i].end = 1;
				pthread_mutex_unlock(&philo[i].check_end);
			}
			return (1);
		}
		usleep(100);
	}
	return (0);
}
