/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanusz <djanusz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/07/10 16:24:41 by djanusz           #+#    #+#             */
/*   Updated: 2023/07/10 16:32:29 by djanusz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

int	mutex_init(t_rules *rules)
{
	int	i;

	rules->all_forks = malloc(sizeof(pthread_mutex_t) * rules->nb_philo);
	if (!rules->all_forks)
		return (-1);
	pthread_mutex_init(&rules->print, NULL);
	i = -1;
	while (++i < rules->nb_philo)
		pthread_mutex_init(&rules->all_forks[i], NULL);
	return (0);
}

t_rules	*philo_init(t_rules *rules)
{
	int		i;

	rules->philo = malloc(sizeof(t_philo) * rules->nb_philo);
	if (!rules->philo || mutex_init(rules))
		return (write(1, "NOT ENOUGH MEMORY\n", 18), NULL);
	i = -1;
	while (++i < rules->nb_philo)
	{
		rules->philo[i].id = i + 1;
		rules->philo[i].die = rules->die;
		rules->philo[i].eat = rules->eat;
		rules->philo[i].sleep = rules->sleep;
		rules->philo[i].must_eat = rules->must_eat;
		rules->philo[i].end = 0;
		pthread_mutex_init(&rules->philo[i].check_last_eat, NULL);
		pthread_mutex_init(&rules->philo[i].check_must_eat, NULL);
		pthread_mutex_init(&rules->philo[i].check_end, NULL);
		rules->philo[i].print = &rules->print;
		rules->philo[i].r_fork = &rules->all_forks[(i + 1) % rules->nb_philo];
		rules->philo[i].l_fork = &rules->all_forks[i];
	}
	return (rules);
}

t_rules	*parsing(char **args)
{
	t_rules	*res;

	if (ft_strslen(args) != 4 && ft_strslen(args) != 5)
		return (write(1, "Wrong number of args\n", 21), NULL);
	res = malloc(sizeof(t_rules));
	if (!res)
		return (write(1, "NOT ENOUGH MEMORY\n", 18), NULL);
	res->nb_philo = ft_atoi(args[0]);
	res->die = ft_atoi(args[1]);
	res->eat = ft_atoi(args[2]);
	res->sleep = ft_atoi(args[3]);
	if (res->nb_philo < 1 || res->die < 1 || res->eat < 1 || res->sleep < 1)
		return (write(1, "Wrong input\n", 12), free(res), NULL);
	if (!args[4])
		res->must_eat = -1;
	else
	{
		res->must_eat = ft_atoi(args[4]);
		if (res->must_eat < 1)
			return (write(1, "Wrong input in must eat\n", 24), free(res), NULL);
	}
	return (philo_init(res));
}
