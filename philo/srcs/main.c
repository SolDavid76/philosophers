/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanusz <djanusz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 15:23:45 by djanusz           #+#    #+#             */
/*   Updated: 2023/07/10 16:27:17 by djanusz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	ft_exit(t_rules *rules)
{
	int	i;

	i = -1;
	while (++i < rules->nb_philo)
	{
		pthread_mutex_destroy(&rules->philo[i].check_end);
		pthread_mutex_destroy(&rules->philo[i].check_must_eat);
		pthread_mutex_destroy(&rules->philo[i].check_last_eat);
		pthread_mutex_destroy(rules->philo[i].l_fork);
	}
	pthread_mutex_destroy(&rules->print);
	free(rules->all_forks);
	free(rules->philo);
	free(rules);
	return (0);
}

int	main(int ac, char **av)
{
	t_rules	*rules;
	int		i;

	(void)ac;
	rules = parsing(av + 1);
	if (!rules)
		return (1);
	start_threads(rules);
	if (!monitoring(rules->philo, rules->nb_philo, rules->die))
		return (1);
	i = -1;
	while (++i < rules->nb_philo)
		pthread_join(rules->philo[i].th, NULL);
	return (ft_exit(rules));
}
