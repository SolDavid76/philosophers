/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: djanusz <djanusz@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/06/28 15:23:45 by djanusz           #+#    #+#             */
/*   Updated: 2023/06/30 17:41:57 by djanusz          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

t_philo	*philo_init(t_rules *rules)
{
	t_philo	*philo;
	int		i;

	philo = malloc(sizeof(t_philo) * rules->nb_philo);
	if (!philo)
		return (write(1, "NOT ENOUGH MEMORY\n", 18), free(rules), NULL);
	i = 0;
	while (i < rules->nb_philo)
	{
		philo[i].die = rules->die;
		philo[i].eat = rules->eat;
		philo[i].sleep = rules->sleep;
		philo[i].must_eat = rules->must_eat;
		philo[i].rules = rules;
		i++;
	}
	return (philo);
}

t_philo	*parsing(char **args)
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
	if (!args[4])
		res->must_eat = -1;
	else
	{
		res->must_eat = ft_atoi(args[4]);
		if (res->must_eat < 1)
			return (write(1, "Wrong input in must eat\n", 24), free(res), NULL);
	}
	if (res->nb_philo < 1 || res->die < 1 || res->eat < 1 || res->sleep < 1)
		return (write(1, "Wrong input\n", 12), free(res), NULL);
	return (philo_init(res));
}

unsigned long	get_time(void)
{
	struct timeval	time;

	gettimeofday(&time, NULL);
	return ((time.tv_sec * 1000) + (time.tv_usec / 1000));
}

void	*routine(void *arg)
{
	t_philo *philo = arg;

	philo->start = get_time();
	philo->index = 0;
	while (42)
	{
		philo->now = get_time();
		if (philo->now - philo->start >= philo->rules->sleep * philo->index)
		{
			printf("%lu\n", philo->now - philo->start);
			philo->index++;
		}
		usleep(philo->rules->sleep); //AMONGUS
	}
}

int	main(int ac, char **av)
{
	t_philo	*philo;

	(void)ac;
	philo = parsing(av + 1);
	if (!philo)
		return (1);
	pthread_create(&(philo[0].id), NULL, routine, &philo[0]);
	printf("nb_philo = %d, die = %d, eat = %d, sleep = %d, must_eat = %d\n", philo->rules->nb_philo, philo->rules->die, philo->rules->eat, philo->rules->sleep, philo->rules->must_eat);
	while (42)
	{
	}
}
